#ifndef MODBUS_DATAACQUIRE_THREAD_H
#define MODBUS_DATAACQUIRE_THREAD_H

#include <QThread>

class Modbus_DataAcquire_Controller;

class Modbus_DataAcquire_Thread:public QObject
{
    Q_OBJECT

private:
    Modbus_DataAcquire_Controller *controller;

public:
    Modbus_DataAcquire_Thread(Modbus_DataAcquire_Controller *controller);
};

#endif // MODBUS_DATAACQUIRE_THREAD_H
