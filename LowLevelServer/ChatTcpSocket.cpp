#include "LowLevelServer/ChatTcpSocket.h"

ChatTcpSocket::ChatTcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(disConTcp()));
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
    UserServer UserServer;
    UserServer.handle(data);
}

void ChatTcpSocket::disConTcp(){
    qDebug() <<" delete socket";
    this->disconnectFromHost();
    this->deleteLater();

 }
