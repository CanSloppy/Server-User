#ifndef USER_H
#define USER_H

#include<QObject>

class User :public QObject
{
    Q_OBJECT
public:
    User();
    User(QString username,QString jsonName);
    void setPwd(QString pwd);
    QString getUsername();
    QString getPwd();
    QString getJsonName();


private:
    QString m_username;
    QString m_pwd;
    QString m_jsonName;
};

#endif // USER_H
