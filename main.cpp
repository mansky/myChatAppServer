#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "LowLevelServer/ChatAppServer.h"
#include "MainWindow.h"
#include "UserServer/UserServer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    ChatAppServer chatAppServer;
//  chatAppServer.listen(QHostAddress::Any,6666);
//  UserServer userServer;
//  QVector<FriendList> friendLists=userServer.getFriendLists(10000);
    return a.exec();
}
