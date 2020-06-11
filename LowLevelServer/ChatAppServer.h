#ifndef CHATAPPSERVER_H
#define CHATAPPSERVER_H

#include <QTcpServer>

class ChatAppServer: public QTcpServer
{
    Q_OBJECT
public:
    ChatAppServer(QObject *parent=0,int port=8888);
    ~ChatAppServer();


protected:
    void incomingConnection(qintptr socketDescriptor) override;//覆盖已获取多线程
private:



};


#endif // CHATAPPSERVER_H
