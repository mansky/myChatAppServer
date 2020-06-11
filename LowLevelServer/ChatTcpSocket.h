#ifndef CHATTCPSOCKET_H
#define CHATTCPSOCKET_H


#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include "UserServer/UserServer.h"
#include "UserServer/UserInfo.h"
#include "LowLevelServer/ChatAppServer.h"
class ChatTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ChatTcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~ChatTcpSocket();

    ChatAppServer *masterServer;

public slots:
    void sentData(const QByteArray & data);//发送信号的槽
    void disConTcp();

protected slots:
    void dataReceived();//接收数据
private:
    UserInfo *userInfo;

};

#endif // CHATTCPSOCKET_H
