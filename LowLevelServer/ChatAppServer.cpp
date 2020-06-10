#include "LowLevelServer/ChatAppServer.h"
#include "LowLevelServer/ChatTcpSocket.h"
#include <qthread.h>

ChatAppServer::ChatAppServer(QObject *parent,int port){
    listen(QHostAddress::Any,port); //监听端口消息
    qDebug()<<"listen"<<port<<endl;
}



ChatAppServer::~ChatAppServer(){
    this->close();

}

void ChatAppServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
//    auto th = ThreadHandle::getClass().getThread();
//    auto tcpTemp = new TcpSocket(socketDescriptor);
//    QString ip =  tcpTemp->peerAddress().toString();
//    qint16 port = tcpTemp->peerPort();

//    connect(tcpTemp,&TcpSocket::sockDisConnect,this,&TcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
//    connect(this,&TcpServer::sentDisConnect,tcpTemp,&TcpSocket::disConTcp);//断开信号

//    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
//    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中
//    emit connectClient(socketDescriptor,ip,port);

    ChatTcpSocket *socket = new ChatTcpSocket(socketDescriptor);

    qDebug()<<socket->peerAddress()<<"|"<<socket->peerPort()<<endl;

//    connect(socket, SIGNAL(readyRead()), socket, SLOT(ReadAndParseData())); // 会移进线程里
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));

    QThread *thread = new QThread(socket); // 以socket为父类，当socket释放删除后也会删除线程，或者将线程的quit信号关联槽deleteLater()也可以达到效果
    connect(socket, SIGNAL(disconnected()), thread, SLOT(quit()));
    socket->moveToThread(thread);
    thread->start();
    emit newConnection();

}
