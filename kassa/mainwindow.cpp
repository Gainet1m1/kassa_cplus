#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputdialog.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , totalIncome(0)
    , totalExpenses(0)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "Type" << "Amount" << "People");
    ui->tableView->setModel(model);

    // Initialize initial cash from beforeIn field
    initialCash = ui->beforeIn->toPlainText().toDouble(); // Set initial cash amount
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushPlus_clicked()
{
    InputDialog dialog(this);
    dialog.setWindowTitle(tr("Add Income"));

    // Connect the signal from InputDialog to a slot in MainWindow
    connect(&dialog, &InputDialog::dataEntered, this, &MainWindow::addIncome);

    while (dialog.exec() == QDialog::Accepted) {
        // The dialog will be reopened after each Enter press
    }
}

void MainWindow::on_pushMinus_clicked()
{
    InputDialog dialog(this);
    dialog.setWindowTitle(tr("Add Expense"));

    // Connect the signal from InputDialog to a slot in MainWindow
    connect(&dialog, &InputDialog::dataEntered, this, &MainWindow::addExpense);

    while (dialog.exec() == QDialog::Accepted) {
        // The dialog will be reopened after each Enter press
    }
}

void MainWindow::updateTable()
{
    // Remove rows with zero values
    for (int row = model->rowCount() - 1; row >= 0; --row) {
        QStandardItem *item = model->item(row, 1);
        if (item && item->text().toDouble() == 0) {
            model->removeRow(row);
        }
    }

    // Update initial cash from beforeIn field
    initialCash = ui->beforeIn->toPlainText().toDouble();

    // Calculate total with initial cash from beforeIn field
    double total = initialCash + totalIncome - totalExpenses;
    ui->Sum->setText("Total: " + QString::number(total)); // Update displayed total
}

void MainWindow::on_clear_clicked()
{
    // Reset income and expenses but keep initial cash amount
    totalIncome = 0;
    totalExpenses = 0;
    model->removeRows(0, model->rowCount()); // Clear the table
    updateTable(); // Update total display to reflect the cleared amounts
}

void MainWindow::addIncome(double income, int numberOfPeople)
{
    totalIncome += income; // Add the income to the total
    model->appendRow(new QStandardItem("Income")); // Add a new row for the income
    model->setItem(model->rowCount() - 1, 1, new QStandardItem(QString::number(income))); // Set the income value
    model->setItem(model->rowCount() - 1, 2, new QStandardItem(QString::number(numberOfPeople))); // Set number of people

    updateTable(); // Update the display of the total
}

void MainWindow::addExpense(double expense, int numberOfPeople)
{
    totalExpenses += expense; // Add the expense to the total
    model->appendRow(new QStandardItem("Expense")); // Add a new row for the expense
    model->setItem(model->rowCount() - 1, 1, new QStandardItem(QString::number(expense))); // Set the expense value
    model->setItem(model->rowCount() - 1, 2, new QStandardItem(QString::number(numberOfPeople))); // Set number of people

    updateTable(); // Update the display of the total
}

void MainWindow::on_pushDel_clicked()
{
    Delegate *delegate = new Delegate(nullptr); // Создайте экземпляр Delegate без родителя
    delegate->setWindowFlags(Qt::Window); // Установите флаги окна
    delegate->move(this->pos() + QPoint(20, 20)); // Переместите форму Delegate относительно MainWindow

    // Соберите данные для таблицы
    QMap<int, double> data;
    for (int row = 0; row < model->rowCount(); ++row) {
        int numberOfPeople = model->item(row, 2)->text().toInt();
        double amount = model->item(row, 1)->text().toDouble();
        QString type = model->item(row, 0)->text();

        if (type == "Expense") {
            amount = -amount; // Вычитаем расходы
        }

        if (data.contains(numberOfPeople)) {
            data[numberOfPeople] += amount;
        } else {
            data[numberOfPeople] = amount;
        }
    }

    // Добавьте нулевого человека с начальным балансом
    double initialCash = ui->beforeIn->toPlainText().toDouble();
    data[0] += initialCash;

    // Обновите таблицу в форме Delegate
    delegate->updateTableView(data);

    delegate->show(); // Покажите форму Delegate как немодальный диалог
    delegate->raise(); // Переместите форму на передний план
}
