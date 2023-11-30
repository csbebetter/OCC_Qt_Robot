#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/qss/StyleSheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    MainWindow w;
    //w.setWindowFlags(Qt::FramelessWindowHint);  // Òþ²Ø±êÌâÀ¸
    w.show();
    qss.close();
    return a.exec();
}
