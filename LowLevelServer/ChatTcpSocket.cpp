#include "LowLevelServer/ChatTcpSocket.h"
#include "LowLevelServer/SocketMap.h"
#include <QThread>

ChatTcpSocket::ChatTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disConTcp()));
    userInfo=new UserInfo();
    userServer=new UserServer();
    userServer->userInfo=this->userInfo;

    connect(userServer,SIGNAL(sendToOtherQQ(const QByteArray &,int )),this,SLOT(sendToOtherQQ(const QByteArray &,int )));
    connect(userServer,SIGNAL(startRegisterToServer()),this,SLOT(startRegisterToServer()));
    connect(this,SIGNAL(fromOther( QByteArray )),this,SLOT(sentData(QByteArray)));

}

ChatTcpSocket::~ChatTcpSocket()
{
    if(userInfo->haveLogin){
        SocketMap::remove(userInfo->QQnum);
    }
    delete userInfo;
    delete userServer;
}


void ChatTcpSocket::dataReceived(){
    auto data  = decodeRead();
    if(data.indexOf("exit")>=0){
        this->disconnectFromHost();
        return;
    }   
    encodeWrite(userServer->handle(data));
}
QByteArray ChatTcpSocket::decodeRead(){
    return this->readAll();
    //解密
}

int ChatTcpSocket::encodeWrite( QByteArray data){
    //应该先加密，再传输
     write(data);
     return 1;
}

void ChatTcpSocket::sentData( QByteArray  data)
{
    encodeWrite(data);
}

void ChatTcpSocket::sendToOtherQQ(const QByteArray &  data,int QQnum)
{
    ChatTcpSocket *desSocket =SocketMap::find(QQnum);
    if(NULL==desSocket){
        this->userServer->logMsg(QQnum,data);
    }else{
        desSocket->fromOther(data);
    }
}

void ChatTcpSocket::startRegisterToServer()
{
    SocketMap::insert(userInfo->QQnum,this);
}

void ChatTcpSocket::disConTcp(){
    this->disconnectFromHost();
}
