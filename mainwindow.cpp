#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Application.h"
#include "File.h"
#include "BackUper.h"

#include <QTimer>
#include <QFileDialog>
#include <QVariant>
#include <QDebug>

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

    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(Selector()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MainLoop()
{
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr.at(i).bHasLinkedApp)
        {
            if (!arr.at(i).GetApp().IsRunning()) // Making sure that App isn't running
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

void MainWindow::Selector()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        if (selectedItems.at(0)->parent()) // Select only children
        {
            for (int i = 1; i < selectedItems.size(); ++i)
            {
                if (!selectedItems.at(i)->parent())
                {
                    selectedItems.at(i)->setSelected(0);
                }
            }
        }
        else // Select all
        {
            for (int i = 0; i < selectedItems.size(); ++i)
            {
                for (int j = 0; j < selectedItems.at(i)->childCount(); ++j)
                {
                    selectedItems.at(i)->child(j)->setSelected(1);
                }
            }
        }
    }
    else
    {
        ui->pushButton->setText("Добавить приложения");
    }
}

void MainWindow::on_pushButton_clicked()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    qInfo() << selectedItems.size();
    if (selectedItems.size() == 0)
    {
        QStringList pathes = QFileDialog::getOpenFileNames(
                    this,
                    tr("Выберите одно или несколько приложений"),
                    ".",
                    "Исполняемые файлы (*.exe)"
                    );
        for (int i = 0; i < pathes.size(); ++i)
        {
            if (!pathes.at(i).isEmpty())
            {
                QFileInfo fileinfo(pathes.at(i));
                QString name = fileinfo.fileName();
                Application App(name.toStdWString());
                QTreeWidgetItem *item = new QTreeWidgetItem(); // Creating Roots
                item->setText(0, name);
                QVariant RootApp = QVariant::fromValue(App);
                item->setData(0,Qt::UserRole+1,RootApp);
                ui->treeWidget->addTopLevelItem(item);
            }
        }
    }
}
