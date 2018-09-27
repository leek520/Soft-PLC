#include "mainwindow.h"
#include <QApplication>
//http://jz.docin.com/p-313859620.html
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
