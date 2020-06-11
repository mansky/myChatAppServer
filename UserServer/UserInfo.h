#ifndef USERINFO_H
#define USERINFO_H
#include <QList>
#include <QMutex>
#include <QSemaphore>
#include<qstring.h>


class UserInfo
{
public:
    int QQnum=0;
    bool haveLogin;
//    QMutex objectMutex;
//    QMutex taskMutex;
//    int taskNum;
//    QList<QString> taskList;

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

