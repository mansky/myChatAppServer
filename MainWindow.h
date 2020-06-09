#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>


class MainWindow :  public QDialog
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0,Qt::WindowFlags f=0);
    ~MainWindow();
private:
    QListWidget *ContentListWidget;
    QLabel *PortLabel;
    QLineEdit *PortLineEdit;
    QPushButton *CreateBtn;
    QGridLayout *mainLayout;
    int port;
    //Server *server;
public slots:
    void slotCreateServer();
    void updateServer(QString,int);
};

#endif // MAINWINDOW_H
