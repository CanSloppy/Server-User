#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSave_triggered()
{
    QString c1 = ui->actionConfig->isChecked()?"true":"false";
    QString c2 = ui->actionconfig2->isChecked()?"true":"false";
    QString c3 = ui->actionConfig3->isChecked()?"true":"false";
    QStringList list = {{"Save:"},
                        {"key1:" + ui->lineEditK1->text()},
                        {"key2:" + ui->lineEditK2->text()},
                        {"config1:" + c1},
                        {"config2:" + c2},
                        {"config3:" + c3}};
    QString ret;
    for(auto i:list)
    {
        ret += i + ' ';
    }
    login::getSocket()->write(ret.toStdString().c_str(),ret.size()+1);
}

void MainWindow::on_actionLoad_triggered()
{
    connect(login::getSocket(),SIGNAL(readyRead()),this,SLOT(read_data()));
    login::getSocket()->write("Load:",6);
}

void MainWindow::read_data()
{
    if(login::getSocket()->bytesAvailable())
    {
        char buf[256] = {};
        login::getSocket()->read(buf,sizeof(buf));
        QString res = buf;
        QStringList list = res.split(":");
        ui->lineEditK1->setText(list[1].split("\"")[1]);
        ui->lineEditK2->setText(list[2].split("\"")[1]);
        if(list[3].split("\"")[1] == "true")
        {
            ui->actionConfig->setChecked(true);
        }
        if(list[4].split("\"")[1] == "true")
        {
            ui->actionconfig2->setChecked(true);
        }
        if(list[5].split("\"")[1] == "true")
        {
            ui->actionConfig3->setChecked(true);
        }
    }
}
