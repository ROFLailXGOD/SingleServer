#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <vector>
#include "Application.h"
#include "File.h"
#include "BackUper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
