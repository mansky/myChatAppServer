#include "UserServer.h"
#include <QDebug>
#include "UserServer/UserInfo.h"
#include "database/MySqlPool.h"

UserServer::UserServer(QObject *parent){
    db=(ConnectionPool::openConnection());//很遗憾，QT5.11之后的版本数据库连接只能在本线程使用。




}
UserServer::~UserServer(){   
    db.close();
    QString name;
    {
        name = db.connectionName();
    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
    //QSqlDatabase::removeDatabase("name");//QT的数据库删除是大坑,QSqlDatabasePrivate::removeDatabase: connection '12s021' is still in use, all queries will cease to work.
    //    {
    //        QSqlDatabase dba = QSqlDatabase::addDatabase("QMYSQL", "12s021");
    //        dba.setHostName("localhost");
    //        dba.setDatabaseName("myqq");
    //        dba.setUserName("root");
    //        dba.setPassword("123456");
    //        dba.close();
    //    }
    //    QSqlDatabase::removeDatabase("12s021");//官方推荐的删除方式，通过作用域自动删除QSqlDatabase对象。

}

int UserServer::addUser(QString nickname,QString password){
    int QQnum=0;
    db.setPassword(password);
    QSqlQuery query(db);
    query.prepare("INSERT INTO `myqq`.`users` (`nickname`, `password`, `salt`) VALUES (?,?, '0');");
    query.bindValue(0,nickname);
    query.bindValue(1,password);
    query.exec();

    QString sqlWords="SELECT  LAST_INSERT_ID();";
    query.exec(sqlWords);//查询插入的ID，LAST_INSERT_ID()与connect相关。
    query.seek(0);
    QQnum=query.value(0).toInt();
    ConnectionPool::closeConnection(db);
    return QQnum;
}
int UserServer::login(int QQnum,QString password){
    int result=1;
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

 QVector<FriendList *>  & UserServer::getFriendLists(int QQnum){
    QVector<FriendList*> friendLists;

    QSqlQuery query(db);
    QHash<int,int> friendToList;

    //获取好友列表
    query.prepare("SELECT  * FROM friendlist WHERE uid = ?");
    query.bindValue(0,QQnum);
    query.exec();
    while(query.next()){
        FriendList *friendList =new FriendList();
        friendList->uid=QQnum;
        friendList->order=query.value(1).toInt();
        friendList->description=query.value(2).toString();

        friendToList.insert(friendList->order,friendToList.size());//将要插入friendList的order和它在friendLists的index关联起来。
        friendLists.append(friendList);
    }

    //获取好友
    query.prepare("SELECT  * FROM friends WHERE uid = ?");
    query.bindValue(0,QQnum);
    query.exec();//获取好友列表
    while(query.next()){
         Friend *myfriend=new Friend();
         myfriend->uid=QQnum;
         myfriend->mid=query.value(1).toInt();
         myfriend->FriendOrder=query.value(2).toInt();
         //friendToList.take(myfriend.FriendOrder)->friends.append(myfriend);//想过使用HashMap加速查找，但是总是报段错误，在插入第二个myfriend会报错。
         friendLists.at(friendToList.take(myfriend->FriendOrder))->friends.append(*myfriend);

//         for (auto iter=friendLists->begin();iter!=friendLists->end();iter++){
//             if(iter->order==myfriend.FriendOrder)iter->friends.append(myfriend);//根据order找到对应friendList，然后插入friend。
//         }
    }

    ConnectionPool::closeConnection(db);
    return friendLists;
}

int UserServer::getMessage(int QQnum,QString message){


}
int UserServer::SendMessage(QString message){

}
int UserServer::SendImg(int QQnum,QString message){

}
int UserServer::logMsg(int QQnum,QString message){
    QSqlQuery query(db);
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    query.prepare("INSERT INTO `myqq`.`noreadmessage` (`uid`, `mid`,`time`, `words`) VALUES (?,?, ?,?);");
    query.bindValue(0,QQnum);//uid是接收方ID；
    query.bindValue(1,this->userInfo->QQnum);//mid是发送方ID；
    query.bindValue(2,timeT);
    query.bindValue(3,message);
    query.exec();

    return 1;
}

QByteArray UserServer::handle(QByteArray data){

    if(data.indexOf("register")>=0){//注册新账号
        int i=data.indexOf("nickname")+9;
        int r=data.indexOf(",",i);
        QString name=data.mid(i,r-i);
        i=data.indexOf("password")+9;
        r=data.indexOf(",",i);
        QString password=data.mid(i,r-i);
        int result=addUser(name,password);
        return QByteArray::number(result);
    }else if(data.indexOf("login")>=0){//登陆
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
            emit startRegisterToServer();
            return "success";
        }else{
            return "false";
        }

    }
    if(!this->userInfo->haveLogin)return "noAuthority";//  已登录才获得执行下面的服务

    if(data.indexOf("getfList")>=0){//获取好友列表
         QVector<FriendList *>result= getFriendLists(this->userInfo->QQnum);
         QByteArray myFriend;
         for(FriendList *f:result){
            myFriend.append("order:"+QByteArray::number(f->order)+",description:"+f->description+",");
            for(Friend mf:f->friends){
                myFriend.append(QByteArray::number(mf.mid)+",");
            }
            myFriend.append("*\r\n");
         }

        return myFriend;
     }else if(data.indexOf("getMsg")>=0){//获取历史消息

        return "ok";
    }else if(data.indexOf("Msg")>=0){//收到要发送给别人的消息
        int i=data.indexOf("to")+3;
        int r=data.indexOf(",",i);
        int des=data.mid(i,r-i).toInt();
        i=data.indexOf("msgID")+6;
        r=data.indexOf(",",i);
        int msgId=data.mid(i,r-i).toInt();
        i=data.indexOf("text")+5;
        r=data.indexOf("*",i);
        QByteArray sendData=data.mid(i,r-i);
        emit sendToOtherQQ(sendData,des);
        return "ok";
    }

    return "noCmd";

}

