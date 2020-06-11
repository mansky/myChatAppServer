#include "LowLevelServer/SocketMap.h"

SocketMap* SocketMap::myInstance=NULL;
QMutex SocketMap::initMutex;
QMutex SocketMap::resourceMutex;

//插入
int SocketMap::insert(int QQnum,ChatTcpSocket* src){
    if(NULL==myInstance){
        QMutexLocker locker(&initMutex);
        if(NULL==myInstance){
            myInstance = new SocketMap();
        }
        locker.unlock();
    }
    QMutexLocker locker(&resourceMutex);
    myInstance->socketResource.insert(QQnum,src);
    return 1;
}
//查找
ChatTcpSocket* SocketMap::find(int QQnum){
    if(NULL==myInstance){
        QMutexLocker locker(&initMutex);
        if(NULL==myInstance){
            myInstance = new SocketMap();
        }
        locker.unlock();
    }
    ChatTcpSocket *result=NULL;
    QMutexLocker locker(&resourceMutex);
    result=myInstance->socketResource.value(QQnum,NULL);
    return result;
}
//移除
int SocketMap::remove(int QQnum){
    if(NULL==myInstance){
        QMutexLocker locker(&initMutex);
        if(NULL==myInstance){
            myInstance = new SocketMap();
        }
        locker.unlock();
    }
    QMutexLocker locker(&resourceMutex);
    myInstance->socketResource.remove(QQnum);
    return 1;
}

void SocketMap::release(){
 if(NULL!=myInstance){
    QMutexLocker locker(&resourceMutex);
    delete myInstance;
    myInstance=NULL;

 }
}
SocketMap::SocketMap(){

}
SocketMap::~SocketMap(){

}
