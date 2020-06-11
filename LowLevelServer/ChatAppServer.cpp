#include "LowLevelServer/ChatAppServer.h"
#include "LowLevelServer/ChatTcpSocket.h"
#include <qthread.h>

ChatAppServer::ChatAppServer(QObject *parent,int port):QTcpServer(parent){
    listen(QHostAddress::Any,port); //监听端口消息
    qDebug()<<"listen"<<port<<endl;
}



ChatAppServer::~ChatAppServer(){
    this->close();

}

void ChatAppServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{

    ChatTcpSocket *socket = new ChatTcpSocket(socketDescriptor);
    socket->masterServer=this;
    QThread *thread = new QThread();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), thread, SLOT(quit()));

    socket->moveToThread(thread);
    thread->start();
    emit newConnection();

}
//void ChatAppServer::sendToOtherQQ(const QByteArray & data,int QQnum){
//    qDebug()<<data<<"|"<<QQnum<<endl;
//}
