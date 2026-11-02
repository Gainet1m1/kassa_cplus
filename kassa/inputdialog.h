#pragma once
#include "money.h"
#include <QDialog>
namespace Ui
{
class InputDialog;
}
class InputDialog : public QDialog
{
    Q_OBJECT
  public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();
    double getValue() const;
    int getNumberOfPeople();
  signals:
    void dataEntered(Money::Amount amount, int participant);
  private slots:
    void on_buttonBox_accepted();
    void on_inputAmount_returnPressed();

  private:
    bool submit();
    Ui::InputDialog *ui;
    Money::Amount value = 0;
};
