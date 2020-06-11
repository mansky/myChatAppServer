#ifndef USERSERVER_H
#define USERSERVER_H

#include <QObject>
#include <qstring.h>
#include "FriendList.h"
#include "UserServer/UserInfo.h"
#include <QtSql>

class UserServer:public QObject
{
     Q_OBJECT
public:
    UserServer(QObject *parent=0);
    ~UserServer();
    int addUser(QString nickname,QString password);
    int login(int QQnum,QString password);
    QVector<FriendList*> & getFriendLists(int QQnum);
    int updateFriendList(int QQnum,int friendQQnum,int FriendList);
    int addFriend(int QQnum,int friendQQnum);
    int getMessage(int QQnum,QString message);
    int SendMessage(QString message);
    int SendImg(int QQnum,QString message);
    QByteArray handle(QByteArray data);
    UserInfo *userInfo;
signals:
    void sendToOtherQQ(const QByteArray & data,int QQnum);
    void startRegisterToServer();
private:;
    QSqlDatabase db;

};

#endif // USERSERVER_H
