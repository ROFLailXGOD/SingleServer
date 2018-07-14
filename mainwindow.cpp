#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "File.h"

#include <QFileDialog>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    arr.reserve(1000);
    LoadData();

    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(Selector()));

    ui->treeWidget->setColumnWidth(0, 200);
}

MainWindow::~MainWindow()
{
    delete ui;
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
        ui->pushButton_3->setEnabled(1);
        ui->pushButton_4->setEnabled(1);
    }
    else
    {
        ui->pushButton->setText("Добавить приложения");
        ui->pushButton_2->setEnabled(0);
        ui->pushButton_3->setEnabled(0);
        ui->pushButton_4->setEnabled(0);
    }
}

void MainWindow::SaveData()
{
    QString path = "data.bin";
    QFile data(path);
    if (data.open(QIODevice::WriteOnly))
    {
        QDataStream out(&data);
        out << arr;
        data.close();
    }
}

void MainWindow::LoadData()
{
    QString path = "data.bin";
    QFile data(path);
    if (data.exists())
    {
        if (data.open(QIODevice::ReadOnly))
        {
            {
                QDataStream in(&data);
                in >> arr;
                data.close();
            }
        }
    }
    QString CurCategory;

    for (int i = 0; i < arr.size(); ++i)
    {
        QTreeWidgetItem *item;
        if (CurCategory != arr.at(i)->GetCategory())
        {
            CurCategory = arr.at(i)->GetCategory();
            item = new QTreeWidgetItem();
            item->setText(0, CurCategory);
            ui->treeWidget->addTopLevelItem(item);
            item->setExpanded(1);
        }
        QTreeWidgetItem *itm = new QTreeWidgetItem(item); // Creating leaves
        QString name = arr.at(i)->GetName();
        itm->setText(0, name);
        if (!arr.at(i)->GetCFolder().isEmpty())
        {
            itm->setText(1, arr.at(i)->GetCFolder());
            if (arr.at(i)->isSynched())
            {
                itm->setTextColor(1,Qt::darkGreen);
                itm->setToolTip(1, "Файл синхронизируется");
            }
            else
            {
                itm->setTextColor(1,Qt::darkYellow);
                itm->setToolTip(1, "Синхронизация приостановлена");
            }
        }
        else
        {
            itm->setText(1, "<Путь до облака отсутствует>");
            itm->setTextColor(1,Qt::darkRed);
            itm->setToolTip(1, "Добавьте Облачную папку");
        }
        QVariant qvFile = QVariant::fromValue(arr.at(i));
        itm->setData(0,Qt::UserRole+1,qvFile);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    SaveData();
    event->accept();
}

void MainWindow::on_pushButton_clicked()
{
    SaveData();

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
                QTreeWidgetItem *item = new QTreeWidgetItem(); // Creating Roots
                item->setText(0, name);

                if (!CatHash.contains(name))
                {
                    CatHash.insert(name, 0);
                }

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
                File *file = new File(pathes.at(i));
                file->SetCategory(selectedItems.at(0)->text(0));

                int amount;
                int position = -1;
                amount = CatHash.value(selectedItems.at(0)->text(0));
                CatHash.insert(selectedItems.at(0)->text(0), amount+1);
                QHash<QString, int>::const_iterator j = CatHash.constBegin();
                while (j != CatHash.constFind(selectedItems.at(0)->text(0)))
                {
                    position += j.value();
                    ++j;
                }
                position += j.value();

                arr.insert(position, file);
                QTreeWidgetItem *item = new QTreeWidgetItem(); // Creating leaves
                QString name = file->GetName();
                item->setText(0, name);
                item->setText(1, "<Путь до облака отсутствует>");
                QVariant qvFile = QVariant::fromValue(file);
                item->setData(0,Qt::UserRole+1,qvFile);
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
                File *file = itemData.value<File*>();
                file->SetCloudFolder(dir);
                file->SetSynch(1);
                selectedItems[i]->setText(1, dir);
                selectedItems[i]->setTextColor(1, Qt::darkGreen);
                selectedItems[i]->setToolTip(1, "Файл синхронизируется");
            }
            else // Parent
            {
                for (int j = 0; j < selectedItems.at(i)->childCount(); ++j)
                {
                    QVariant itemData = selectedItems.at(i)->child(j)->data(0, Qt::UserRole+1);
                    File *file = itemData.value<File*>();
                    file->SetCloudFolder(dir);
                    file->SetSynch(1);
                    selectedItems.at(i)->child(j)->setText(1, dir);
                    selectedItems.at(i)->child(j)->setTextColor(1, Qt::darkGreen);
                    selectedItems.at(i)->child(j)->setToolTip(1, "Файл синхронизируется");
                }
            }
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "", "Вы действительно хотите удалить выделенные файлы?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
        for (int i = 0; i < selectedItems.size(); ++i)
        {
            if (selectedItems.at(i)->parent()) // Child
            {
                QVariant itemData = selectedItems.at(i)->data(0, Qt::UserRole+1);
                File *file = itemData.value<File*>();
                arr.removeOne(file);
                delete file;
                delete selectedItems[i];
            }
            else // Parent
            {
                for (int j = 0; j < selectedItems.at(i)->childCount(); ++j)
                {
                    QVariant itemData = selectedItems.at(i)->child(j)->data(0, Qt::UserRole+1);
                    File *file = itemData.value<File*>();
                    arr.removeOne(file);
                    delete file;
                }
                delete selectedItems[i];
            }
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    for (int i = 0; i < selectedItems.size(); ++i)
    {
        if (selectedItems.at(i)->parent()) // Child
        {
            QVariant itemData = selectedItems.at(i)->data(0, Qt::UserRole+1);
            File *file = itemData.value<File*>();
            if (selectedItems.at(i)->textColor(1) != Qt::darkRed)
            {
                if (file->isSynched())
                {
                    file->SetSynch(0);
                    selectedItems[i]->setTextColor(1, Qt::darkYellow);
                    selectedItems[i]->setToolTip(1, "Синхронизация приостановлена");
                }
                else
                {
                    file->SetSynch(1);
                    selectedItems[i]->setTextColor(1, Qt::darkGreen);
                    selectedItems[i]->setToolTip(1, "Файл синхронизируется");
                }
            }
        }
        else // Parent
        {
            for (int j = 0; j < selectedItems.at(i)->childCount(); ++j)
            {
                QVariant itemData = selectedItems.at(i)->child(j)->data(0, Qt::UserRole+1);
                File *file = itemData.value<File*>();
                if (selectedItems.at(i)->child(j)->textColor(1) != Qt::darkRed)
                {
                    if (file->isSynched())
                    {
                        file->SetSynch(0);
                        selectedItems.at(i)->child(j)->setTextColor(1, Qt::darkYellow);
                        selectedItems.at(i)->child(j)->setToolTip(1, "Синхронизация приостановлена");
                    }
                    else
                    {
                        file->SetSynch(1);
                        selectedItems.at(i)->child(j)->setTextColor(1, Qt::darkGreen);
                        selectedItems.at(i)->child(j)->setToolTip(1, "Файл синхронизируется");
                    }
                }
            }
        }
    }
}
