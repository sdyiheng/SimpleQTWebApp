#ifndef MODBUS_DATAACQUIRE_THREAD_H
#define MODBUS_DATAACQUIRE_THREAD_H

#include <QThread>
#include <QtSerialBus/QModbusRtuSerialMaster>


class Modbus_DataAcquire_Thread: public QThread{
    Q_OBJECT

private :
    long counter;
    bool isDone();
    void doDataAcquire(QModbusRtuSerialMaster*);

public:
    void setCounter(long _counter)
    {
        this->counter = _counter;
    }

    long getCounter()
    {
        return this->counter;
    }

    explicit Modbus_DataAcquire_Thread(QObject *parent = nullptr)
            : QThread(parent)
        {
            //qDebug() << "Worker Thread : " << QThread::currentThreadId();
        }

protected:
    void run();

};

#endif // MODBUS_DATAACQUIRE_THREAD_H
