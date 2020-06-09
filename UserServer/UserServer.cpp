#include "UserServer.h"
#include "database/MySqlPool.h"
#include <QDebug>

UserServer::UserServer(){

}
UserServer::~UserServer(){

}

int UserServer::addUser(QString nickname,QString password){
    int QQnum=0;
    QString sqlWords="INSERT INTO `myqq`.`users` (`nickname`, `password`, `salt`) VALUES ('"+nickname+"','"+password+"', '0');";
    QSqlDatabase db=ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(sqlWords);//插入新的账户
    sqlWords="SELECT  LAST_INSERT_ID();";
    query.exec(sqlWords);//查询插入的ID，LAST_INSERT_ID()与connect相关。
    query.seek(0);
    QQnum=query.value(0).toInt();
    ConnectionPool::closeConnection(db);
    return QQnum;
}
int UserServer::login(int QQnum,QString password){
    int result=1;
    QSqlDatabase db=ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.prepare("SELECT  * FROM users WHERE uid = ?");
    query.bindValue(0,QQnum);
    query.exec();
    query.last();
    QString innerPassword=query.value(2).toString();
    if(innerPassword.size()==0){//账户没有注册
        result= -1;
    }else if(innerPassword.compare(password)!=0){//密码不相等
        result= 0;
    }
    ConnectionPool::closeConnection(db);
    return result;

}

 QVector<FriendList>  & UserServer::getFriendLists(int QQnum){
    QVector<FriendList> *friendLists=new QVector<FriendList>();
    QSqlDatabase db=ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.prepare("SELECT  * FROM friendlist WHERE uid = ?");
    query.bindValue(0,QQnum);
    query.exec();
    while(query.next()){
        FriendList friendList;
        friendList.uid=QQnum;
        friendList.order=query.value(1).toInt();
        friendList.description=query.value(2).toString();
        friendLists->append(friendList);
    }

    ConnectionPool::closeConnection(db);
    return *friendLists;
}

int UserServer::getMessage(int QQnum,QString message){


}
int UserServer::SendMessage(QString message){
}
int UserServer::SendImg(int QQnum,QString message){

}
