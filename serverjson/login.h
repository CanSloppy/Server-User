#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QTcpSocket>

static QTcpSocket *tcpSocket = new QTcpSocket;

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    static QTcpSocket* getSocket();

private slots:
    void on_pushButtonLogin_clicked();
    void connect_success();
    void read_data();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
