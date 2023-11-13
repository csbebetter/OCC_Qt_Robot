#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss("StyleSheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    MainWindow w;
    w.show();
    qss.close();
    return a.exec();
}
