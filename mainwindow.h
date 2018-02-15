#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BackUper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<BackUper*> arr;

private slots:
    void MainLoop();
    void Selector();
    void SaveData();
    void LoadData();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    void closeEvent(QCloseEvent *event) override;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
