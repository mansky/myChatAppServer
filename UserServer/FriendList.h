#ifndef FRIENDLIST_H
#define FRIENDLIST_H
#include <QVector>
#include <QString>
class Friend{
public:
    int uid;//归属的账户的ID
    int mid;//朋友的账户的ID
    int FriendOrder;
};

class FriendList
{
public:
    int uid;//归属的账户的ID
    int order;//序号
    QString description;
    QVector<Friend> friends;

};

#endif // FRIENDLIST_H
