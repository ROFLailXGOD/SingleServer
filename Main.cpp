#include "mainwindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    return a.exec();
}
