#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "delegate.h" // Обновленное имя

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushPlus_clicked();
    void on_pushMinus_clicked();
    void on_clear_clicked();
    void on_pushDel_clicked(); // Добавьте этот слот

    void addIncome(double income, int numberOfPeople); // Добавьте этот слот
    void addExpense(double expense, int numberOfPeople); // Добавьте этот слот

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    double totalIncome;
    double totalExpenses;
    double initialCash;

    void updateTable();
};

#endif // MAINWINDOW_H
