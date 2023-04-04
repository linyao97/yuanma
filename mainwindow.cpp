#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QtNetwork>
#include <qdebug.h>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDomComment>

int dq=0,dt=0,dj=0,dxj=0;
int qr=0;
int qt=0;
int msec=3000;
int jy=0;
int m=0,kw1=0,kw2=0,kw3=0,kw4=0,bihe=0,gongxu1=0,gongxu2=0,gongxu3=0,gongxu4=0,lscc2,lscc3,gxwc=0,bhgs=0,idy=0,kaishi=0,it,ict=0,icu=0,idt=0,sd=1000,gx=0;
QString lscc,lscc4,lscc5;
QSqlDatabase db ;
QList<QString> nydl,xqnydl,id,jydy,jydz,jydy2,jydz2,jydl,jydl2;
QList<int> ny,xqny,jycs,jycs2;
int sny,sxqny,qk=0,gx1=0,gx2=0,gx3=0,gx4=0,sjycs;
QString days,dayss,gongxu,ordId,tmplId,Id,matcode="",snydl,sxqnydl,sjydz,sjydz2;
QByteArray sz;
QString addr1,addr2,addr3,addr4,addr5,addr6;
int port1,port2,port3,port4,port5,port6;
int gc1,gc2,gc3,gc4;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pTcpServer = new QTcpServer(this);
    ui->comboBox->addItem("触点间绝缘");
    ui->comboBox_2->addItem("线圈触点间绝缘");
    ui->comboBox_3->addItem("触点间耐压");
    ui->comboBox_5->addItem("线圈触点间耐压");
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->comboBox->setEnabled(false);

    //ui->lineEdit->setText(QString::number( sd));
    ui->spinBox->setEnabled(false);
    ui->spinBox_2->setEnabled(false);
    ui->spinBox_3->setEnabled(false);
    ui->spinBox_4->setEnabled(false);
    ui->lineEdit->setEnabled(false);

    clent = new QTcpSocket(this);
    clent2 = new QTcpSocket(this);
    clent3 = new QTcpSocket(this);
    clent4 = new QTcpSocket(this);
    clent5 = new QTcpSocket(this);
    clent6 = new QTcpSocket(this);


    myTimer = new QTimer(this);           //初始化QTimer



    //connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    //connect(this,SIGNAL(fetchTimesignals()),this,SLOT(fetchTime()));
    connect(&serial,&QSerialPort::readyRead,this,&MainWindow::serialPort_readyRead);
    connect(&serial2,&QSerialPort::readyRead,this,&MainWindow::serialPort2_readyRead);

    connect(clent,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead1);
    connect(clent,&QTcpSocket::connected,this,&MainWindow::onConnected);
    connect(clent,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected);

    connect(clent2,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead2);
    connect(clent2,&QTcpSocket::connected,this,&MainWindow::onConnected2);
    connect(clent2,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected2);



    connect(clent3,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead3);
    connect(clent3,&QTcpSocket::connected,this,&MainWindow::onConnected3);
    connect(clent3,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected3);

    connect(clent4,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead4);
    connect(clent4,&QTcpSocket::connected,this,&MainWindow::onConnected4);
    connect(clent4,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected4);

    connect(clent5,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead5);
    connect(clent5,&QTcpSocket::connected,this,&MainWindow::onConnected5);
    connect(clent5,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected5);

    connect(clent6,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead6);
    connect(clent6,&QTcpSocket::connected,this,&MainWindow::onConnected6);
    connect(clent6,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected6);

    connect(clent7,&QTcpSocket::readyRead,this,&MainWindow::onSocketReadyRead7);
    connect(clent7,&QTcpSocket::connected,this,&MainWindow::onConnected7);
    connect(clent7,&QTcpSocket::disconnected,this,&MainWindow::onDisconnected7);


    ui->spinBox->setValue(1);


    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy年MM月dd日");//启动时读取当天日期
    days="D:\\数据库\\";
    days += localTime;
    days +=".accdb";
    dayss = "D:/数据库/"+localTime+".accdb;";
    int c=0;
    QFile filename(days);
    if(!filename.exists(days))
    {
        QProcess pro;
        QString strPath = "access/CreateAccess.exe";
        pro.startDetached(strPath);
        c=1;
        _sleep(2000);
        pro.close();
    }
    filename.close();
    db = QSqlDatabase::addDatabase("QODBC","test");
    QString str="DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ="+dayss;
    if(c==1)
    {

        QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
        db.setDatabaseName(dbname);
        //db.open();
        qDebug()<<dbname;
        qDebug()<<db.open();
        QSqlQuery query(db);
        QString  sqlstr = "create table insp_result(ID int,产品条码 varchar(255), 绝缘电阻OK int,绝缘电压 float, 绝缘电阻值 float,绝缘电阻漏电流 float,触点线圈间绝缘OK int,触点线圈间绝缘电压 float, 触点线圈间绝缘电阻值 float,触点线圈间绝缘漏电流 float,触点间耐压OK int,触点间耐压漏电流 float,线圈_触点间介质耐压OK int,[触点线圈间耐压漏电流（断开）] float,创建时间 varchar(255),测试结果 int,记录完毕 int,上传状态 int,上传结果 varchar(255))";
        query.exec(sqlstr);

        query.clear();
    }

    db.close();



    QFile file("configure.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in (&file);
    QTextCodec *cidec=QTextCodec::codecForName("GBK");
    in.setCodec(cidec);
    int i=0;
    while (!in.atEnd())
    {
        QString line = cidec->fromUnicode(in.readLine());
        i=i+1;
        /*if(i==1)
        {
            ui->spinBox->setValue(line.toInt());
        }
        if(i==2)
        {
            ui->spinBox_2->setValue(line.toInt());
        }
        if(i==3)
        {
            ui->spinBox_3->setValue(line.toInt());
        }
        if(i==4)
        {
            ui->comboBox_4->setCurrentIndex(line.toInt());
        }
        if(i==5)
        {
            ui->comboBox->setCurrentIndex(line.toInt());
        }
        if(i==6)
        {
            ui->comboBox_2->setCurrentIndex(line.toInt());
        }
        if(i==7)
        {
            ui->comboBox_3->setCurrentIndex(line.toInt());
        }
        if(i==8)
        {
            ui->comboBox_5->setCurrentIndex(line.toInt());
        }
        if(i==9)
        {
            ui->comboBox_6->setCurrentIndex(line.toInt());
        }
        if(i==10)
        {
            if(line=="1")
            {
                ui->radioButton->setChecked(true);
            }
            else
            {
                ui->radioButton_2->setChecked(true);
            }
        }*/
        if(i==9)
        {
            ui->lineEdit->setText(QString::number( line.toInt()));
        }

    }

}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::fetchTime()
{

}

void MainWindow::accept()
{
    QString msg ="7B 06 00 00 06 7D";

    QByteArray str;
    StringToHex(msg,str);
    clent3->close();
    clent4->close();
    clent3->connectToHost(addr3,port3);
    clent4->connectToHost(addr4,port4);
    for (int i=0;i<5;i++)
    {
        if(i==1)
        {


            if(gx1==1)
            {
                ui->textEdit->insertPlainText("耐压未读取\n");
                if(kw3!=0)
                {
                    kw3=kw3-1;
                }
                else
                {
                    nydl.append("ng");
                    ny.append(0);
                }
                if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
                {
                    id.takeAt(0);
                    ny.takeAt(0);
                    jycs.takeAt(0);
                    jydy.takeAt(0);
                    jydz.takeAt(0);
                    jydl.takeAt(0);
                    jycs2.takeAt(0);
                    jydy2.takeAt(0);
                    jydz2.takeAt(0);
                    jydl2.takeAt(0);
                    nydl.takeAt(0);
                    xqny.takeAt(0);
                    xqnydl.takeAt(0);//数据向前传递与产品对应
                    ui->textEdit->insertPlainText("错误数据");
                }
                else if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
                {

                    QDateTime local(QDateTime::currentDateTime());
                    QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

                    QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
                    db.setDatabaseName(dbname);
                    bool ok =db.open();
                    qDebug() << ok;
                    QSqlQuery query(db);
                    QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
                    query.exec(sqlstr);
                    query.next();
                    qDebug() << query.value(0).toString();
                    QString m = query.value(0).toString();
                    QString str1="";
                    if(jycs.at(0)==0)
                    {
                        str1+="绝缘1NG ";
                    }
                    if(jycs2.at(0)==0)
                    {
                        str1+="触点线圈间绝缘NG";
                    }
                    if(xqny.at(0)==0)
                    {
                        str1+="线圈触点间耐压NG ";
                    }
                    if(ny.at(0)==0)
                    {
                        str1+="触点间耐压NG";
                    }
                    if(str1!="")
                    {
                        accept2();
                        bhgs++;
                        QString str2=QString::number(bhgs);
                        str2 +=localTime+id.at(0)+":"+str1+"/n";
                        ui->textEdit_2->insertPlainText(str2);
                        QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                        if(!file.open(QIODevice::WriteOnly | QFile::Text))
                        {

                        }
                        file.write(str2.toUtf8());
                        file.close();
                        ui->textEdit->insertPlainText("产品ng");
                    }
                    else if(m=="")
                    {
                        /*if(bihe==1)
                            {
                                query.clear();
                                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                                qDebug() << sqlstr;
                                query.exec(sqlstr);

                            }
                            else
                            {
                                query.clear();
                                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"',null,null,'"+localTime+"',1,1,0);";
                                qDebug() << sqlstr;
                                query.exec(sqlstr);
                            }*/
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                        query.exec(sqlstr);
                        ui->textEdit->insertPlainText("写入完成\n");
                        accept2();
                    }
                    else
                    {/*
                        if(bihe==1)
                            {
                                int a=m.toInt()+1;
                                m=QString::number(a);
                                query.clear();
                                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                                query.exec(sqlstr);
                            }
                            else
                            {
                                int a=m.toInt()+1;
                                m=QString::number(a);
                                query.clear();
                                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"','"+QString::number( xqny.at(0))+"','"+xqnydl.at(0)+"',null,null'"+localTime+"',1,1,0);";
                                query.exec(sqlstr);
                            }*/

                        int a=m.toInt()+1;
                        m=QString::number(a);
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";


                        query.exec(sqlstr);
                        accept2();
                        ui->textEdit->insertPlainText("写入完成\n");
                    }

                    db.close();

                }

            }
            gx1=1;
            clent->write("RD 0?\r");

        }
        if(i==2)
        {


            if(gx2==1)
            {
                ui->textEdit->insertPlainText("耐压未读取\n");
                if(kw4!=0)
                {
                    kw4=kw4-1;
                }
                else
                {
                    xqnydl.append("ng");
                    xqny.append(0);
                }
                if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
                {
                    id.takeAt(0);
                    ny.takeAt(0);
                    jycs.takeAt(0);
                    jydy.takeAt(0);
                    jydz.takeAt(0);
                    jydl.takeAt(0);
                    jycs2.takeAt(0);
                    jydy2.takeAt(0);
                    jydz2.takeAt(0);
                    jydl2.takeAt(0);
                    nydl.takeAt(0);
                    xqny.takeAt(0);
                    xqnydl.takeAt(0);//数据向前传递与产品对应
                    ui->textEdit->insertPlainText("错误数据");
                }
                else if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
                {

                    QDateTime local(QDateTime::currentDateTime());
                    QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

                    QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
                    db.setDatabaseName(dbname);
                    bool ok =db.open();
                    qDebug() << ok;
                    QSqlQuery query(db);
                    QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
                    query.exec(sqlstr);
                    query.next();
                    qDebug() << query.value(0).toString();
                    QString m = query.value(0).toString();
                    QString str1="";
                    if(jycs.at(0)==0)
                    {
                        str1+="绝缘1NG ";
                    }
                    if(jycs2.at(0)==0)
                    {
                        str1+="触点线圈间绝缘NG";
                    }
                    if(xqny.at(0)==0)
                    {
                        str1+="线圈触点间耐压NG ";
                    }
                    if(ny.at(0)==0)
                    {
                        str1+="触点间耐压NG";
                    }
                    if(str1!="")
                    {
                        accept2();
                        bhgs++;
                        QString str2=QString::number(bhgs);
                        str2 +=localTime+id.at(0)+":"+str1+"/n";
                        ui->textEdit_2->insertPlainText(str2);
                        QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                        if(!file.open(QIODevice::WriteOnly | QFile::Text))
                        {

                        }
                        file.write(str2.toUtf8());
                        file.close();
                        ui->textEdit->insertPlainText("产品ng");
                    }
                    else if(m=="")
                    {

                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                        query.exec(sqlstr);
                        ui->textEdit->insertPlainText("写入完成\n");
                        accept2();
                    }
                    else
                    {

                        int a=m.toInt()+1;
                        m=QString::number(a);
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";


                        query.exec(sqlstr);
                        accept2();
                        ui->textEdit->insertPlainText("写入完成\n");
                    }

                    db.close();

                }
            }
            gx2=1;
            clent2->write("RD 0?\r");

        }
        /*if(i==3)
        {

            clent3->write(str);
            if(gx3==1)
            {
                ui->textEdit->insertPlainText("绝缘未读取\n");
                if(kw2!=0)
                {

                    kw2=kw2-1;
                }
                else
                {
                    jydy2.append("ng");
                    jydz2.append("ng");
                    jycs2.append(0);
                    jydl2.append("ng");

                }
            }
            gx3=1;


        }

        if(i==4)
        {

            clent4->write(str);
            if(gx4==1)
            {
                ui->textEdit->insertPlainText("绝缘未读取\n");
                if(kw1!=0)
                {

                    kw1=kw1-1;
                }
                else
                {
                    jydz.append("ng");
                    jydy.append("ng");
                    jycs.append(0);
                    jydl.append("ng");
                }
            }
            gx4=1;


        }*/
        


    }

}

void MainWindow::accept1(int i)
{
    QString msg ="7B 06 00 00 06 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    QByteArray str;
    StringToHex(msg,str);
    it=i;
    socket_list.at(it)->write(str);

}

void MainWindow::serialPort_readyRead()
{

}

void MainWindow::serialPort2_readyRead()
{

}
void MainWindow::on_pushButton_clicked()
{
    /*QString IP="192.168.0.201";
    quint16 port=8234;
    QHostAddress addr(IP);
    m_pTcpServer->listen(addr,port);
    ui->textEdit->insertPlainText("**开始监听");
    ui->textEdit->insertPlainText("**服务器地址:"+m_pTcpServer->serverAddress().toString());
    ui->textEdit->insertPlainText("**服务器端口"+QString::number(m_pTcpServer->serverPort()));*/
    /*if(!ui->radioButton->isChecked() and !ui->radioButton_2->isChecked())
    {
        QMessageBox::about(NULL,"提示","请选型");
    }
    else
    {
    clent->connectToHost("169.254.191.177",5300);
    clent2->connectToHost("169.254.191.177",5301);
    clent3->connectToHost("169.254.191.177",5303);
    clent4->connectToHost("169.254.191.177",5302);
    clent5->connectToHost("169.254.191.178",5305);
    clent6->connectToHost("169.254.191.179",5306);

    }*/

    QFile ip("ip.txt");
    if (!ip.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in1 (&ip);
    QTextCodec *cidec1=QTextCodec::codecForName("GBK");
    in1.setCodec(cidec1);
    int i=0;
    while (!in1.atEnd())
    {
        QString line = cidec1->fromUnicode(in1.readLine());
        qDebug()<<line;
        i=i+1;
        if(i==1)
        {
           addr1=line;
        }
        if(i==2)
        {
           port1=line.toInt();
        }
        if(i==3)
        {
           addr2=line;
        }
        if(i==4)
        {
           port2=line.toInt();
        }
        if(i==5)
        {
           addr3=line;
        }
        if(i==6)
        {
           port3=line.toInt();
        }
        if(i==7)
        {
           addr4=line;
        }
        if(i==8)
        {
           port4=line.toInt();
        }
        if(i==9)
        {
           addr5=line;
        }
        if(i==10)
        {
           port5=line.toInt();
        }
        if(i==11)
        {
           addr6=line;
        }
        if(i==12)
        {
           port6=line.toInt();
        }
    }


    clent->connectToHost(addr1,port1);
    clent2->connectToHost(addr2,port2);
    clent3->connectToHost(addr3,port3);
    clent4->connectToHost(addr4,port4);
    clent5->connectToHost(addr5,port5);
    clent6->connectToHost(addr6,port6);


}


void MainWindow::closeEvent(QCloseEvent *event)
{//关闭窗口时停止监听
    if (m_pTcpServer->isListening())
        m_pTcpServer->close();//停止网络监听

}

void MainWindow::onClientConnected()
{
    ui->textEdit->insertPlainText("**client socket connected");
    ui->textEdit->insertPlainText("**peer address:"+m_pTcpSocket->peerAddress().toString());
    ui->textEdit->insertPlainText("**peer port:"+QString::number(m_pTcpSocket->peerPort())+"\n");

}

void MainWindow::onClientDisconnected()
{
    ui->textEdit->insertPlainText("**client socket disconnected");
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
    case QAbstractSocket::UnconnectedState:
        ui->label->setText("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        ui->label->setText("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        ui->label->setText("scoket状态：ConnectingState");
        break;

    case QAbstractSocket::ConnectedState:
        ui->label->setText("scoket状态：ConnectedState");
        break;

    case QAbstractSocket::BoundState:
        ui->label->setText("scoket状态：BoundState");
        break;

    case QAbstractSocket::ClosingState:
        ui->label->setText("scoket状态：ClosingState");
        break;

    case QAbstractSocket::ListeningState:
        ui->label->setText("scoket状态：ListeningState");
    }
}





void MainWindow::current(QByteArray temp_current)
{
    QString str,a;
    QDataStream out_current(&temp_current,QIODevice::ReadWrite);
    while(!out_current.atEnd())
    {
        qint8 outChar = 0;
        out_current>>outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        a +=str;
    }
    bool ok;
    float i=a.toInt(&ok,16);
    a=QString::number(i);
    i=i/1000;

    lscc=QString("%1").arg(i);


}

void MainWindow::current1(QByteArray temp_current)//绝缘电阻
{
    QString str,a;
    QDataStream out_current(&temp_current,QIODevice::ReadWrite);
    while(!out_current.atEnd())
    {
        qint8 outChar = 0;
        out_current>>outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        a +=str;
    }
    bool ok;
    float i=a.toInt(&ok,16);
    a=QString::number(i);
    i=i/100;
    lscc4=QString("%1").arg(i);
}

void MainWindow::voltage1(QByteArray temp_voltage)//绝缘电压
{
    QString str,a;
    QDataStream out_voltage(&temp_voltage,QIODevice::ReadWrite);
    while(!out_voltage.atEnd())
    {
        qint8 outChar = 0;
        out_voltage>>outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        a +=str;
    }
    bool ok;
    int i=a.toInt(&ok,16);
    a=QString::number(i);

    lscc5=QString("%1").arg(i);
}
void MainWindow::conclusion(QByteArray temp_pressure)
{
    QString str,a;
    QDataStream out_pressure(&temp_pressure,QIODevice::ReadWrite);
    while(!out_pressure.atEnd())
    {
        qint8 outChar = 0;
        out_pressure>>outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        a +=str;
    }
    bool ok;
    int i=a.toInt(&ok,16);
    a=QString("%1").arg(i,4,2,QLatin1Char('0'));
    if(a.length()<8)
    {
        QString temp="0";
        for (int i=1;i<8-a.length();i++) {
            temp.append("0");
        }
        a=temp.append(a);
    }
    QString str1="11";


    if(a.left(2)=="00")
    {
        str1="结果：测试完成\n";
        gxwc=1;
    }
    else if(a.left(2)=="01")
    {
        str1="结果：ACW测试\n";
        gxwc=0;
    }
    else if(a.left(2)=="10")
    {
        str1="结果：IR测试\n";
        gxwc=0;
    }
    else if(a.left(2)=="11")
    {
        str1="结果：故障状态\n";
        gxwc=3;
    }
    else
    {
        gxwc=3;
    }

    QString b=a.left(3);

    b=a.left(5);
    b=b.right(1);
    if(b=="1")
    {
        str1 +="IR测试合格  ";
        lscc2=1;
    }
    else if(b=="0")
    {
        str1 +="IR测试不合格  ";
        lscc2=2;
    }




    b=a.left(8);
    b=b.right(1);
    if(b=="1")
    {
        str1 +="ACW测试合格\n";
        lscc3=1;
    }
    else if(b=="0")
    {
        str1 +="ACW测试不合格\n";
        lscc3=0;
    }

}

void MainWindow::onSocketReadyRead()
{


}

void MainWindow::onNewConnection()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    if(m_pTcpServer->isListening())
    {
        m_pTcpServer->close();
        ui->textEdit->insertPlainText("停止监听");
    }
    kaishi=2;
}
void MainWindow::StringToHex(QString str, QByteArray &senddata)//转换为十六进制（判断长度高地位）
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)//十六进制字节转换
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void MainWindow::on_pushButton_3_clicked()//启动耐压
{

    ui->textEdit_2->clear();
    ui->textEdit_2->setText("id:"+QString::number(id.length())+"\r");
    ui->textEdit_2->insertPlainText("ny:"+QString::number(ny.length())+"\r");
    ui->textEdit_2->insertPlainText("xqny:"+QString::number(xqny.length())+"\r");
    ui->textEdit_2->insertPlainText("jycs:"+QString::number(jycs.length())+"\r");
    ui->textEdit_2->insertPlainText("jycs2:"+QString::number(jycs2.length())+"\r");
    ui->textEdit_2->insertPlainText("kw1:"+QString::number(kw1)+"\r");
    ui->textEdit_2->insertPlainText("kw2:"+QString::number(kw2)+"\r");
    ui->textEdit_2->insertPlainText("kw3:"+QString::number(kw3)+"\r");
    ui->textEdit_2->insertPlainText("kw4:"+QString::number(kw4)+"\r");
}

QString MainWindow::xiaoyan(QString val)
{
    int c=0,len=val.size();
    for (int i=0;i<len;i++)
    {
        QString str = val.left(2);
        val = val.right(val.size()-2);
        bool ok;
        int val1=str.toInt(&ok,16);
        c=c+val1;
        i=i+1;
    }
    val=QString::number(c,16);
    val=val.right(2);
    return  val;
}

void MainWindow::on_pushButton_4_clicked()//设置
{
    if(ui->radioButton->isChecked() or ui->radioButton_2->isChecked())
    {
        QFile file1("com.txt");
        if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in1 (&file1);
        QTextCodec *cidec=QTextCodec::codecForName("GBK");
        in1.setCodec(cidec);
        int i=0;
        while (!in1.atEnd())
        {
            QString line = cidec->fromUnicode(in1.readLine());
            i=i+1;
            if(i==1)
            {
                serial.setPortName(line);
                serial.setBaudRate(QSerialPort::Baud9600);//绝缘
                serial.setDataBits(QSerialPort::Data8);
                serial.setParity(QSerialPort::NoParity);
                serial.setStopBits(QSerialPort::OneStop);
                serial.setFlowControl(QSerialPort::NoFlowControl);
                if(!serial.open(QIODevice::ReadWrite))
                {
                    QMessageBox::about(NULL,"提示","无法连接mes");
                }

            }
            if(i==1)
            {
                serial2.setPortName(line);
                serial2.setBaudRate(QSerialPort::Baud9600);//绝缘
                serial2.setDataBits(QSerialPort::Data8);
                serial2.setParity(QSerialPort::NoParity);
                serial2.setStopBits(QSerialPort::OneStop);
                serial2.setFlowControl(QSerialPort::NoFlowControl);
                if(!serial2.open(QIODevice::ReadWrite))
                {
                    QMessageBox::about(NULL,"提示","无法连接mes");
                }

            }
        }




        sd= ui->lineEdit->text().toInt();
        gongxu1=4;
        gongxu2=3;
        gongxu3=1;
        gongxu4=2;
        if(ui->radioButton->isChecked())
        {
            kw1 = ui->spinBox->text().toInt()+2;
        }
        else
        {
            kw1 = ui->spinBox->text().toInt();
        }
        kw2 = ui->spinBox_2->text().toInt()+kw1+1;
        kw3 = ui->spinBox_3->text().toInt()+kw2+1;
        kw4 = ui->spinBox_4->text().toInt()+kw3+1;

        QFile file("configure.txt");
        if(!file.open(QIODevice::WriteOnly | QFile::Text))
        {

        }
        QString str = ui->spinBox->text();
        file.write(str.toUtf8());
        file.write("\n");

        str=ui->spinBox_2->text();
        file.write(str.toUtf8());
        file.write("\n");

        str=ui->spinBox_3->text();
        file.write(str.toUtf8());
        file.write("\n");

        str=QString::number( ui->comboBox->currentIndex());
        file.write(str.toUtf8());
        file.write("\n");

        str=QString::number( ui->comboBox_2->currentIndex());
        file.write(str.toUtf8());
        file.write("\n");

        str=QString::number( ui->comboBox_3->currentIndex());
        file.write(str.toUtf8());
        file.write("\n");

        str=QString::number( ui->comboBox_5->currentIndex());
        file.write(str.toUtf8());
        file.write("\n");



        str=bihe;
        file.write(str.toUtf8());
        file.write("\n");

        str=ui->lineEdit->text();
        file.write(str.toUtf8());
        file.write("\n");

        file.close();
        ui->lineEdit->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        ui->spinBox_3->setEnabled(false);
    }
    else {
        QMessageBox::about(NULL,"提示","未选二维码方向");
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    clent->write("RD 0?\r");
    QString msg ="7B 06 00 00 06 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    QByteArray str;
    StringToHex(msg,str);
    clent3->write(str);

}

void MainWindow::accept2()
{
    sny=ny.at(0);
    sxqny=xqny.at(0);
    snydl=nydl.at(0);
    sjycs=jycs.at(0);
    sxqnydl=xqnydl.at(0);
    sjydz=jydz.at(0);
    sjydz2=jydz2.at(0);
    QString msg ="02 || "+id.at(0)+"||||0057-1="+QString::number( sny)+"'0||||0056-1="+ QString::number(sxqny)+"'0||||0019-1="+sjydz+"'0||||0083-1="+snydl+"'0||||0084-1="+sxqnydl+"'0||||0116-1="+sjydz2+"'0||||0055-1="+ QString::number(sjycs)+"'0||||0D";
    ui->textEdit_2->setText("[发送]"+msg+"\n");
    QByteArray str=msg.toUtf8();
    //StringToHex(msg,str);
    serial.write(str);
    id.takeAt(0);
    jycs.takeAt(0);
    jydy.takeAt(0);
    jydz.takeAt(0);
    jydl.takeAt(0);
    jycs2.takeAt(0);
    jydy2.takeAt(0);
    jydz2.takeAt(0);
    jydl2.takeAt(0);
    ny.takeAt(0);
    nydl.takeAt(0);
    xqny.takeAt(0);
    xqnydl.takeAt(0);//数据向前传递与产品对
    /*QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(QUrl("http://192.168.10.22/sfis/ws/mes/product/v1?wsd"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,(QVariant("text/xml;charst=utf-8")));
    QString soapXML = "<soapenv:Envelope xmlns:soapenv='http://schemas.xmlsoap.org/soap/envelope/' xmlns:impl='http://impl.v1.mes.ws.hfepc.com/'><soapenv:Header/><soapenv:Body><impl:getByCode><arg0>"+id.at(0)+"</arg0></impl:getByCode></soapenv:Body></soapenv:Envelope>" ;
    qDebug()<<soapXML;
    QNetworkReply *reply = manager->post(req,soapXML.toUtf8());
    QAbstractSocket::connect(reply,&QNetworkReply::finished,[=](){
        QByteArray bytes = reply->readAll();

        QDomDocument doc;
        doc.setContent(bytes);
        QDomNodeList nodelist = doc.elementsByTagName("curOprnId");
        gongxu=nodelist.at(0).toElement().text();
        ui->textEdit->insertPlainText("工序ID"+gongxu+"\r");
        if(gongxu!="44")
        {

            QMessageBox::about(NULL,"提示","当前产品不在本工序");

            return;
        }
        else
        {

            nodelist = doc.elementsByTagName("ordId");
            ordId=nodelist.at(0).toElement().text();
            nodelist = doc.elementsByTagName("id");
            Id=nodelist.at(0).toElement().text();
            today.productId = Id;
            today.ordId =  ordId;
            today.oprnId =gongxu;
            ui->textEdit->insertPlainText("产品ID"+today.productId+"\r");
            ui->textEdit->insertPlainText("工单ID"+today.ordId+"\r");

            accept3();
        }

    });*/
}

void MainWindow::accept3()
{
    QNetworkAccessManager *manager1 = new QNetworkAccessManager();
    manager1->clearAccessCache();
    QNetworkRequest req;
    req.setUrl(QUrl("http://192.168.10.22/sfis/ws/order/v3?wsdl"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,(QVariant("text/xml;charst=utf-8")));
    QString soapXML = "<soapenv:Envelope xmlns:soapenv='http://schemas.xmlsoap.org/soap/envelope/' xmlns:impl='http://impl.v3.biz.ws.hfepc.com/'><soapenv:Header/><soapenv:Body><impl:getById><arg0>"+ordId+"</arg0></impl:getById></soapenv:Body></soapenv:Envelope>" ;
    QNetworkReply *reply1 = manager1->post(req,soapXML.toUtf8());
    QAbstractSocket::connect(reply1,&QNetworkReply::finished,[=](){
        QByteArray bytes = reply1->readAll();
        QDomDocument doc;
        doc.setContent(bytes);
        QDomNodeList nodelist = doc.elementsByTagName("tmplId");    /**< 读取类型节点集合 */
        tmplId=nodelist.at(0).toElement().text();
        today.tmplId = tmplId;
        nodelist=doc.elementsByTagName("matCode");
        ui->textEdit->insertPlainText("工单号"+today.tmplId+"\r");
        accept4();

    });
}

void MainWindow::accept4()
{
    QNetworkAccessManager *manager2 = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(QUrl("http://192.168.10.22/sfis/ws/inspStandard/v3?wsdl"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,(QVariant("text/xml;charst=utf-8")));
    QString soapXML = "<soapenv:Envelope xmlns:soapenv='http://schemas.xmlsoap.org/soap/envelope/' xmlns:impl='http://impl.v3.biz.ws.hfepc.com/'><soapenv:Header/><soapenv:Body><impl:getByTmplId><arg0>"+tmplId+"</arg0></impl:getByTmplId></soapenv:Body></soapenv:Envelope>" ;
    QNetworkReply *reply2 = manager2->post(req,soapXML.toUtf8());
    QAbstractSocket::connect(reply2,&QNetworkReply::finished,[=](){
        QByteArray bytes = reply2->readAll();
        QDomDocument doc;
        doc.setContent(bytes);
        QDomNodeList nodelist = doc.elementsByTagName("paramId"); /**< 读取类型节点集合 */
        QDomNodeList nodelist1 = doc.elementsByTagName("oprnId"); /**< 读取类型节点集合 */
        for(int i=0; i<nodelist1.count(); i++){
            QString helloWorld=nodelist1.at(i).toElement().text();
            /*if(helloWorld=="32")
            {
                QString helloWorld=nodelist.at(i).toElement().text();
                forecast[gx].id=helloWorld;
                forecast[gx].inspSeq="1";
                forecast[gx].value="2.5";
                gx++;
            }*/
            if(helloWorld=="44")
            {
                QString helloWorld=nodelist.at(i).toElement().text();
                if(helloWorld=="148")
                {
                    gx++;
                }
            }


        }
        accept5();
    });

}

void MainWindow::accept5()
{
    QJsonObject yesjson;
    //构建json对象yesjson




    //插入数据///




    //构建Json对象




    QJsonObject subjson[gx];



    //构建Json数组




    QJsonArray jsonArray;


    for (int i = 0; i <gx; ++i)

    {

        subjson[i].insert( "id", forecast[i].id);
        subjson[i].insert( "inspSeq", forecast[i].inspSeq);
        subjson[i].insert( "value", forecast[i].value);

    }



    for (int i = 0; i <gx; ++i)

    {

        jsonArray.append(QJsonValue(subjson[i]));

    }

    QJsonObject dataobj;
    //构建json对象dataobj



    dataobj.insert("ordId",today.ordId);

    dataobj.insert("tmplId",today.tmplId);

    dataobj.insert("operatorNo", "admin");

    dataobj.insert("productId",today.productId);

    dataobj.insert("equAssetsNo",today.equAssetsNo);

    dataobj.insert("stationNo",today.stationNo);

    dataobj.insert("oprnId", today.oprnId);

    dataobj.insert("params",QJsonValue(jsonArray));




    QJsonObject json;
    //构建json对象json

    //json.insert("inspResultVO", dataobj);

    QJsonDocument document;



    document.setObject(dataobj);







    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);



    //将dataobj中的数据插入到json对象中
    QNetworkAccessManager *manager3 = new QNetworkAccessManager(this);
    QNetworkRequest req;
    req.setUrl(QUrl("http://192.168.10.22/sfis/xny/inspResult/isPassAndSaveToNextOprnExt"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QString soapXML = json_str ;
    QNetworkReply *reply3 = manager3->post(req,byte_array);
    QAbstractSocket::connect(reply3,&QNetworkReply::finished,[=](){
        QByteArray bytes = reply3->readAll();
        qDebug()<<bytes;
        QXmlStreamReader reader(bytes);
      QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array, &json_error);
        QString success,message;
        if(json_error.error == QJsonParseError::NoError)
        {
            qDebug() << "JSON格式正确";
            QJsonObject root_Obj =parse_doucment.object();

            success = root_Obj.value("success").toString();
            message = root_Obj.value("message").toString();
        }
        else
        {
            qDebug() << "JSON格式错误";
        }
        if(success=="false")
        {
            QMessageBox::about(NULL,"提示",message);
        }
        else
        {
            ui->textEdit->insertPlainText("上传成功\r");

        }


    });
    gx=0;
}

void MainWindow::onSocketReadyRead1()
{

    if(clent->bytesAvailable()>=18)
    {
        gx1=0;
        QByteArray temp =clent->readAll();

        QByteArray temp1=temp.right(9).left(5);

        QByteArray temp2=temp.right(17).left(5);

        QByteArray temp3=temp.left(8).right(1);

        qDebug()<<temp;

        if(temp3!="1")
        {
            ui->textEdit->insertPlainText("耐压测试失败\n");

                if(kw3!=0)
                {
                    kw3=kw3-1;
                }
                else
                {
                    nydl.append("ng");
                    ny.append(0);
                }


        }
        else
        {
            ui->textEdit->insertPlainText("耐压测试完成\n");

            if(kw3!=0)
            {
                kw3=kw3-1;
            }
            else
            {
                nydl.append(temp1);
                ny.append(1);
            }
        }

        if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
        {
            id.takeAt(0);
            ny.takeAt(0);
            jycs.takeAt(0);
            jydy.takeAt(0);
            jydz.takeAt(0);
            jydl.takeAt(0);
            jycs2.takeAt(0);
            jydy2.takeAt(0);
            jydz2.takeAt(0);
            jydl2.takeAt(0);
            nydl.takeAt(0);
            xqny.takeAt(0);
            xqnydl.takeAt(0);//数据向前传递与产品对应
            ui->textEdit->insertPlainText("错误数据");
        }
        else if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
        {

            QDateTime local(QDateTime::currentDateTime());
            QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

            QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
            db.setDatabaseName(dbname);
            bool ok =db.open();
            qDebug() << ok;
            QSqlQuery query(db);
            QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
            query.exec(sqlstr);
            query.next();
            qDebug() << query.value(0).toString();
            QString m = query.value(0).toString();
            QString str1="";
            if(jycs.at(0)==0)
            {
                str1+="绝缘1NG ";
            }
            if(jycs2.at(0)==0)
            {
                str1+="触点线圈间绝缘NG";
            }
            if(xqny.at(0)==0)
            {
                str1+="线圈触点间耐压NG ";
            }
            if(ny.at(0)==0)
            {
                str1+="触点间耐压NG";
            }
            if(str1!="")
            {
                accept2();
                bhgs++;
                QString str2=QString::number(bhgs);
                str2 +=localTime+id.at(0)+":"+str1+"/n";
                ui->textEdit_2->insertPlainText(str2);
                QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                if(!file.open(QIODevice::WriteOnly | QFile::Text))
                {

                }
                file.write(str2.toUtf8());
                file.close();
                ui->textEdit->insertPlainText("产品ng");
            }
            else if(m=="")
            {
                /*if(bihe==1)
                    {
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                        qDebug() << sqlstr;
                        query.exec(sqlstr);

                    }
                    else
                    {
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"',null,null,'"+localTime+"',1,1,0);";
                        qDebug() << sqlstr;
                        query.exec(sqlstr);
                    }*/
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                query.exec(sqlstr);
                accept2();
                ui->textEdit->insertPlainText("写入完成\n");
            }
            else
            {

                int a=m.toInt()+1;
                m=QString::number(a);
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";

                accept2();
                query.exec(sqlstr);

                ui->textEdit->insertPlainText("写入完成\n");
            }

            db.close();

        }
        else
        {
            ui->textEdit->insertPlainText("未判定\n"+id.at(0));
        }



    }
}

void MainWindow::onConnected()
{
    ui->textEdit_2->insertPlainText("耐压1上线\r");
}

void MainWindow::onDisconnected()
{
     ui->textEdit_2->insertPlainText("耐压1下线\r");
}

void MainWindow::onSocketReadyRead2()
{

    if(clent2->bytesAvailable()>=18)
    {
        gx2=0;
        QByteArray temp =clent2->readAll();
        QByteArray temp1=temp.right(9).left(5);

        QByteArray temp2=temp.right(17).left(5);

        QByteArray temp3=temp.left(8).right(1);

        qDebug()<<temp;
        if(temp3!="1")
        {
            ui->textEdit->insertPlainText("耐压测试失败\r");

            if(kw4!=0)
            {
                kw4=kw4-1;
            }
            else
            {
                xqnydl.append("ng");
                xqny.append(0);
            }

        }
        else
        {
            ui->textEdit->insertPlainText("耐压测试完成\n");

            if(kw4!=0)
            {
                kw4=kw4-1;
            }
            else
            {
                xqnydl.append(temp1);
                xqny.append(1);
            }

        }
        if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
        {
            id.takeAt(0);
            ny.takeAt(0);
            jycs.takeAt(0);
            jydy.takeAt(0);
            jydz.takeAt(0);
            jydl.takeAt(0);
            jycs2.takeAt(0);
            jydy2.takeAt(0);
            jydz2.takeAt(0);
            jydl2.takeAt(0);
            nydl.takeAt(0);
            xqny.takeAt(0);
            xqnydl.takeAt(0);//数据向前传递与产品对应
            ui->textEdit->insertPlainText("错误数据");
        }
        else if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
        {

            QDateTime local(QDateTime::currentDateTime());
            QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

            QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
            db.setDatabaseName(dbname);
            bool ok =db.open();
            qDebug() << ok;
            QSqlQuery query(db);
            QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
            query.exec(sqlstr);
            query.next();
            qDebug() << query.value(0).toString();
            QString m = query.value(0).toString();
            QString str1="";
            if(jycs.at(0)==0)
            {
                str1+="绝缘1NG ";
            }
            if(jycs2.at(0)==0)
            {
                str1+="触点线圈间绝缘NG";
            }
            if(xqny.at(0)==0)
            {
                str1+="线圈触点间耐压NG ";
            }
            if(ny.at(0)==0)
            {
                str1+="触点间耐压NG";
            }
            if(str1!="")
            {
                accept2();
                bhgs++;
                QString str2=QString::number(bhgs);
                str2 +=localTime+id.at(0)+":"+str1+"/n";
                ui->textEdit_2->insertPlainText(str2);
                QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                if(!file.open(QIODevice::WriteOnly | QFile::Text))
                {

                }
                file.write(str2.toUtf8());
                file.close();
                ui->textEdit->insertPlainText("产品ng");
            }
            else if(m=="")
            {
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                query.exec(sqlstr);
                ui->textEdit->insertPlainText("写入完成\n");
                accept2();
            }
            else
            {
                int a=m.toInt()+1;
                m=QString::number(a);
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";

                accept2();
                query.exec(sqlstr);

                ui->textEdit->insertPlainText("写入完成\n");
            }

            db.close();

        }
        else
        {
            ui->textEdit->insertPlainText("未判定\n"+id.at(0));
        }

    }
}

void MainWindow::onConnected2()
{
    ui->textEdit_2->insertPlainText("耐压2上线\n");
}

void MainWindow::onDisconnected2()
{
     ui->textEdit_2->insertPlainText("耐压2下线\n");
}

void MainWindow::onSocketReadyRead3()
{

    if(clent3->bytesAvailable()>=18)
    {
        gx3=0;
        QByteArray temp =clent3->readAll();
        if(temp.length() < 18)
        {
            temp=temp.right(18);
        }

        QByteArray temp_conclusion=temp.left(16);
        temp_conclusion=temp_conclusion.right(1);
        QByteArray temp_voltage1=temp.left(10);
        temp_voltage1=temp_voltage1.right(2);
        QByteArray temp_current1=temp.left(13);
        temp_current1=temp_current1.right(3);
        QByteArray temp_current=temp.left(6);
        temp_current=temp_current.right(2);

        conclusion(temp_conclusion);
        voltage1(temp_voltage1);
        current1(temp_current1);
        current(temp_current);
        if(gxwc==1)
        {
            ui->textEdit->insertPlainText("绝缘2测试完成\n");
        }
        else
        {
            ui->textEdit->insertPlainText("绝缘2测试失败\n");
        }
        if(gxwc==3)
        {

            if(kw2!=0)
            {

                kw2=kw2-1;
            }
            else
            {
                jydy2.append("ng");
                jydz2.append("ng");
                jycs2.append(0);
                jydl2.append("ng");

            }

            lscc2=2;
            lscc4="";
            lscc5="";
        }
        else
        {

            if(kw2!=0)
            {
                kw2=kw2-1;
            }
            else
            {
                jydl2.append(lscc);
                jydz2.append(lscc4);
                jydy2.append(lscc5);
                jycs2.append(lscc2);
            }

        }
        lscc="";
        lscc2=2;
        lscc4="";
        lscc5="";



        if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
        {
            id.takeAt(0);
            ny.takeAt(0);
            jycs.takeAt(0);
            jydy.takeAt(0);
            jydz.takeAt(0);
            jydl.takeAt(0);
            jycs2.takeAt(0);
            jydy2.takeAt(0);
            jydz2.takeAt(0);
            jydl2.takeAt(0);
            nydl.takeAt(0);
            xqny.takeAt(0);
            xqnydl.takeAt(0);//数据向前传递与产品对应
            ui->textEdit->insertPlainText("错误数据");
        }
        else if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
        {

            QDateTime local(QDateTime::currentDateTime());
            QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

            QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
            db.setDatabaseName(dbname);
            bool ok =db.open();
            qDebug() << ok;
            QSqlQuery query(db);
            QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
            query.exec(sqlstr);
            query.next();
            qDebug() << query.value(0).toString();
            QString m = query.value(0).toString();
            QString str1="";
            if(jycs.at(0)==0)
            {
                str1+="绝缘1NG ";
            }
            if(jycs2.at(0)==0)
            {
                str1+="触点线圈间绝缘NG";
            }
            if(xqny.at(0)==0)
            {
                str1+="线圈触点间耐压NG ";
            }
            if(ny.at(0)==0)
            {
                str1+="触点间耐压NG";
            }
            if(str1!="")
            {
                accept2();
                bhgs++;
                QString str2=QString::number(bhgs);
                str2 +=localTime+id.at(0)+":"+str1+"/n";
                ui->textEdit_2->insertPlainText(str2);
                QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                if(!file.open(QIODevice::WriteOnly | QFile::Text))
                {

                }
                file.write(str2.toUtf8());
                file.close();
                ui->textEdit->insertPlainText("产品ng");
            }
            else if(m=="")
            {

                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                query.exec(sqlstr);
                ui->textEdit->insertPlainText("写入完成\n");
                accept2();
            }
            else
            {

                int a=m.toInt()+1;
                m=QString::number(a);
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";


                query.exec(sqlstr);
                accept2();
                ui->textEdit->insertPlainText("写入完成\n");
            }

            db.close();

        }
        else
        {
            ui->textEdit->insertPlainText("未判定\n"+id.at(0));
        }

    }
    else
    {
        ui->textEdit->insertPlainText("绝缘2回复数据错误\n"+id.at(0));
    }



}

void MainWindow::onConnected3()
{

    ui->textEdit_2->insertPlainText("绝缘2上线\n");
    if(id.length()>0)
    {
        QString msg ="7B 06 00 00 06 7D";

        QByteArray str;
        StringToHex(msg,str);

        if(gx3==1)
        {
            ui->textEdit->insertPlainText("绝缘未读取\n");
            if(kw2!=0)
            {

                kw2=kw2-1;
            }
            else
            {
                jydy2.append("ng");
                jydz2.append("ng");
                jycs2.append(0);
                jydl2.append("ng");

            }
            if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
            {
                id.takeAt(0);
                ny.takeAt(0);
                jycs.takeAt(0);
                jydy.takeAt(0);
                jydz.takeAt(0);
                jydl.takeAt(0);
                jycs2.takeAt(0);
                jydy2.takeAt(0);
                jydz2.takeAt(0);
                jydl2.takeAt(0);
                nydl.takeAt(0);
                xqny.takeAt(0);
                xqnydl.takeAt(0);//数据向前传递与产品对应
                ui->textEdit->insertPlainText("错误数据");
            }
            else if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
            {

                QDateTime local(QDateTime::currentDateTime());
                QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

                QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
                db.setDatabaseName(dbname);
                bool ok =db.open();
                qDebug() << ok;
                QSqlQuery query(db);
                QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
                query.exec(sqlstr);
                query.next();
                qDebug() << query.value(0).toString();
                QString m = query.value(0).toString();
                QString str1="";
                if(jycs.at(0)==0)
                {
                    str1+="绝缘1NG ";
                }
                if(jycs2.at(0)==0)
                {
                    str1+="触点线圈间绝缘NG";
                }
                if(xqny.at(0)==0)
                {
                    str1+="线圈触点间耐压NG ";
                }
                if(ny.at(0)==0)
                {
                    str1+="触点间耐压NG";
                }
                if(str1!="")
                {
                    accept2();
                    bhgs++;
                    QString str2=QString::number(bhgs);
                    str2 +=localTime+id.at(0)+":"+str1+"/n";
                    ui->textEdit_2->insertPlainText(str2);
                    QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                    if(!file.open(QIODevice::WriteOnly | QFile::Text))
                    {

                    }
                    file.write(str2.toUtf8());
                    file.close();
                    ui->textEdit->insertPlainText("产品ng");
                }
                else if(m=="")
                {

                    query.clear();
                    sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                    query.exec(sqlstr);
                    ui->textEdit->insertPlainText("写入完成\n");
                    accept2();
                }
                else
                {

                    int a=m.toInt()+1;
                    m=QString::number(a);
                    query.clear();
                    sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";


                    query.exec(sqlstr);
                    accept2();
                    ui->textEdit->insertPlainText("写入完成\n");
                }

                db.close();

            }
        }
        gx3=1;
        clent3->write(str);
    }
}

void MainWindow::onDisconnected3()
{
     ui->textEdit_2->insertPlainText("绝缘2下线\n");
}

void MainWindow::onSocketReadyRead4()
{
    if(clent4->bytesAvailable()>=18)
    {
        gx4=0;
        QByteArray temp =clent4->readAll();

        if(temp.length() < 18)
        {
            temp=temp.right(18);
        }

        QByteArray temp_conclusion=temp.left(16);
        temp_conclusion=temp_conclusion.right(1);
        QByteArray temp_voltage1=temp.left(10);
        temp_voltage1=temp_voltage1.right(2);
        QByteArray temp_current1=temp.left(13);
        temp_current1=temp_current1.right(3);
        QByteArray temp_current=temp.left(6);
        temp_current=temp_current.right(2);

        conclusion(temp_conclusion);
        voltage1(temp_voltage1);
        current1(temp_current1);
        current(temp_current);
        if(gxwc==1)
        {
            ui->textEdit->insertPlainText("绝缘1测试完成\n");
        }
        else
        {
            ui->textEdit->insertPlainText("绝缘1测试失败\n");
        }
        if(gxwc==3)
        {

            if(kw1!=0)
            {

                kw1=kw1-1;
            }
            else
            {
                jydz.append("ng");
                jydy.append("ng");
                jycs.append(0);
                jydl.append("ng");
            }



            lscc2=2;
            lscc4="";
            lscc5="";
        }
        else
        {

            if(kw1!=0)
            {
                kw1=kw1-1;
            }
            else
            {
                jydz.append(lscc4);
                jydy.append(lscc5);
                jycs.append(lscc2);
                jydl.append(lscc);
            }

            lscc="";
            lscc2=2;
            lscc4="";
            lscc5="";


        }

        if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
        {
            id.takeAt(0);
            ny.takeAt(0);
            jycs.takeAt(0);
            jydy.takeAt(0);
            jydz.takeAt(0);
            jydl.takeAt(0);
            jycs2.takeAt(0);
            jydy2.takeAt(0);
            jydz2.takeAt(0);
            jydl2.takeAt(0);
            nydl.takeAt(0);
            xqny.takeAt(0);
            xqnydl.takeAt(0);//数据向前传递与产品对应
            ui->textEdit->insertPlainText("错误数据");
            //if(id.length()!=jycs.length()+1 or id.length()!=jycs2.length()+2 or id.length()!=ny.length()+3 )
        }
        else if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
        {

            QDateTime local(QDateTime::currentDateTime());
            QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

            QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
            db.setDatabaseName(dbname);
            bool ok =db.open();
            qDebug() << ok;
            QSqlQuery query(db);
            QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
            query.exec(sqlstr);
            query.next();
            qDebug() << query.value(0).toString();
            QString m = query.value(0).toString();
            QString str1="";
            if(jycs.at(0)==0)
            {
                str1+="绝缘1NG ";
            }
            if(jycs2.at(0)==0)
            {
                str1+="触点线圈间绝缘NG";
            }
            if(xqny.at(0)==0)
            {
                str1+="线圈触点间耐压NG ";
            }
            if(ny.at(0)==0)
            {
                str1+="触点间耐压NG";
            }
            if(str1!="")
            {
                accept2();
                bhgs++;
                QString str2=QString::number(bhgs);
                str2 +=localTime+id.at(0)+":"+str1+"/n";
                ui->textEdit_2->insertPlainText(str2);
                QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                if(!file.open(QIODevice::WriteOnly | QFile::Text))
                {

                }
                file.write(str2.toUtf8());
                file.close();
                ui->textEdit->insertPlainText("产品ng");
            }
            else if(m=="")
            {
                /*if(bihe==1)
                    {
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                        qDebug() << sqlstr;
                        query.exec(sqlstr);

                    }
                    else
                    {
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"',null,null,'"+localTime+"',1,1,0);";
                        qDebug() << sqlstr;
                        query.exec(sqlstr);
                    }*/
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                query.exec(sqlstr);
                ui->textEdit->insertPlainText("写入完成\n");
                accept2();
            }
            else
            {/*
                if(bihe==1)
                    {
                        int a=m.toInt()+1;
                        m=QString::number(a);
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                        query.exec(sqlstr);
                    }
                    else
                    {
                        int a=m.toInt()+1;
                        m=QString::number(a);
                        query.clear();
                        sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"','"+QString::number( xqny.at(0))+"','"+xqnydl.at(0)+"',null,null'"+localTime+"',1,1,0);";
                        query.exec(sqlstr);
                    }*/

                int a=m.toInt()+1;
                m=QString::number(a);
                query.clear();
                sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";


                query.exec(sqlstr);
                accept2();
                ui->textEdit->insertPlainText("写入完成\n");
            }

            db.close();

        }
        else
        {
            ui->textEdit->insertPlainText("未判定\n"+id.at(0));
        }

    }
    else
    {
        ui->textEdit->insertPlainText("绝缘1回复数据错误\n"+id.at(0));
    }

}

void MainWindow::onConnected4()
{
    ui->textEdit_2->insertPlainText("绝缘1上线\n");

    if(id.length()>0)
    {
        QString msg ="7B 06 00 00 06 7D";

        QByteArray str;
        StringToHex(msg,str);

        if(gx4==1)
        {
            ui->textEdit->insertPlainText("绝缘未读取\n");
            if(kw1!=0)
            {

                kw1=kw1-1;
            }
            else
            {
                jydz.append("ng");
                jydy.append("ng");
                jycs.append(0);
                jydl.append("ng");
            }
            if(ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")!=-1)
            {
                id.takeAt(0);
                ny.takeAt(0);
                jycs.takeAt(0);
                jydy.takeAt(0);
                jydz.takeAt(0);
                jydl.takeAt(0);
                jycs2.takeAt(0);
                jydy2.takeAt(0);
                jydz2.takeAt(0);
                jydl2.takeAt(0);
                nydl.takeAt(0);
                xqny.takeAt(0);
                xqnydl.takeAt(0);//数据向前传递与产品对应
                ui->textEdit->insertPlainText("错误数据");
            }
            else if( ny.length()>1 and xqny.length()>0 and jycs.length()>3 and jycs2.length()>2 and id.at(0).indexOf("ERROR")==-1)//产品完成三道工序后将数据存储到数据库
            {

                QDateTime local(QDateTime::currentDateTime());
                QString localTime = local.toString("yyyy-MM-dd hh:mm:ss");//启动时读取当天日期

                QString dbname = QString("DRIVER={Microsoft Access Driver (*.mdb)}; FIL={MS Access};DBQ="+dayss);
                db.setDatabaseName(dbname);
                bool ok =db.open();
                qDebug() << ok;
                QSqlQuery query(db);
                QString sqlstr = "select  top 1  ID from insp_result  order by  ID  desc ;";
                query.exec(sqlstr);
                query.next();
                qDebug() << query.value(0).toString();
                QString m = query.value(0).toString();
                QString str1="";
                if(jycs.at(0)==0)
                {
                    str1+="绝缘1NG ";
                }
                if(jycs2.at(0)==0)
                {
                    str1+="触点线圈间绝缘NG";
                }
                if(xqny.at(0)==0)
                {
                    str1+="线圈触点间耐压NG ";
                }
                if(ny.at(0)==0)
                {
                    str1+="触点间耐压NG";
                }
                if(str1!="")
                {
                    accept2();
                    bhgs++;
                    QString str2=QString::number(bhgs);
                    str2 +=localTime+id.at(0)+":"+str1+"/n";
                    ui->textEdit_2->insertPlainText(str2);
                    QFile file("D:\\NG记录\\"+localTime.left(10)+".txt");
                    if(!file.open(QIODevice::WriteOnly | QFile::Text))
                    {

                    }
                    file.write(str2.toUtf8());
                    file.close();
                    ui->textEdit->insertPlainText("产品ng");
                }
                else if(m=="")
                {
                    /*if(bihe==1)
                        {
                            query.clear();
                            sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                            qDebug() << sqlstr;
                            query.exec(sqlstr);

                        }
                        else
                        {
                            query.clear();
                            sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"',null,null,'"+localTime+"',1,1,0);";
                            qDebug() << sqlstr;
                            query.exec(sqlstr);
                        }*/
                    query.clear();
                    sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values(1,'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                    query.exec(sqlstr);
                    ui->textEdit->insertPlainText("写入完成\n");
                    accept2();
                }
                else
                {/*
                    if(bihe==1)
                        {
                            int a=m.toInt()+1;
                            m=QString::number(a);
                            query.clear();
                            sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"',null,null,"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";
                            query.exec(sqlstr);
                        }
                        else
                        {
                            int a=m.toInt()+1;
                            m=QString::number(a);
                            query.clear();
                            sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',1,"+QString::number( ny.at(0))+",'"+nydl.at(0)+"','"+QString::number( xqny.at(0))+"','"+xqnydl.at(0)+"',null,null'"+localTime+"',1,1,0);";
                            query.exec(sqlstr);
                        }*/

                    int a=m.toInt()+1;
                    m=QString::number(a);
                    query.clear();
                    sqlstr = "insert into insp_result(ID,产品条码,绝缘电阻OK,绝缘电压,绝缘电阻值,绝缘电阻漏电流,触点线圈间绝缘OK,触点线圈间绝缘电压,触点线圈间绝缘电阻值,触点线圈间绝缘漏电流,触点间耐压OK,触点间耐压漏电流,线圈_触点间介质耐压OK,[触点线圈间耐压漏电流（断开）],创建时间,测试结果,记录完毕,上传状态) values("+m+",'"+id.at(0)+"',"+QString::number(jycs.at(0))+","+jydy.at(0)+","+jydz.at(0)+","+jydl.at(0)+","+QString::number(jycs2.at(0))+","+jydy2.at(0)+","+jydz2.at(0)+","+jydl2.at(0)+","+QString::number( ny.at(0))+",'"+nydl.at(0)+"',"+QString::number( xqny.at(0))+",'"+xqnydl.at(0)+"','"+localTime+"',1,1,0);";


                    query.exec(sqlstr);
                    accept2();
                    ui->textEdit->insertPlainText("写入完成\n");
                }

                db.close();

            }
        }
        gx4=1;
        clent4->write(str);

    }
}

void MainWindow::onDisconnected4()
{
     ui->textEdit_2->insertPlainText("绝缘1下线\n");
}

void MainWindow::onSocketReadyRead5()
{
    QByteArray temp =clent5->readAll();

    id.append(temp);


        ui->textEdit->clear();

    ui->textEdit->insertPlainText("条码读取完成"+temp+"\n");
    if(temp.indexOf("ERROR")==-1  )
    {
        _sleep(sd);
    }
    else
    {
        _sleep(sd-500);
    }
    accept();
}

void MainWindow::onConnected5()
{
    ui->textEdit_2->insertPlainText("扫码1上线\n");
}

void MainWindow::onDisconnected5()
{
     ui->textEdit_2->insertPlainText("扫码1下线\n");
}

void MainWindow::onSocketReadyRead6()
{
    QByteArray temp =clent6->readAll();
    id.append(temp);

    ui->textEdit->clear();

    ui->textEdit->insertPlainText("条码读取完成"+temp+"\n");
    if(temp.indexOf("ERROR")==-1  )
    {
        _sleep(sd);
    }
    else
    {
        _sleep(sd-500);
    }
    accept();
}

void MainWindow::onConnected6()
{
    ui->textEdit_2->insertPlainText("扫码2上线\n");
}

void MainWindow::onDisconnected6()
{
     ui->textEdit_2->insertPlainText("扫码2下线\n");
}


void MainWindow::onConnected7()
{

}

void MainWindow::onDisconnected7()
{

}

void MainWindow::onSocketReadyRead7()
{
    QByteArray temp=clent7->readAll();
    if(temp=="1")
    {
        QString msg ="02 || "+id.at(id.length()-1)+"||||0057-1="+QString::number( sny)+"'0||||0D";

        QByteArray str=msg.toUtf8();

        serial2.write(str);
    }
    else
    {
        QString msg ="02 || "+id.at(id.length()-1)+"||||0057-1="+QString::number( sny)+"'0||||0D";

        QByteArray str=msg.toUtf8();

        serial2.write(str);
    }
}
