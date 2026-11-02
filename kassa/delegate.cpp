#include "delegate.h"
#include "ui_delegate.h"
#include <QHeaderView>
Delegate::Delegate(QWidget *parent)
    : QDialog(parent), ui(new Ui::Delegate), model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    resize(680, 580);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->delegate->setMaximumHeight(60);
}
Delegate::~Delegate() { delete ui; }
void Delegate::updateTableView(const QMap<int, Money::Amount> &data)
{
    balances.fill(0);
    model->clear();
    model->setHorizontalHeaderLabels({tr("Участник"), tr("Чистые поступления")});
    for (int i = 0; i < 4; ++i)
    {
        balances[i] = data.value(i);
        model->appendRow({new QStandardItem(i ? tr("Участник %1").arg(i) : tr("Общая касса (0)")),
                          new QStandardItem(Money::format(balances[i]))});
    }
    ui->delegate->setPlainText("0.00");
    ui->label1->setText(tr("Введите сумму и нажмите «Рассчитать переводы». Остаток учитывается "
                           "через общую кассу (0)."));
}
void Delegate::on_pushButton_clicked()
{
    Money::Amount total;
    if (!Money::parse(ui->delegate->toPlainText(), total))
    {
        ui->label1->setText(
            tr("Введите неотрицательную сумму с точностью до копейки (до 999999999,99)."));
        return;
    }
    const auto target = Money::shares(total);
    auto balance = balances;
    QLabel *labels[] = {ui->label0, ui->label2, ui->label3};
    for (int i = 1; i <= 3; ++i)
    {
        balance[i] -= target[i - 1];
        labels[i - 1]->setText(tr("Участник %1: %2 (плюс — передать, минус — получить)")
                                   .arg(i)
                                   .arg(Money::format(balance[i])));
    }
    QStringList lines;
    for (const auto &transfer : Money::settle(balance))
        lines << tr("%1 → %2: %3")
                     .arg(transfer.from)
                     .arg(transfer.to)
                     .arg(Money::format(transfer.amount));
    ui->label1->setText(lines.isEmpty() ? tr("Переводы не требуются.") : lines.join('\n'));
}
