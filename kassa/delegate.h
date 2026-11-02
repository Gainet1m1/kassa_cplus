#pragma once
#include "money.h"
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>
namespace Ui
{
class Delegate;
}
class Delegate : public QDialog
{
    Q_OBJECT
  public:
    explicit Delegate(QWidget *parent = nullptr);
    ~Delegate();
    void updateTableView(const QMap<int, Money::Amount> &data);
  private slots:
    void on_pushButton_clicked();

  private:
    Ui::Delegate *ui;
    QStandardItemModel *model;
    std::array<Money::Amount, 4> balances{};
};
