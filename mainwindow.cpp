#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <string>
#include "Application.h"
#include "File.h"
#include "BackUper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arr.reserve(100);
    Application App(L"Terraria.exe");
    BackUper File(App, L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld");
    (File.GetFile()).SetCloudFolder(L"C:\\Users\\Zabey\\OneDrive\\Terraria");
    arr.push_back(File);

    QTimer *mainTimer = new QTimer(this);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(MainLoop()));
    mainTimer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MainLoop()
{
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i].bHasLinkedApp)
        {
            if (!arr[i].GetApp().IsRunning()) // Making sure that App isn't running
            {
                (arr[i].GetFile()).UpdateFile();
            }
        }
        else
        {
            (arr[i].GetFile()).UpdateFile();
        }
    }
}
