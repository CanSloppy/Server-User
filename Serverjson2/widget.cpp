#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);
    tcpServer = new QTcpServer;
    tcpServer->listen(QHostAddress("127.0.0.1"),1234);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(new_connect()));
    setWindowTitle("服务器");
    initSql();
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::checkFromSql()
{
    db.open();
    QSqlQuery query;
    QString s=QString("select * from User where username = '%1' AND passsword = '%2';")
            .arg(user->getUsername()).arg(user->getPwd());
    //select * from User where username = 'cancan' AND passsword = '123';
    query.exec(s);
    if(query.first()){
        ui->plainTextEdit->appendPlainText("数据库中查询到此用户");
        db.close();
        return true;
    }
    else
    {
        db.close();
        ui->plainTextEdit->appendPlainText("数据库中无此用户或密码错误");
        return false;
    }
}

void Widget::initSql()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("CanSloppy");
    db.setUserName("root");
    db.setPassword("211335");
}

bool Widget::saveJson(const QString &str)
{
    QFile file("../Serverjson2/json/" + user->getJsonName());
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        ui->plainTextEdit->appendPlainText("打开文件失败");
        return false;
    }
    QStringList list = str.split(" ");
    QTextStream stream(&file);
    stream << "{\n"
           << "\t\"key1\" : \"" << list[1].split(":")[1] << "\"\n"
           << "\t\"key2\" : \"" << list[2].split(":")[1] << "\"\n"
           << "\t\"config1\" : \"" << list[3].split(":")[1] << "\"\n"
           << "\t\"config2\" : \"" << list[4].split(":")[1] << "\"\n"
           << "\t\"config3\" : \"" << list[5].split(":")[1] << "\"\n"
           << "}";
    file.close();
}

bool Widget::loadJson(int i)
{
    QFile file("../Serverjson2/json/" + user->getJsonName());
    if(!file.open(QIODevice::ReadOnly))
    {
        ui->plainTextEdit->appendPlainText("打开文件失败");
        return false;
    }
    QTextStream stream(&file);
    QString ret = file.readAll();
    ui->plainTextEdit->appendPlainText(ret);
    socketArr[i]->write(ret.toStdString().c_str(),ret.size()+1);
    file.close();
}

void Widget::new_connect()
{
    ui->plainTextEdit->appendPlainText("有新的客户端连接");
    QTcpSocket* tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(read_data()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnect_success()));
    socketArr.push_back(tcpSocket);
}

void Widget::read_data()
{
    for(int i=0; i<socketArr.size(); i++)
    {
        if(socketArr[i]->bytesAvailable())
        {
            char buf[256] = {};
            socketArr[i]->read(buf,sizeof(buf));
            QString str = buf;
            if(str.split(":")[0] == "Username")
            {
                str = str.split(":")[1];
                user = new User(str,str + ".json");
                ui->plainTextEdit->appendPlainText("用户名:" + str +" 已登录");
            }
            else if(str.split(":")[0] == "Password")
            {
                str = str.split(":")[1];
                user->setPwd(str);
                ui->plainTextEdit->appendPlainText("密码:" + str);
                bool ret = checkFromSql();
                if(ret) socketArr[i]->write("true",5);
                else socketArr[i]->write("false",6);
            }
            else if(str.split(":")[0] == "Load")
            {
                ui->plainTextEdit->appendPlainText("客户端请求Load");
                ui->plainTextEdit->appendPlainText(str);
                if(loadJson(i))
                    ui->plainTextEdit->appendPlainText("读取json文件成功");
                else
                    ui->plainTextEdit->appendPlainText("读取json文件失败");
            }
            else if(str.split(":")[0] == "Save")
            {
                ui->plainTextEdit->appendPlainText("客户端请求Save");
                ui->plainTextEdit->appendPlainText(str);
                if(saveJson(str))
                    ui->plainTextEdit->appendPlainText("保存json文件成功");
                else
                    ui->plainTextEdit->appendPlainText("保存json文件失败");
            }
        }
    }
}

void Widget::disconnect_success()
{
    ui->plainTextEdit->appendPlainText("客户端断开连接");
}
