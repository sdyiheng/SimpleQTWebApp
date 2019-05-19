#ifndef MODBUS_DATAACQUIRE_H
#define MODBUS_DATAACQUIRE_H

#include <QtCore>
#include <QtSerialBus/QModbusRtuSerialMaster>
#include <QtWebSockets/QWebSocketServer>

class Modbus_DataAcquire : public QObject
{
    Q_OBJECT

public:

    explicit Modbus_DataAcquire(const QSettings* settings, QObject *parent = nullptr);

    void setCounter(long _leftCounter)
    {
        this->leftCounter = _leftCounter;
    }

    long getCounter()
    {
        return this->leftCounter;
    }

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
