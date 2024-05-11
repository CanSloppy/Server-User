#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QMessageBox>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle("登录");
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connect_success()));
    tcpSocket->connectToHost("127.0.0.1",1234);
    if(!tcpSocket->waitForConnected(1000))
    {
        QMessageBox::information(this,"提示","服务器连接失败","Ok");
    }

}

login::~login()
{
    delete ui;
}

QTcpSocket *login::getSocket()
{
    return tcpSocket;
}

void login::on_pushButtonLogin_clicked()
{
    if(ui->lineEditUserName->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","用户名为空","Ok");
        return;
    }
    if(ui->lineEditPwd->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","密码为空","Ok");
    }
    QString username = "Username:" + ui->lineEditUserName->text();
    QString password = "Password:" + ui->lineEditPwd->text();
    tcpSocket->write(username.toStdString().c_str(),
                     username.size()+1);
    tcpSocket->waitForBytesWritten(500);
    tcpSocket->write(password.toStdString().c_str(),
                     password.size()+1);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(read_data()));
}

void login::connect_success()
{
    QMessageBox::information(this,"提示","服务器连接成功","Ok");
    ui->pushButtonLogin->setEnabled(true);
}

void login::read_data()
{
    if(tcpSocket->bytesAvailable())
    {
        char buf[256] = {};
        tcpSocket->read(buf,sizeof(buf));
        QString res = buf;
        if(res == "true")
        {
            disconnect(tcpSocket,SIGNAL(readyRead()),this,SLOT(read_data()));
            this->close();
            MainWindow *w = new MainWindow;
            w->show();
        }
        else
        {
            QMessageBox::critical(this,"错误","用户名或密码错误","Ok");
        }
    }
}


