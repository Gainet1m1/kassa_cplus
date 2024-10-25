#include "inputdialog.h"
#include "ui_inputdialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QKeyEvent> // Добавьте этот заголовочный файл

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputDialog)
    , value(0)
    , numberOfPeople(0) // Default to no people selected
    , dataEnteredFlag(false) // Добавьте этот флаг
{
    // Create the layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Amount input field
    inputAmount = new QLineEdit(this);
    inputAmount->setPlaceholderText("Enter Sold Amount");
    layout->addWidget(inputAmount);

    // Checkboxes for number of people
    checkBox1 = new QCheckBox("1 Person", this);
    checkBox2 = new QCheckBox("2 Persons", this);
    checkBox3 = new QCheckBox("3 Persons", this);

    layout->addWidget(checkBox1);
    layout->addWidget(checkBox2);
    layout->addWidget(checkBox3);

    // Button box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                           | QDialogButtonBox::Cancel,
                                                       this);
    layout->addWidget(buttonBox);

    // Connect signals
    connect(buttonBox, &QDialogButtonBox::accepted, this, &InputDialog::on_buttonBox_accepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InputDialog::reject);
    connect(inputAmount, &QLineEdit::returnPressed, this, &InputDialog::on_inputAmount_returnPressed);

    setLayout(layout); // Set the layout
}

InputDialog::~InputDialog()
{
    delete ui;
}

double InputDialog::getValue() const
{
    return value; // Return the input amount
}

int InputDialog::getNumberOfPeople()
{
    // Count how many checkboxes are checked
    numberOfPeople = 0;
    if (checkBox1->isChecked())
        numberOfPeople=1;
    if (checkBox2->isChecked())
        numberOfPeople=2;
    if (checkBox3->isChecked())
        numberOfPeople=3;
    return numberOfPeople; // Return the number of people selected
}

void InputDialog::on_buttonBox_accepted()
{
    value = inputAmount->text().toDouble(); // Get the amount entered
    accept();                               // Close the dialog
}

void InputDialog::on_inputAmount_returnPressed()
{
    // Get the amount entered and handle it accordingly
    value = inputAmount->text().toDouble();
    int numberOfPeople = getNumberOfPeople();

    // Emit the signal to send the data to MainWindow only if it hasn't been emitted before
    if (!dataEnteredFlag) {
        emit dataEntered(value, numberOfPeople);
        dataEnteredFlag = true; // Set the flag to true after emitting the signal
    }

    // Clear the input field for the next entry
    inputAmount->clear();

    // Optionally, you might want to set focus back to the input field if needed
    inputAmount->setFocus();

    // Reset the flag for the next input
    dataEnteredFlag = false;
}

void InputDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // Если нажата клавиша Enter, обновляем значение в поле ввода, но не закрываем диалог
        on_inputAmount_returnPressed();
        event->ignore(); // Игнорируем событие, чтобы предотвратить закрытие диалога
    }
    else
    {
        QDialog::keyPressEvent(event); // Передаем событие дальше
    }
}
