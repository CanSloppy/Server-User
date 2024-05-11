#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool checkFromSql();
    void initSql();
    bool saveJson(const QString &str);
    bool loadJson(int i);

private slots:
    void new_connect();
    void read_data();
    void disconnect_success();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer;
    QList<QTcpSocket *> socketArr;
    User *user;
    QSqlDatabase db;
};
#endif // WIDGET_H
