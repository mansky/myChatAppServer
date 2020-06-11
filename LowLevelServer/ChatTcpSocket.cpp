#include "LowLevelServer/ChatTcpSocket.h"

#include <QThread>

ChatTcpSocket::ChatTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disConTcp()));
    userInfo=new UserInfo();

}

ChatTcpSocket::~ChatTcpSocket()
{

}

void ChatTcpSocket::sentData(const QByteArray &data)
{
    write(data);
}

void ChatTcpSocket::dataReceived(){
    auto data  = this->readAll();
    if(data.indexOf("exit")>=0){
        this->disconnectFromHost();
        return;
    }
    UserServer userServer;
    userServer.userInfo=this->userInfo;
    connect(&userServer,SIGNAL(sendToOtherQQ(const QByteArray&,int )),this->masterServer,SLOT(sendToOtherQQ(const QByteArray&,int )));
    write(userServer.handle(data));
}

void ChatTcpSocket::disConTcp(){
    this->disconnectFromHost();
 }



