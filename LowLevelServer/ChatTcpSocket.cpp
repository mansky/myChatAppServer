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
    qDebug() <<QThread::currentThread();
    UserServer UserServer;
    UserServer.userInfo=this->userInfo;
    write(UserServer.handle(data));
}

void ChatTcpSocket::disConTcp(){
    qDebug() <<" delete socket";
    this->disconnectFromHost();
 }
