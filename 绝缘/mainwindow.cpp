#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QtNetwork>
#include <qdebug.h>


int dq=0;
int qr=0;
int qt=0;
int msec=3000;
int jy=0;
int m=0,kw1=0,kw2=0,kw3=0,bihe=0,gongxu1=0,gongxu2=0,gongxu3=0,lscc2,lscc3,gxwc=0,bhgs=0,idy=0,kaishi=0;
QString id1,id2,id3,id4,id5,lscc;
QSqlDatabase db ;
QList<QString> nydl,xqnydl,id;
QList<int> ny,xqny,jycs;
QString days,dayss;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    test_thread = new testthread();
    m_pTcpServer = new QTcpServer(this);
    ui->comboBox->addItem("绝缘电阻");
    ui->comboBox->addItem("触点间耐压");
    ui->comboBox->addItem("线圈-触点间耐压");
    ui->comboBox_2->addItem("绝缘电阻");
    ui->comboBox_2->addItem("触点间耐压");
    ui->comboBox_2->addItem("线圈-触点间耐压");
    ui->comboBox_3->addItem("绝缘电阻");
    ui->comboBox_3->addItem("触点间耐压");
    ui->comboBox_3->addItem("线圈-触点间耐压");

    connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    connect(this,SIGNAL(fetchTimesignals()),this,SLOT(fetchTime()));
    connect(&serial,&QSerialPort::readyRead,this,&MainWindow::serialPort_readyRead);

    QObject::connect(test_thread, SIGNAL(send()), this, SLOT(accept()));

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
        QString  sqlstr = "create table insp_result(ID int,绝缘电阻 varchar(255),触点间耐压 varchar(255),触点间耐压漏电流 varchar(255),[线圈-触点间耐压（断开）]varchar(255),[触点线圈间耐压漏电流（断开）] varchar(255),[线圈-触点间耐压（闭合）] varchar(255),[触点线圈间耐压漏电流（闭合）] varchar(255))";
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
        if(i==1)
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
            ui->comboBox->setCurrentIndex(line.toInt());
        }
        if(i==5)
        {
            ui->comboBox_2->setCurrentIndex(line.toInt());
        }
        if(i==6)
        {
            ui->comboBox_3->setCurrentIndex(line.toInt());
        }
        if(i==7)
        {
            if(line=="1")
            {
                ui->radioButton->setChecked(true);
            }
            else
            {
                ui->radioButton_2->setChecked(true);
            }
        }
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::fetchTime()
{
    on_pushButton_5_clicked();
    QString msg ="7B 06 00 04 0A 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    qt=1;
    QByteArray str;
    StringToHex(msg,str);
    m_pTcpSocket->write(str);
}

void MainWindow::accept()
{
    QString msg ="7B 06 00 00 06 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    dq=1;
    QByteArray str;
    StringToHex(msg,str);
    m_pTcpSocket->write(str);
}

void MainWindow::serialPort_readyRead()
{
    QByteArray temp =serial.read(18);
    QDataStream out(&temp,QIODevice::ReadWrite);
    QString str,a;
    qDebug()<<temp;
    while(!out.atEnd())
    {
        qint8 outChar = 0;
        out>>outChar;   //每字节填充一次，直到结束
        //十六进制的转换
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
        a += str;

    }

    QByteArray b =QByteArray::fromHex(QString(a).toUtf8());
    id.append(b);
    idy=1;
    if(kaishi==0)
    {
        test_thread->start();
        kaishi=1;
    }



}
void MainWindow::on_pushButton_clicked()
{
    QString IP="192.168.0.201";
    quint16 port=8234;
    QHostAddress addr(IP);
    m_pTcpServer->listen(addr,port);
    ui->textEdit->insertPlainText("**开始监听");
    ui->textEdit->insertPlainText("**服务器地址:"+m_pTcpServer->serverAddress().toString());
    ui->textEdit->insertPlainText("**服务器端口"+QString::number(m_pTcpServer->serverPort()));
    serial.setPortName("com1");//扫码枪一
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    if(!serial.open(QIODevice::ReadOnly))
    {
        QMessageBox::about(NULL,"提示","无法连接扫码枪一");
    }
    else {
    serial.setDataTerminalReady(true);
    }
    serial.clear();

}

void MainWindow::Sleep(int i)
{
    QTime dieTime = QTime::currentTime().addMSecs(i);
    while( QTime::currentTime() < dieTime )
    {QCoreApplication::processEvents(QEventLoop::AllEvents, 100);}
    QString msg ="7B 06 00 00 06 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    dq=1;
    QByteArray str;
    StringToHex(msg,str);
    m_pTcpSocket->write(str);
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
    QString msg ="7B 06 00 02 08 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    qr=1;
    QByteArray str;
    StringToHex(msg,str);
    m_pTcpSocket->write(str);
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
    ui->textEdit_2->setText(a);
    lscc=QString::number(i);

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
        lscc2=0;
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

    if(dq==1)//工序数据读取及保存
    {
        //QByteArray temp =m_pTcpSocket->readAll();
        for (int i = 0;i < socket_list.length();i ++)
        {
            QByteArray temp =socket_list.at(i)->readAll();
            QString c=socket_list.at(i)->peerAddress().toString();
            ui->textEdit_2->insertPlainText(c);
            QByteArray temp_current=temp.left(6);
            temp_current=temp_current.right(2);
            QByteArray temp_conclusion=temp.left(16);
            temp_conclusion=temp_conclusion.right(1);
            QDataStream out(&temp,QIODevice::ReadWrite);
            QString str,a;
            while(!out.atEnd())
            {
                qint8 outChar = 0;
                out>>outChar;   //每字节填充一次，直到结束
                //十六进制的转换
                str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
                a +=str;
                a += " ";
            }
            if(a=="7b064f4ba07d")
            {
                ui->textEdit->insertPlainText("[IN]:命令已发送\n");
            }
            else if(a=="7b064e4fa37d")
            {
                ui->textEdit->insertPlainText("[IN]:命令错误\n");
            }

            a += "\n";
            ui->textEdit->insertPlainText("[IN]"+a);
            current(temp_current);
            conclusion(temp_conclusion);

            if (c=="192.168.0.7" and gxwc==1 and id.length()!=0)//触点间耐压判断是否到位及保存数据
            {
                if(gongxu1==1)
                {
                    if(kw1!=0)
                    {
                        kw1=kw1-1;
                    }
                    else
                    {
                        nydl.append(lscc);
                        ny.append(lscc3);
                    }
                }
                else if(gongxu2==1)
                {
                    if(kw2!=0)
                    {
                        kw2=kw2-1;
                    }
                    else
                    {
                        nydl.append(lscc);
                        ny.append(lscc3);
                    }
                }
                else
                {
                    if(kw3!=0)
                    {
                        kw3=kw3-1;
                    }
                    else
                    {
                        nydl.append(lscc);
                        ny.append(lscc3);
                    }
                }
                lscc2=2;
                lscc="";
                lscc3=2;
            }
            else if(c=="192.168.0.8" and gxwc==1 and id.length()!=0)//线圈触点间耐压判断是否到位及保存数据
            {
                if(gongxu1==2)
                {
                    if(kw1!=0)
                    {
                        kw1=kw1-1;
                    }
                    else
                    {
                        xqnydl.append(lscc);
                        xqny.append(lscc3);
                    }
                }
                else if(gongxu2==2)
                {
                    if(kw2!=0)
                    {
                        kw2=kw2-1;
                    }
                    else
                    {
                        xqnydl.append(lscc);
                        xqny.append(lscc3);
                    }
                }
                else
                {
                    if(kw3!=0)
                    {
                        kw3=kw3-1;
                    }
                    else
                    {
                        xqnydl.append(lscc);
                        xqny.append(lscc3);
                    }
                }
                lscc2=2;
                lscc="";
                lscc3=2;

            }
            else if(c=="192.168.0.9" and gxwc==1 and id.length()!=0)//绝缘测试判断是否到位及保存数据
            {
                if(gongxu1==2)
                {
                    if(kw1!=0)
                    {
                        kw1=kw1-1;
                    }
                    else
                    {
                        jycs.append(lscc2);
                    }
                }
                else if(gongxu2==2)
                {
                    if(kw2!=0)
                    {
                        kw2=kw2-1;
                    }
                    else
                    {
                        jycs.append(lscc2);
                    }
                }
                else
                {
                    if(kw3!=0)
                    {
                        kw3=kw3-1;
                    }
                    else
                    {
                        jycs.append(lscc2);
                    }
                }
                lscc2=2;
                lscc="";
                lscc3=2;
            }

            if(idy==0 and i==2 and id.length()!=0 and gxwc==1)
            {
                id.append("null");
            }
            else
            {
                idy=0;
            }
            if(id.at(0)=="null" and ny.length()!=0 and xqny.length()!=0 and jycs.length()!=0)
            {
                id.takeAt(0);
                jycs.takeAt(0);
                ny.takeAt(0);
                nydl.takeAt(0);
                xqny.takeAt(0);
                xqnydl.takeAt(0);//数据向前传递与产品对应
            }
            else if(id.at(0)!="null" and ny.length()!=0 and xqny.length()!=0 and jycs.length()!=0)//产品完成三道工序后将数据存储到数据库
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
                    str1+="绝缘NG ";
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
                    bhgs++;
                    QString str2=QString::number(bhgs);
                    str2 +="、"+id1+":"+str1+"/n";
                    ui->textEdit_2->insertPlainText(str2);
                    QFile file("D:\\NG记录\\"+localTime+".txt");
                    if(!file.open(QIODevice::WriteOnly | QFile::Text))
                    {

                    }
                    file.write(str2.toUtf8());
                    file.close();
                }
                if(m=="")
                {
                    if(bihe==1)
                    {
                        query.clear();
                        sqlstr = "insert into insp_result(ID,条码,绝缘电阻,触点间耐压,触点间耐压漏电流,[线圈-触点间耐压（断开）],[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态]) values(1,'"+id.at(0)+"','"+jycs.at(0)+"','"+ny.at(0)+"','"+nydl.at(0)+"',null,null,'"+xqny.at(0)+"','"+xqnydl.at(0)+"','"+localTime+"','ok',1,0);";
                        qDebug() << sqlstr;
                        query.exec(sqlstr);

                    }
                    else
                    {
                        query.clear();
                        sqlstr = "insert into insp_result(ID,条码,绝缘电阻,触点间耐压,触点间耐压漏电流,[线圈-触点间耐压（断开）],[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态]) values(1,'"+id.at(0)+"','"+jycs.at(0)+"','"+ny.at(0)+"','"+nydl.at(0)+"','"+xqny.at(0)+"','"+xqnydl.at(0)+"',null,null'"+localTime+"','ok',1,0);";
                        qDebug() << sqlstr;
                        query.exec(sqlstr);
                    }
                }
                else
                {
                    if(bihe==1)
                    {
                        int a=m.toInt()+1;
                        m=QString::number(a);
                        query.clear();
                        sqlstr = "insert into insp_result(ID,条码,绝缘电阻,触点间耐压,触点间耐压漏电流,[线圈-触点间耐压（断开）],[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态]) values("+m+",'"+id.at(0)+"','"+jycs.at(0)+"','"+ny.at(0)+"','"+nydl.at(0)+"','"+xqny.at(0)+"','"+xqnydl.at(0)+"',null,null'"+localTime+"','ok',1,0);";
                        query.exec(sqlstr);
                    }
                    else
                    {
                        int a=m.toInt()+1;
                        m=QString::number(a);
                        query.clear();
                        sqlstr = "insert into insp_result(ID,条码,绝缘电阻,触点间耐压,触点间耐压漏电流,[线圈-触点间耐压（断开）],[触点线圈间耐压漏电流（断开）],[线圈-触点间耐压（闭合）],[触点线圈间耐压漏电流（闭合）],创建时间,测试结果,记录完毕,上传状态]) values("+m+",'"+id.at(0)+"','"+jycs.at(0)+"','"+ny.at(0)+"','"+nydl.at(0)+"','"+xqny.at(0)+"','"+xqnydl.at(0)+"',null,null'"+localTime+"','ok',1,0);";
                        query.exec(sqlstr);
                    }

                }

                db.close();
                id.takeAt(0);
                jycs.takeAt(0);
                ny.takeAt(0);
                nydl.takeAt(0);
                xqny.takeAt(0);
                xqnydl.takeAt(0);//数据向前传递与产品对应
            }
            dq=0;

        }
    }
    else if(qr==1)
    {
        if(m_pTcpSocket->bytesAvailable()>=6)
        {
            QByteArray temp =m_pTcpSocket->readAll();
            QDataStream out(&temp,QIODevice::ReadWrite);
            QString str,a;
            qr=0;
            while(!out.atEnd())
            {
                qint8 outChar = 0;
                out>>outChar;   //每字节填充一次，直到结束
                //十六进制的转换
                str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
                a +=str;
                //a += " ";
            }
            ui->textEdit->insertPlainText(a);
            if(a=="7b064f4ba07d")
            {
                a="[IN]:命令已发送\n";
                ui->textEdit->insertPlainText(a);
            }
            else if(a=="7b064e4fa37d")
            {
                ui->textEdit->insertPlainText("[IN]:命令错误\n");
            }

        }


    }
    else if(qt==1)
    {

            QByteArray temp =m_pTcpSocket->readAll();
            QDataStream out(&temp,QIODevice::ReadWrite);
            QString str,a;
            qt=0;
            while(!out.atEnd())
            {
                qint8 outChar = 0;
                out>>outChar;   //每字节填充一次，直到结束
                //十六进制的转换
                str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
                a +=str;
                //a += " ";
            }
            ui->textEdit->insertPlainText(a);

            QString b=a.left(20);
            b=b.right(4);
            bool ok;
            int i=b.toInt(&ok,16);
            b=QString("%1").arg(i,4,10,QLatin1Char('0'));
            msec = b.toInt()*100;

            b=a.left(26);
            b=b.right(4);
            i=b.toInt(&ok,16);
            b=QString("%1").arg(i,4,10,QLatin1Char('0'));
            msec += b.toInt()*100;
            b=a.left(30);
            b=b.right(4);


            i=b.toInt(&ok,16);
            b=QString("%1").arg(i,4,10,QLatin1Char('0'));
            msec += b.toInt()*100;
            b=b.setNum(msec);
            ui->textEdit->insertPlainText("\n"+b);

            if (socket_list.length()>=2)
            {
                test_thread->start();
            }

    }
    else
    {
        QByteArray temp =m_pTcpSocket->readAll();
        QDataStream out(&temp,QIODevice::ReadWrite);
        QString str,a;
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;   //每字节填充一次，直到结束
            //十六进制的转换
            str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
            a +=str;
            a += " ";
        }
        ui->textEdit->insertPlainText(a+"\n");

        if(a=="00 01 ff fe ")
        {
            emit fetchTimesignals();

        }

    }
}

void MainWindow::onNewConnection()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    socket_list.append(m_pTcpSocket);
    qDebug()<<socket_list;
    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(onClientConnected()));
    onClientConnected();
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(onClientDisconnected()));
    connect(m_pTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(m_pTcpSocket->state());
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
}

void MainWindow::on_pushButton_2_clicked()
{
    if(m_pTcpServer->isListening())
    {
        m_pTcpServer->close();
        ui->textEdit->insertPlainText("停止监听");
    }
    kaishi=0;
    test_thread->quit();
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
    QString msg ="7B 06 00 01 07 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    qr=1;
    QByteArray str;
    StringToHex(msg,str);
    m_pTcpSocket->write(str);
    for (int i = 0;i < socket_list.length();i ++)
    {
    socket_list.at(i)->write(str);
    }
    Sleep(msec+1000);
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
    if(ui->comboBox->currentText()=="")
    {
        gongxu1=1;
    }
    else if(ui->comboBox->currentText()=="")
    {
        gongxu1=2;
    }
    else
    {
        gongxu1=3;
    }

    if(ui->comboBox_2->currentText()=="")
    {
        gongxu2=1;
    }
    else if(ui->comboBox_2->currentText()=="")
    {
        gongxu2=2;
    }
    else
    {
        gongxu3=3;
    }

    if(ui->comboBox_3->currentText()=="")
    {
        gongxu3=1;
    }
    else if(ui->comboBox_3->currentText()=="")
    {
        gongxu3=2;
    }
    else
    {
        gongxu3=3;
    }

    if(ui->radioButton->isChecked())
    {
        bihe=1;
    }
    else if(ui->radioButton_2->isChecked())
    {
        bihe=0;
    }

    kw1 = ui->spinBox->text().toInt();   //三个工序空位
    kw2 = ui->spinBox_2->text().toInt()+kw1;
    kw3 = ui->spinBox_3->text().toInt()+kw2;

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

    str=bihe;
    file.write(str.toUtf8());
    file.write("\n");


    file.close();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString msg ="7B 06 00 02 08 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    qr=1;
    QByteArray str;
    StringToHex(msg,str);
    //m_pTcpSocket->write(str);
    for (int i = 0;i < socket_list.length();i ++)
    {
        socket_list.at(i)->write(str);

    }
}

void MainWindow::on_pushButton_6_clicked()
{
    QString msg ="7B 06 00 00 06 7D";
    ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");
    dq=1;
    QByteArray str;
    StringToHex(msg,str);
    m_pTcpSocket->write(str);
}

void MainWindow::on_pushButton_7_clicked()
{
    if(ui->comboBox->currentText()=="耐压测试模式")
    {
        QString msg ="7B 07 00 03 00 0A 7D";
        ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");

        QByteArray str;
        StringToHex(msg,str);
        m_pTcpSocket->write(str);

        m=0;

    }
    else if(ui->comboBox->currentText()=="绝缘测试模式")
    {
        QString msg ="7B 07 00 03 01 0B 7D";
        ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");

        QByteArray str;
        StringToHex(msg,str);
        m_pTcpSocket->write(str);

        m=1;
    }
    qr=1;
}

void MainWindow::on_pushButton_8_clicked()
{
    if(ui->comboBox_2->currentText()=="通讯启动")
    {
        QString msg ="7B 07 00 08 00 0F 7D";
        ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");

        QByteArray str;
        StringToHex(msg,str);
        m_pTcpSocket->write(str);
    }
    else if(ui->comboBox_2->currentText()=="plc遥控")
    {
        QString msg ="7B 07 00 08 01 10 7D";
        ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");

        QByteArray str;
        StringToHex(msg,str);
        m_pTcpSocket->write(str);
    }
    else if(ui->comboBox_2->currentText()=="按键启动")
    {
        QString msg ="7B 07 00 08 02 11 7D";
        ui->textEdit->insertPlainText("[SERVER]"+msg+"\n");

        QByteArray str;
        StringToHex(msg,str);
        m_pTcpSocket->write(str);
    }
    qr=1;
}

