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

    void fetchTime();

    void Sleep(int i);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void serialPort_readyRead();

    void accept();

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
    QThread *test_thread;
    QSerialPort serial;
    QString getLocalIP();//获取本机IP地址
};
#endif // MAINWINDOW_H
