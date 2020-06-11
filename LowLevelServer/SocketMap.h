#include "LowLevelServer/ChatTcpSocket.h"
#include <QMutex>
#include <QHash>


class SocketMap{
public:
    static int insert(int QQnum,ChatTcpSocket* src);
    static ChatTcpSocket* find(int QQnum);
    static int remove(int QQnum);
    static void release();

     ~SocketMap();
private:
    SocketMap();
    static SocketMap* myInstance;
    static QMutex initMutex;
    static QMutex resourceMutex;
    QHash<int,ChatTcpSocket*>  socketResource;

};
