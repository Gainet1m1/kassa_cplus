#pragma once
#include "money.h"
#include <QMainWindow>
#include <QStandardItemModel>
namespace Ui
{
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  private slots:
    void on_pushPlus_clicked();
    void on_pushMinus_clicked();
    void on_clear_clicked();
    void on_pushDel_clicked();
    void addIncome(Money::Amount amount, int participant);
    void addExpense(Money::Amount amount, int participant);

  private:
    void addEntry(Money::Amount amount, int participant, bool income);
    void updateTable();
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Money::Amount totalIncome = 0, totalExpenses = 0;
};
