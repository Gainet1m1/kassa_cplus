#include "delegate.h"
#include "inputdialog.h"
#include "mainwindow.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QTextEdit>
#include <QTimer>
#include <QtTest>

class KassaTest : public QObject
{
    Q_OBJECT
  private slots:
    void moneyInput()
    {
        Money::Amount amount;
        QVERIFY(Money::parse(" 123,45 ", amount));
        QCOMPARE(amount, 12345LL);
        QVERIFY(Money::parse("0.01", amount));
        QCOMPARE(amount, 1LL);
        QVERIFY(Money::parse("-5.50", amount, true));
        QCOMPARE(amount, -550LL);
        QCOMPARE(Money::format(amount), QString("-5.50"));
        for (const auto &invalid : {"", "nan", "inf", "-1", "1.234", "1e5", "1000000000", "1,2,3"})
            QVERIFY2(!Money::parse(invalid, amount), invalid);
    }
    void settlement()
    {
        for (Money::Amount total : {0LL, 1LL, 2LL, 100LL, 99999999999LL})
        {
            const auto shares = Money::shares(total);
            QCOMPARE(shares[0] + shares[1] + shares[2], total);
            QVERIFY(shares[0] - shares[2] <= 1);
        }
        // Cover ties, all-positive, all-negative, mixed and exact balances.
        for (int a = -2; a <= 2; ++a)
            for (int b = -2; b <= 2; ++b)
                for (int c = -2; c <= 2; ++c)
                {
                    std::array<Money::Amount, 4> balance{0, a * 101, b * 101, c * 101};
                    balance[0] = -(balance[1] + balance[2] + balance[3]);
                    for (const auto &transfer : Money::settle(balance))
                    {
                        QVERIFY(transfer.amount > 0);
                        QVERIFY(transfer.from != transfer.to);
                        balance[transfer.from] -= transfer.amount;
                        balance[transfer.to] += transfer.amount;
                    }
                    for (auto remainder : balance)
                        QCOMPARE(remainder, 0LL);
                }
    }
    void inputButtonsAndEnter()
    {
        InputDialog dialog;
        auto *amount = dialog.findChild<QLineEdit *>("inputAmount");
        auto *first = dialog.findChild<QCheckBox *>("checkBox1");
        auto *third = dialog.findChild<QCheckBox *>("checkBox3");
        auto *buttons = dialog.findChild<QDialogButtonBox *>("buttonBox");
        int emitted = 0;
        Money::Amount last = 0;
        int participant = -1;
        connect(&dialog, &InputDialog::dataEntered, this,
                [&](Money::Amount value, int who)
                {
                    ++emitted;
                    last = value;
                    participant = who;
                });
        dialog.show();
        amount->setText("abc");
        buttons->button(QDialogButtonBox::Ok)->click();
        QCOMPARE(emitted, 0);
        QVERIFY(dialog.isVisible());
        first->click();
        third->click();
        QVERIFY(!first->isChecked());
        amount->setText("10,05");
        QTest::keyClick(amount, Qt::Key_Return);
        QCOMPARE(emitted, 1);
        QCOMPARE(last, 1005LL);
        QCOMPARE(participant, 3);
        QVERIFY(dialog.isVisible());
        QVERIFY(amount->text().isEmpty());
        amount->setText("2.10");
        buttons->button(QDialogButtonBox::Ok)->click();
        QCOMPARE(emitted, 2);
        QCOMPARE(last, 210LL);
        QCOMPARE(dialog.result(), int(QDialog::Accepted));
        InputDialog cancel;
        connect(&cancel, &InputDialog::dataEntered, this, [&] { ++emitted; });
        cancel.findChild<QLineEdit *>("inputAmount")->setText("99");
        cancel.findChild<QDialogButtonBox *>("buttonBox")
            ->button(QDialogButtonBox::Cancel)
            ->click();
        QCOMPARE(emitted, 2);
    }
    void journalAndInitialBalance()
    {
        MainWindow window;
        auto *table = window.findChild<QTableView *>("tableView");
        auto *initial = window.findChild<QLineEdit *>("beforeIn");
        auto *sum = window.findChild<QLabel *>("Sum");
        initial->setText("100,00");
        QVERIFY(QMetaObject::invokeMethod(&window, "addIncome", Qt::DirectConnection,
                                          Q_ARG(Money::Amount, 2505), Q_ARG(int, 1)));
        QVERIFY(QMetaObject::invokeMethod(&window, "addExpense", Qt::DirectConnection,
                                          Q_ARG(Money::Amount, 1000), Q_ARG(int, 2)));
        QCOMPARE(table->model()->rowCount(), 2);
        QVERIFY(sum->text().contains("115.05"));
        QVERIFY(!table->model()->flags(table->model()->index(0, 0)).testFlag(Qt::ItemIsEditable) ||
                table->editTriggers() == QAbstractItemView::NoEditTriggers);
        initial->setText("200");
        QVERIFY(sum->text().contains("215.05"));
        initial->setText("bad");
        QVERIFY(!window.findChild<QPushButton *>("pushDel")->isEnabled());
        initial->setText("200");
        window.findChild<QPushButton *>("pushDel")->click();
        auto *split = window.findChild<Delegate *>();
        QVERIFY(split);
        QCOMPARE(split->findChild<QTableView *>("tableView")->model()->rowCount(), 4);
        split->findChild<QTextEdit *>("delegate")->setPlainText("30");
        split->findChild<QPushButton *>("pushButton")->click();
        QVERIFY(split->findChild<QLabel *>("label0")->text().contains("15.05"));
        QVERIFY(split->findChild<QLabel *>("label2")->text().contains("-20.00"));
    }
    void clearJournalKeepsInitialCash()
    {
        MainWindow window;
        auto *initial = window.findChild<QLineEdit *>("beforeIn");
        initial->setText("50");
        QVERIFY(QMetaObject::invokeMethod(&window, "addIncome", Qt::DirectConnection,
                                          Q_ARG(Money::Amount, 1500), Q_ARG(int, 0)));
        QTimer::singleShot(0,
                           []
                           {
                               auto *confirmation =
                                   qobject_cast<QMessageBox *>(QApplication::activeModalWidget());
                               if (confirmation)
                                   confirmation->button(QMessageBox::No)->click();
                           });
        window.findChild<QPushButton *>("clear")->click();
        QCOMPARE(window.findChild<QTableView *>("tableView")->model()->rowCount(), 1);
        QTimer::singleShot(0,
                           []
                           {
                               auto *confirmation =
                                   qobject_cast<QMessageBox *>(QApplication::activeModalWidget());
                               if (confirmation)
                                   confirmation->button(QMessageBox::Yes)->click();
                           });
        window.findChild<QPushButton *>("clear")->click();
        QCOMPARE(window.findChild<QTableView *>("tableView")->model()->rowCount(), 0);
        QCOMPARE(initial->text(), QString("50"));
        QVERIFY(window.findChild<QLabel *>("Sum")->text().contains("50.00"));
    }
    void settlementRefreshAndInvalidInput()
    {
        Delegate dialog;
        dialog.updateTableView({{1, 3000}});
        dialog.updateTableView({{2, 1000}});
        dialog.findChild<QTextEdit *>("delegate")->setPlainText("30");
        dialog.findChild<QPushButton *>("pushButton")->click();
        QVERIFY(dialog.findChild<QLabel *>("label0")->text().contains("-10.00"));
        QVERIFY(dialog.findChild<QLabel *>("label2")->text().contains("0.00"));
        dialog.findChild<QTextEdit *>("delegate")->setPlainText("nan");
        dialog.findChild<QPushButton *>("pushButton")->click();
        QVERIFY(!dialog.findChild<QLabel *>("label1")->text().contains(QChar(0x2192)));
    }
};
QTEST_MAIN(KassaTest)
#include "tst_kassa.moc"
