#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "testthread.h"
#include <QMainWindow>
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QLabel>
#include <ActiveQt/QAxObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct Today
     {

         QString productId;

         QString ordId;

         QString oprnId;

         QString equAssetsNo;

         QString stationNo;

         QString tmplId;

         QString time;


     };

struct Forecast

     {

         QString id;


         QString inspSeq;


         QString value;


     };
class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void fetchTimesignals();

private slots:
    void onNewConnection();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onClientConnected(); //Client Socket connected
    void onClientDisconnected();//Client Socket disconnected
    void onSocketReadyRead();//读取socket传入的数据*/
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
    QString xiaoyan(QString val);

    void current(QByteArray temp_current);

    void conclusion(QByteArray temp_pressure);

    void voltage1(QByteArray temp_voltage);



    void current1(QByteArray temp_current);


    void fetchTime();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


    void serialPort_readyRead();

    void serialPort2_readyRead();

    void accept();

    void accept1(int i);

    void accept2();

    void accept3();

    void accept4();

    void accept5();

    void onSocketReadyRead1();
    void onConnected();
    void onDisconnected();

    void onSocketReadyRead2();
    void onConnected2();
    void onDisconnected2();

    void onSocketReadyRead3();
    void onConnected3();
    void onDisconnected3();

    void onSocketReadyRead4();
    void onConnected4();
    void onDisconnected4();

    void onSocketReadyRead5();
    void onConnected5();
    void onDisconnected5();

    void onSocketReadyRead6();
    void onConnected6();
    void onDisconnected6();

    void onSocketReadyRead7();
    void onConnected7();
    void onDisconnected7();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel* m_pLabListen;//状态栏标签
    QLabel* m_pLabSocketState;//状态栏标签
    QTcpServer *m_pTcpServer;//tcp服务器
    QTcpSocket* m_pTcpSocket;//tcp通信的Socket
    QList<QTcpSocket*> socket_list;
    //testthread *test_thread;
    QSerialPort serial,serial2;
    QString getLocalIP();//获取本机IP地址
    QTimer *myTimer;
    Today today;
    Forecast forecast[50];
    QTcpSocket *clent,*clent2,*clent3,*clent4,*clent5,*clent6,*clent7;
};
#endif // MAINWINDOW_H
