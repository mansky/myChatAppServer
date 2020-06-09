#include "widget.h"
#include <QApplication>
#include <QDebug>
//#include "LowLevelServer/chatAppServer.h"
#include "MainWindow.h"
#include "UserServer/UserServer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    //mainWindow.show();
    UserServer userServer;
    QVector<FriendList> friendLists=userServer.getFriendLists(10000);
    return a.exec();
}
