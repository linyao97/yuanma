#include "testthread.h"
#include <QDebug>
#include <QSerialPort>
extern  QSerialPort serial;
testthread::testthread()
{
    flagRunning = true;
}

void testthread::run()
{

    flagRunning = true;
    mutex.lock();
    while(flagRunning)
    {
        mutex.unlock();
        emit send();
        msleep(100);
        mutex.lock();
    }
    mutex.unlock();
}

void testthread::stop()
{
    mutex.lock();
    //flagRunning = false;
    //mutex.unlock();
}

void testthread::pause()
{
    mutex.unlock();
}

void testthread::StringToHex(QString str, QByteArray &senddata)
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
char testthread::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
}
