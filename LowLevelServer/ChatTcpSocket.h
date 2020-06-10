#ifndef CHATTCPSOCKET_H
#define CHATTCPSOCKET_H


#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include "UserServer/UserServer.h"

class ChatTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ChatTcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~ChatTcpSocket();

signals:
    //void readData(const int,const QString &,const quint16,const QByteArray &);
   // void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void sentData(const QByteArray & data);//发送信号的槽
    void disConTcp();

protected slots:
    void dataReceived();//接收数据
//    void startNext();//处理下一个
//protected:
//    QFutureWatcher<QByteArray> watcher;
//    QQueue<QByteArray> datas;
private:
//   qintptr socketID;
//    QMetaObject::Connection dis;

};

#endif // CHATTCPSOCKET_H
