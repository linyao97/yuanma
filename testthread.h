#ifndef TESTTHREAD_H
#define TESTTHREAD_H
#include <QString>
#include <QThread>
#include <QMutex>

class testthread:public QThread
{
     Q_OBJECT
public:
    testthread();
    virtual void run();
    void stop();
    void pause();
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
signals:
    void send();
private:
    bool flagRunning;
    QMutex mutex;
    int count;
};

#endif // TESTTHREAD_H
