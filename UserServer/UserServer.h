#ifndef USERSERVER_H
#define USERSERVER_H

#include <qstring.h>
#include "FriendList.h"

class UserServer
{
public:
    UserServer();
    ~UserServer();
    int addUser(QString nickname,QString password);
    int login(int QQnum,QString password);
    QVector<FriendList> & getFriendLists(int QQnum);
    int updateFriendList(int QQnum,int friendQQnum,int FriendList);
    int addFriend(int QQnum,int friendQQnum);
    int getMessage(int QQnum,QString message);
    int SendMessage(QString message);
    int SendImg(int QQnum,QString message);
private:
};

#endif // USERSERVER_H
