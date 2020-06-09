#include "MainWindow.h"


//服务端主界面类
MainWindow::MainWindow(QWidget *parent,Qt::WindowFlags f)
    :QDialog(parent,f){
    setWindowTitle(tr("ChatServer"));  //程序在main中创建TcpSever对象，因此会从这里开始执行
    ContentListWidget = new QListWidget;
    PortLabel = new QLabel(tr("Listen"));
    PortLineEdit = new QLineEdit;

    CreateBtn = new QPushButton(tr("Creat Server"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(ContentListWidget,0,0,1,2);
    mainLayout->addWidget(PortLabel,1,0);
    mainLayout->addWidget(PortLineEdit,1,1);
    mainLayout->addWidget(CreateBtn,2,0,1,2);

    port=8848;
    PortLineEdit->setText(QString::number(port));

    connect(CreateBtn,SIGNAL(clicked()),this,SLOT(slotCreateServer())); //点击按钮创建服务
}

//创建d
void MainWindow::slotCreateServer()
{
    //启动服务器，创建Server
    qDebug("hi");
//    server = new Server(this,port);
//    //检测更新服务器信号
//    connect(server,SIGNAL(updateServer(QString,int)),this,SLOT(updateServer(QString,int)));

//    CreateBtn->setEnabled(false);
}
//更新服务器，即同步客户端发送的消息到服务端程序界面
void MainWindow::updateServer(QString msg,int length)
{
    ContentListWidget->addItem(msg.left(length));
}


MainWindow::~MainWindow(){

}
