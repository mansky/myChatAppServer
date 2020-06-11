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

signals:
    void RegisterToServer(ChatTcpSocket *socket,int QQnum);
    void fromOther( QByteArray data);//发送信号的槽
public slots:
    void sendToOtherQQ(const  QByteArray &  data,int QQnum);//处理跨线程的消息通信
    void sentData( QByteArray data);//发送信号的槽
    void disConTcp();
    void startRegisterToServer();
protected slots: 
    void dataReceived();//接收数据
private:
    UserInfo *userInfo;
    UserServer *userServer;
    int encodeWrite( QByteArray data);
    QByteArray decodeRead();

};

#endif // CHATTCPSOCKET_H
