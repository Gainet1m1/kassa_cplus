#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QButtonGroup>
#include <QPushButton>
InputDialog::InputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    auto *group = new QButtonGroup(this);
    group->addButton(ui->checkBox1);
    group->addButton(ui->checkBox2);
    group->addButton(ui->checkBox3);
    group->setExclusive(true);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setAutoDefault(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(false);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    ui->inputAmount->setFocus();
}
InputDialog::~InputDialog() { delete ui; }
double InputDialog::getValue() const { return value / 100.0; }
int InputDialog::getNumberOfPeople()
{
    return ui->checkBox1->isChecked()   ? 1
           : ui->checkBox2->isChecked() ? 2
           : ui->checkBox3->isChecked() ? 3
                                        : 0;
}
bool InputDialog::submit()
{
    if (!Money::parse(ui->inputAmount->text(), value) || value <= 0)
    {
        ui->errorLabel->setText(
            tr("Введите положительную сумму, например 1250,50 (максимум 999999999,99)."));
        ui->inputAmount->setFocus();
        return false;
    }
    emit dataEntered(value, getNumberOfPeople());
    ui->errorLabel->clear();
    ui->inputAmount->clear();
    ui->inputAmount->setFocus();
    return true;
}
void InputDialog::on_buttonBox_accepted()
{
    if (submit())
        accept();
}
void InputDialog::on_inputAmount_returnPressed() { submit(); }
