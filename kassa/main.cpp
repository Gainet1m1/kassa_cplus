#include "mainwindow.h"
#include <QApplication>
#include <QTimer>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Kassa");
    app.setOrganizationName("Kassa");
    app.setStyle("Fusion");
    app.setStyleSheet(QStringLiteral(
        "QWidget { font-size: 14px; } QMainWindow, QDialog { background: #f3f5f9; }"
        "QPushButton { padding: 10px 16px; border: 1px solid #c9d1dd; border-radius: 6px; "
        "background: #ffffff; color: #17253b; }"
        "QPushButton:hover { background: #e1ebff; } QPushButton:disabled { color: #7b8494; }"
        "QLineEdit, QTextEdit, QTableView { background: white; color: #17253b; border: 1px solid "
        "#c9d1dd; padding: 5px; }"
        "QLabel { color: #17253b; } QLabel#title, QLabel#Sum { font-size: 20px; font-weight: bold; "
        "}"
        "QLabel#errorLabel { color: #b42318; }"));
    MainWindow window;
    window.show();
    if (app.arguments().contains("--smoke-test"))
        QTimer::singleShot(500, &app, &QApplication::quit);
    return app.exec();
}
