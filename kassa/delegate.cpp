#include "delegate.h"
#include "ui_delegate.h"

// Глобальный массив dagestan
double dagestan[4] = {0.0, 0.0, 0.0, 0.0};

Delegate::Delegate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Delegate)
    , model(new QStandardItemModel(this)) // Инициализируйте модель
{
    ui->setupUi(this);
    ui->tableView->setModel(model); // Свяжите модель с QTableView
}

Delegate::~Delegate()
{
    delete ui;
}

void Delegate::updateTableView(const QMap<int, double>& data)
{
    // Очистите модель перед обновлением
    model->clear();

    // Установите заголовки столбцов
    model->setHorizontalHeaderLabels(QStringList() << "Number of People" << "Amount");

    // Заполните модель данными
    int row = 0;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        QStandardItem *itemPeople = new QStandardItem(QString::number(it.key()));
        QStandardItem *itemAmount = new QStandardItem(QString::number(it.value()));

        model->setItem(row, 0, itemPeople);
        model->setItem(row, 1, itemAmount);
        if (row < 4) {
            dagestan[it.key()] = it.value();
        }

        ++row;
    }

    // Отладочный вывод для проверки значений dagestan
    qDebug() << "dagestan values:";
    for (int i = 0; i < 4; ++i) {
        qDebug() << "dagestan[" << i << "] = " << dagestan[i];
    }
}

void Delegate::on_pushButton_clicked()
{
    // Получите данные из QTextEdit
    QString text = ui->delegate->toPlainText();
    QStringList lines = text.split("\n");

    // Calculate debts
    double valueFromDelegate = ui->delegate->toPlainText().toDouble(); // total amount from QTextEdit
    double perPerson = valueFromDelegate / 3; // Assuming this is the correct logic for splitting

    // Calculate debts for each person
    double debts[3] = {dagestan[1] - perPerson, dagestan[2] - perPerson, dagestan[3] - perPerson};

    // Флаги, указывающие, участвует ли каждый человек в расчетах

    // Find the maximum and minimum debts
    // Find the maximum and minimum debts
    int maxIndex = 0, minIndex = 0, betwIndex = 0;
    for (int i = 1; i < 3; ++i) {
        if (debts[i] > debts[maxIndex]) {
            maxIndex = i;
        }
        if (debts[i] < debts[minIndex]) {
            minIndex = i;
        }
    }

    // Ensure maxIndex and minIndex are not the same
    if (maxIndex == minIndex) {
        // Если maxIndex и minIndex совпадают, это может привести к некорректному определению betwIndex
        // Вы можете добавить дополнительную логику для обработки этого случая, если это возможно
        // Например, выбрать любой другой индекс
        for (int i = 0; i < 3; ++i) {
            if (i != maxIndex) {
                betwIndex = i;
                break;
            }
        }
    } else {
        // Если maxIndex и minIndex не совпадают, найдите betwIndex
        for (int i = 0; i < 3; ++i) {
            if (i != maxIndex && i != minIndex) {
                betwIndex = i;
                break;
            }
        }
    }


    double maxDebt = debts[maxIndex];
    double minDebt = debts[minIndex];
    double betwDebt = debts[betwIndex];

    // Update QLabel
    ui->label0->setText(QString::number(debts[0])); // Total for 1 person

    // Logic to update labels based on the described logic
    QString result;
    if (maxIndex != -1 && minIndex != -1) {
        if (maxDebt > 0) {
            if (minDebt < 0) {
                double delta = minDebt + maxDebt;
                if (delta == 0) {
                    if (betwDebt>0) {                    result = QString("V1-1 %1 give %4 to %2; %3 give %5 to 0").arg(maxIndex + 1).arg(minIndex + 1).arg(betwIndex + 1).arg(maxDebt).arg(betwDebt);
} else {                    result = QString("V1-2 %1 give %4 to %2; %3 get %5 from 0").arg(maxIndex + 1).arg(minIndex + 1).arg(betwIndex + 1).arg(maxDebt).arg(betwDebt);
}
                } else {
                    if (delta > 0) {
                        if (betwDebt < 0) {
                            if (delta + betwDebt == 0) {
                                result = QString("v2 %1 give %2 to %3; %1 give %4 to %5").arg(maxIndex + 1).arg(-minDebt).arg(minIndex + 1).arg(-betwDebt).arg(betwIndex + 1);
                            } else {
                                if (delta + betwDebt  > 0) {
                                    result = QString("3v %1 give %2 to %3; %1 give %4 to %5; %1 give %6+%4 to 0").arg(maxIndex + 1).arg(-minDebt).arg(minIndex + 1).arg(betwDebt).arg(betwIndex + 1).arg(delta);
                                } else {
                                    result = QString("4v %1 give %2 to %3; %1 give %4 to %5; %5 get %6+%4 from 0").arg(maxIndex + 1).arg(-minDebt).arg(minIndex + 1).arg(delta).arg(betwIndex + 1).arg(betwDebt);
                                }
                            }
                        } else {
                            if (betwDebt == 0) {
                                result = QString("V5 %1 give %2 to %3; %1 give %4 to 0").arg(maxIndex + 1).arg(-minDebt).arg(minIndex + 1).arg(delta);
                            } else {
                                result = QString("V6 %1 give %2 to %3; %1 give %4 to 0; %3; %5 give %6 to 0").arg(maxIndex + 1).arg(-minDebt).arg(minIndex + 1).arg(delta).arg(betwIndex + 1).arg(betwDebt);
                            }
                        }
                    } else {
                        if (betwDebt > 0) {
                            if (delta + betwDebt == 0) {
                                result = QString("V7 %1 give %2 to %3; %4 give %5 to %3").arg(maxIndex + 1).arg(maxDebt).arg(minIndex + 1).arg(betwIndex + 1).arg(betwDebt);
                            } else {
                                if ((delta + betwDebt) < 0) {
                                    result = QString("V8 %1 give %2 to %3; %4 give %6 to %3; %4 get %6+%5 from 0").arg(maxIndex + 1).arg(maxDebt).arg(minIndex + 1).arg(betwIndex + 1).arg(betwDebt).arg(delta);
                                } else {
                                    result = QString("V9 %1 give %2 to %3; %4 give %6 to %3; %4 give %6+%5 to 0").arg(maxIndex + 1).arg(maxDebt).arg(minIndex + 1).arg(betwIndex + 1).arg(betwDebt).arg(delta);
                                }
                            }
                        } else {
                            result = QString("V10 %1 give %2 to %3; %3 get %6+%5 from 0; %4 get %7 from 0").arg(maxIndex + 1).arg(-maxDebt).arg(minIndex + 1).arg(betwIndex + 1).arg(maxDebt).arg(minDebt).arg(betwDebt);
                        }
                    }
                }
            } else {
                result = QString("V11 %1 give %2 to 0; %3 give %4 to 0; %5 give %6 to 0").arg(maxIndex + 1).arg(maxDebt).arg(minIndex + 1).arg(minDebt).arg(betwIndex + 1).arg(betwDebt);
            }
        } else {
            result = QString("V12 %1 get %2 from 0; %3 get %4 from 0; %5 get %6 from 0").arg(maxIndex + 1).arg(maxDebt).arg(minIndex + 1).arg(minDebt).arg(betwIndex + 1).arg(betwDebt);
        }
    } else {
        result = "from 0.";
    }

    ui->label1->setText(result);
    ui->label2->setText(QString::number(debts[1])); // Debt for 2 persons
    ui->label3->setText(QString::number(debts[2])); // Debt for 3 persons
}

// if debt[maxindex]<=0 {

//         settext %maxindex get %debts[maxindex] from 0,%minindex get %debts[minindex] from 0, %betwIndex get %debts[btwindex] from 0;}
//         else {
//         if debts[minindex]<0 {
//                 if delta=debt[min]+debt[max]==0 {
//                         if debts[btw]>0 {
//                                 settext %maxindex give %debts[maxindex] to %minindex , %betwIndex get %debts[btwindex] to 0;
//                             } else {
//                                 settext %maxindex give %debts[maxindex] to %minindex , %betwIndex get %debts[btwindex] from 0;
//                             }
//                     } else {
//                     if delta>0 {
//                             if debts[btw]<0 {
//                                     if debts[btw]+delta==0 {
//                                             settext %maxindex give %debts[minindex] to %minindex , %maxIndex give %debts[btwindex] to  %btwindex;
//                                         }
//                                     else{
//                                         if did[btw]+delta>0 {
//                                                 settext %maxindex give %debts[minindex] to %minindex , %maxIndex give %debts[btwindex] to  %btwindex,%maxindex give %delta+debt[btw] to 0;

//                                             }
//                                         else {
//                                             settext %maxindex give %debts[minindex] to %minindex , %maxIndex give %delta to  %btwindex,%btwindex get %delta+debt[btw] from 0;

//                                         }

//                                     }
//                                 }else {
//                                     settext %maxindex give %debts[minindex] to %minindex , %maxIndex give %delta to  0,%btwindex give %debt[btw] to 0;

//                                 }
//                         } else{
//                         if debt[btw]>0 {
//                                 if delta+debt[btw]=0 {
//                                         settext %maxindex give %debts[minindex] to %minindex ,%btwindex give %debt[btw] to %minindex;

//                                     } else {
//                                     if delta+debt[btw]>0 {
//                                             settext %maxindex give %debts[minindex] to %minindex ,%btwindex give %delta to %minindex,%btwindex give %delta+debt[btw] to 0 ;

//                                         }else {
//                                         settext %maxindex give %debts[minindex] to %minindex ,%btwindex give %delta to %minindex,%btwindex give %delta+debt[btw] from 0 ;

//                                     }
//                                 }
//                             } else {
//                                 settext %maxindex give %debts[minindex] to %minindex ,%btwindex get %debt[btw] from 0, %minindex get %delta from 0 ;

//                             }
//                     }
//                 }
//             } else {
//                 settext %maxindex give %debts[maxindex] to 0 ,%btwindex give %debt[btw] to 0, %minindex give %debt[minindex] to 0 ;

//             }
//         }


