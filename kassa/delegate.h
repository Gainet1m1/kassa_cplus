#ifndef DELEGATE_H
#define DELEGATE_H

#include <QDialog>
#include <QStandardItemModel>
#include "ui_delegate.h"

namespace Ui {
class Delegate;
}

class Delegate : public QDialog
{
    Q_OBJECT

public:
    explicit Delegate(QWidget *parent = nullptr);
    ~Delegate();

    void updateTableView(const QMap<int, double>& data); // Измените сигнатуру метода

private slots:
    void on_pushButton_clicked(); // Добавьте этот слот

private:
    Ui::Delegate *ui;
    QStandardItemModel *model; // Добавьте этот член
};

#endif // DELEGATE_H
