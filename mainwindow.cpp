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

    ui->treeWidget->setColumnWidth(0, 200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MainLoop()
{
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr.at(i)->bHasLinkedApp)
        {
            if (!arr.at(i)->GetApp().IsRunning()) // Making sure that App isn't running
            {
//                (arr[i].GetFile()).UpdateFile();
            }
        }
        else
        {
//            (arr[i].GetFile()).UpdateFile();
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
            ui->pushButton->setText("Добавить приложения");
        }
        else // Select only parents
        {
            for (int i = 1; i < selectedItems.size(); ++i)
            {
                if (selectedItems.at(i)->parent())
                {
                    selectedItems.at(i)->setSelected(0);
                }
            }
            if (selectedItems.size() == 1)
            {
                ui->pushButton->setText("Добавить файлы");
            }
            else
            {
                ui->pushButton->setText("Добавить приложения");
            }
        }
        ui->pushButton_2->setEnabled(1);
    }
    else
    {
        ui->pushButton->setText("Добавить приложения");
        ui->pushButton_2->setEnabled(0);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (ui->pushButton->text() == "Добавить приложения")
    {
        QStringList pathes = QFileDialog::getOpenFileNames(
                    this,
                    tr("Выберите одно или несколько приложений"),
                    "/",
                    "Исполняемые файлы (*.exe)"
                    );
        for (int i = 0; i < pathes.size(); ++i)
        {
            if (!pathes.at(i).isEmpty())
            {
                QFileInfo fileinfo(pathes.at(i));
                QString name = fileinfo.fileName();
                Application *App = new Application(name.toStdWString());
                QTreeWidgetItem *item = new QTreeWidgetItem(); // Creating Roots
                item->setText(0, name);
                QVariant RootApp = QVariant::fromValue(App);


                item->setData(0,Qt::UserRole+1,RootApp);
                ui->treeWidget->addTopLevelItem(item);
            }
        }
    }
    else
    {
        QStringList pathes = QFileDialog::getOpenFileNames(
                    this,
                    tr("Выберите один или несколько файлов"),
                    "/",
                    "Любые файлы (*.*)"
                    );
        for (int i = 0; i < pathes.size(); ++i)
        {
            if (!pathes.at(i).isEmpty())
            {
                QVariant itemData = selectedItems.at(0)->data(0, Qt::UserRole + 1);
                Application *App = itemData.value<Application*>();
                BackUper *Bcpr = new BackUper(App, pathes.at(i).toStdWString());
                arr.push_back(Bcpr);
                QTreeWidgetItem *item = new QTreeWidgetItem(); // Creating leaves
                QString name = QString::fromWCharArray(Bcpr->GetFile().GetName().c_str());
                item->setText(0, name);
                item->setText(1, "<Путь до облака отсутствует>");
                QVariant qvBcpr = QVariant::fromValue(Bcpr);
                item->setData(0,Qt::UserRole+1,qvBcpr);
                item->setTextColor(1,Qt::darkRed);
                item->setToolTip(1, "Добавьте Облачную папку");
                selectedItems.at(0)->addChild(item);
                selectedItems.at(0)->setExpanded(1);
            }
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите папку"),
                                                 "/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        for (int i = 0; i < selectedItems.size(); ++i)
        {
            if (selectedItems.at(i)->parent()) // Child
            {
                QVariant itemData = selectedItems.at(i)->data(0, Qt::UserRole+1);
                BackUper *Bcpr = itemData.value<BackUper*>();
                Bcpr->GetFile().SetCloudFolder(dir.toStdWString());
                selectedItems[i]->setText(1, dir);
                selectedItems[i]->setTextColor(1, Qt::darkGreen);
                selectedItems[i]->setToolTip(1, "Файл синхронизируется");
            }
            else // Parent
            {
                for (int j = 0; j < selectedItems.at(i)->childCount(); ++j)
                {
                    QVariant itemData = selectedItems.at(i)->child(j)->data(0, Qt::UserRole+1);
                    BackUper *Bcpr = itemData.value<BackUper*>();
                    Bcpr->GetFile().SetCloudFolder(dir.toStdWString());
                    selectedItems.at(i)->child(j)->setText(1, dir);
                    selectedItems.at(i)->child(j)->setTextColor(1, Qt::darkGreen);
                    selectedItems.at(i)->child(j)->setToolTip(1, "Файл синхронизируется");
                }
            }
        }
    }
}
