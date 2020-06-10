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
    //QHash<int,FriendList *> friendToList;

    //获取好友列表
    query.prepare("SELECT  * FROM friendlist WHERE uid = ?");
    query.bindValue(0,QQnum);
    query.exec();
    while(query.next()){
        FriendList friendList ;
        friendList.uid=QQnum;
        friendList.order=query.value(1).toInt();
        friendList.description=query.value(2).toString();
        friendLists->append(friendList);
    }
//    for (auto iter=friendLists->begin();iter!=friendLists->end();iter++)
//    {
//        friendToList.insert(iter->order,iter);//小心VECTOR自动扩容导致指针失效。
//    }
    //获取好友
    query.prepare("SELECT  * FROM friends WHERE uid = ?");
    query.bindValue(0,QQnum);
    query.exec();//获取好友列表
    while(query.next()){
         Friend myfriend;
         myfriend.uid=QQnum;
         myfriend.mid=query.value(1).toInt();
         myfriend.FriendOrder=query.value(2).toInt();
         //friendToList.take(myfriend.FriendOrder)->friends.append(myfriend);//想过使用HashMap加速查找，但是总是报段错误，在插入第二个myfriend会报错。
         for (auto iter=friendLists->begin();iter!=friendLists->end();iter++){
             if(iter->order==myfriend.FriendOrder)iter->friends.append(myfriend);//根据order找到对应friendList，然后插入friend。
         }
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
int handle(QByteArray data){

}
