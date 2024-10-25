#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QKeyEvent> // Добавьте этот заголовочный файл

namespace Ui {
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
    void dataEntered(double value, int numberOfPeople); // Добавьте этот сигнал

private slots:
    void on_buttonBox_accepted();        // Handle OK button
    void on_inputAmount_returnPressed(); // Handle Enter key

protected:
    void keyPressEvent(QKeyEvent *event) override; // Добавьте этот метод

private:
    Ui::InputDialog *ui;
    double value;       // To store the input amount
    int numberOfPeople; // To store the number of people selected
    bool dataEnteredFlag; // Добавьте этот флаг

    // Указатели на виджеты
    QLineEdit *inputAmount;
    QCheckBox *checkBox1;
    QCheckBox *checkBox2;
    QCheckBox *checkBox3;
};

#endif // INPUTDIALOG_H
