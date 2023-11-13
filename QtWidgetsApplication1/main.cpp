#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont newFont("Microsoft YaHei",10);
    a.setFont(newFont);
    MainWindow w;
    w.show();

    return a.exec();
}
