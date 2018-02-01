#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Application.h"
#include "File.h"
#include "BackUper.h"

#include <QTimer>
#include <QFileDialog>

#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arr.reserve(100);

    QTimer *mainTimer = new QTimer(this);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(MainLoop()));
    mainTimer->start(15000);
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

void MainWindow::on_pushButton_clicked()
{
    QStringList pathes = QFileDialog::getOpenFileNames(
                this,
                tr("Выберите приложение"),
                ".",
                "Executables (*.exe)"
                );
    for (int i = 0; i < pathes.size(); ++i)
    {
        if (!pathes.at(i).isEmpty())
        {
            QFileInfo fileinfo(pathes.at(i));
            QString name = fileinfo.fileName();
            Application App(name.toStdWString());
            BackUper File(App, L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld");
            (File.GetFile()).SetCloudFolder(L"C:\\Users\\Zabey\\OneDrive\\Terraria");
            arr.push_back(File);

            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, name);
            ui->treeWidget->addTopLevelItem(item);
        }
    }
}
