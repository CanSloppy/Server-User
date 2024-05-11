#include "user.h"

User::User()
{

}

User::User(QString username,QString jsonName)
{
   m_username = username;
   m_jsonName = jsonName;
}

void User::setPwd(QString pwd)
{
    m_pwd = pwd;
}

QString User::getUsername()
{
    return m_username;
}

QString User::getPwd()
{
    return m_pwd;
}

QString User::getJsonName()
{
    return m_jsonName;
}
