#include "UserServer.h"
#include "database/MySqlPool.h"
#include <QDebug>
#include "UserServer/UserInfo.h"

UserServer::UserServer(){

}
UserServer::~UserServer(){

}

int UserServer::addUser(QString nickname,QString password){
    int QQnum=0;
   // QString sqlWords="INSERT INTO `myqq`.`users` (`nickname`, `password`, `salt`) VALUES ('"+nickname+"','"+password+"', '0');";
    QSqlDatabase db=ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.prepare("INSERT INTO `myqq`.`users` (`nickname`, `password`, `salt`) VALUES (?,?, '0');");
    query.bindValue(0,nickname);
    query.bindValue(1,password);
    query.exec();
   // query.exec(sqlWords);//插入新的账户
    QString sqlWords="SELECT  LAST_INSERT_ID();";
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
QByteArray UserServer::handle(QByteArray data){

    if(data.indexOf("register")>=0){
        int i=data.indexOf("nickname")+9;
        int r=data.indexOf(",",i);
        QString name=data.mid(i,r-i);
        i=data.indexOf("password")+9;
        r=data.indexOf(",",i);
        QString password=data.mid(i,r-i);
        int result=addUser(name,password);
        return QByteArray::number(result);
    }else if(data.indexOf("login")>=0){
        int i=data.indexOf("QQnum")+6;
        int r=data.indexOf(",",i);
        int QQnum=data.mid(i,r-i).toInt();
        i=data.indexOf("password")+9;
        r=data.indexOf(",",i);
        QString password=data.mid(i,r-i);
        int result=login(QQnum,password);
        if(result>0){
            this->userInfo->QQnum=QQnum;
            this->userInfo->haveLogin=true;
            return "success";
        }else{
            return "false";
        }

    }
    //if(!this->userInfo->haveLogin)return "noAuthority";//  已登录才获得执行下面的服务

    if(data.indexOf("getfList")>=0){
         QVector<FriendList>result= getFriendLists(this->userInfo->QQnum);
         QByteArray myFriend;
         for(FriendList f:result){
            myFriend.append("order:"+QByteArray::number(f.order)+",description:"+f.description+",");
            for(Friend mf:f.friends){
                myFriend.append(QByteArray::number(mf.mid)+",");
            }
            myFriend.append("*\r\n");
         }
        return myFriend;
     }else if(data.indexOf("getMsg")>=0){

        return "ok";
    }else if(data.indexOf("Msg")>=0){
        int i=data.indexOf("to")+3;
        int r=data.indexOf(",",i);
        int des=data.mid(i,r-i).toInt();
        i=data.indexOf("msgID")+6;
        r=data.indexOf(",",i);
        int msgId=data.mid(i,r-i).toInt();
        i=data.indexOf("text")+5;
        r=data.indexOf("*",i);
        QByteArray sendData=data.mid(i,r-i);
        return "ok";
    }

    return "noCmd";

}

