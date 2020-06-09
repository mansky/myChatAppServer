#ifndef USERINFO_H
#define USERINFO_H
#include <QList>
#include <QMutex>
#include <QSemaphore>
#include<qstring.h>


class UserInfo
{
public:
    int QQnum;

    QMutex objectMutex;
    QMutex taskMutex;
    int taskNum;
    QList<QString> taskList;

    QString getNickname() const;
    void setNickname(const QString &value);

    int getPassword() const;
    void setPassword(int value);

    int getSalt() const;
    void setSalt(int value);

private:
    QString nickname;
    int password;
    int salt;

};














#endif // USERINFO_H

QString UserInfo::getNickname() const
{
return nickname;
}

void UserInfo::setNickname(const QString &value)
{
nickname = value;
}

int UserInfo::getPassword() const
{
return password;
}

void UserInfo::setPassword(int value)
{
password = value;
}

int UserInfo::getSalt() const
{
return salt;
}

void UserInfo::setSalt(int value)
{
salt = value;
}
