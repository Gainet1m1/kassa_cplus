#include "mainwindow.h"
#include "delegate.h"
#include "inputdialog.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    resize(860, 580);
    setMinimumSize(640, 400);
    model->setHorizontalHeaderLabels({tr("Операция"), tr("Сумма"), tr("Участник")});
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    ui->beforeIn->setMaximumWidth(180);
    connect(ui->beforeIn, &QLineEdit::textChanged, this, &MainWindow::updateTable);
    updateTable();
}
MainWindow::~MainWindow() { delete ui; }
void MainWindow::on_pushPlus_clicked()
{
    InputDialog dialog(this);
    dialog.setWindowTitle(tr("Приход"));
    connect(&dialog, &InputDialog::dataEntered, this, &MainWindow::addIncome);
    dialog.exec();
}
void MainWindow::on_pushMinus_clicked()
{
    InputDialog dialog(this);
    dialog.setWindowTitle(tr("Расход"));
    connect(&dialog, &InputDialog::dataEntered, this, &MainWindow::addExpense);
    dialog.exec();
}
void MainWindow::addEntry(Money::Amount amount, int participant, bool income)
{
    if (amount <= 0 || amount > 99999999999LL || participant < 0 || participant > 3)
        return;
    if (income)
        totalIncome += amount;
    else
        totalExpenses += amount;
    auto *type = new QStandardItem(income ? tr("Приход") : tr("Расход"));
    type->setData(income, Qt::UserRole);
    auto *value = new QStandardItem(Money::format(amount));
    value->setData(amount, Qt::UserRole);
    model->appendRow({type, value, new QStandardItem(QString::number(participant))});
    updateTable();
}
void MainWindow::addIncome(Money::Amount amount, int participant)
{
    addEntry(amount, participant, true);
}
void MainWindow::addExpense(Money::Amount amount, int participant)
{
    addEntry(amount, participant, false);
}
void MainWindow::updateTable()
{
    Money::Amount initial = 0;
    const bool valid = Money::parse(ui->beforeIn->text(), initial, true);
    ui->beforeIn->setToolTip(valid ? tr("Остаток до начала операций")
                                   : tr("Некорректная сумма: до 9 цифр и 2 знаков после запятой"));
    ui->Sum->setText(valid
                         ? tr("Итого: %1").arg(Money::format(initial + totalIncome - totalExpenses))
                         : tr("Проверьте начальный остаток"));
    ui->pushDel->setEnabled(valid);
}
void MainWindow::on_clear_clicked()
{
    if (model->rowCount() &&
        QMessageBox::question(
            this, tr("Очистка"),
            tr("Удалить все операции текущего сеанса? Начальный остаток сохранится.")) !=
            QMessageBox::Yes)
        return;
    totalIncome = totalExpenses = 0;
    model->removeRows(0, model->rowCount());
    updateTable();
}
void MainWindow::on_pushDel_clicked()
{
    QMap<int, Money::Amount> data;
    for (int i = 0; i < 4; ++i)
        data[i] = 0;
    for (int row = 0; row < model->rowCount(); ++row)
    {
        const auto amount = model->item(row, 1)->data(Qt::UserRole).toLongLong();
        data[model->item(row, 2)->text().toInt()] +=
            model->item(row, 0)->data(Qt::UserRole).toBool() ? amount : -amount;
    }
    Money::Amount initial;
    if (!Money::parse(ui->beforeIn->text(), initial, true))
        return;
    data[0] += initial;
    auto *dialog = new Delegate(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->updateTableView(data);
    dialog->show();
}
