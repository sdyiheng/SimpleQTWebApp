#ifndef MODBUS_DATAACQUIRE_H
#define MODBUS_DATAACQUIRE_H

#include <QtCore>
#include <QtSerialBus/QModbusRtuSerialMaster>
#include <QtWebSockets/QWebSocketServer>
#include "modbus_dataacquire_controller.h"

class Modbus_DataAcquire : public QObject
{
    Q_OBJECT

public:

    explicit Modbus_DataAcquire(const QSettings* settings, const Modbus_DataAcquire_Controller* controller, QObject *parent = nullptr);

    void setCounter(long _leftCounter)
    {
        this->leftCounter = _leftCounter;
    }

    long getCounter()
    {
        return this->leftCounter;
    }

    void init();

public slots:
    void start(int leftCounter);
    void stop();

public slots:
    void onReplayFinished();
    void onNewWebSocketConnection();
    void socketDisconnected();

private:
    /** Configuration settings */
    const QSettings* settings;
    long leftCounter;
    QModbusRtuSerialMaster* modbusClient;
    QWebSocketServer* websocketserver;
    QList<QWebSocket*> websocketConnections;
    QModbusDataUnit* unit;
    const Modbus_DataAcquire_Controller* controller;

    bool isDone()
    {
        return this->leftCounter == 0;
    }
    void counterDown(){
        if(leftCounter>0)
            leftCounter--;
    }

    void doDataAcquire();






};

#endif // MODBUS_DATAACQUIRE_H
