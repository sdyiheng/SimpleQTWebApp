#include "modbus_dataacquire.h"
#include <QtSerialPort/QSerialPort>
#include <QtCore>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

Modbus_DataAcquire::Modbus_DataAcquire(const QSettings* settings, QObject *parent) : QObject(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    this->settings = settings;

    this->modbusClient = new QModbusRtuSerialMaster(parent);
    QString port = settings->value("port").toString();

    qDebug()<<"Port:"<<port<<endl;

    //ModBus通信采用RTU传输模式，波特率115200，8数据位，1停止位，奇偶校验无。
    this->modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter,port);//获取串口端口,下面设置各种参数
    this->modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::NoParity);//无较验
    this->modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,QSerialPort::Baud115200);//波特率为115200
    this->modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data8);//数据位为8位
    this->modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,QSerialPort::OneStop);//停止位为1位
    this->modbusClient->setTimeout(1000);//连接超时1S
    this->modbusClient->setNumberOfRetries(3);//连接失败重试三次连接

    //this->modbusClient->connectDevice();

    //websocket连接侦听
    websocketserver = new QWebSocketServer("Server", QWebSocketServer::NonSecureMode);
    connect(websocketserver,SIGNAL(newConnection()),this,SLOT(onNewWebSocketConnection()));
    websocketserver->listen(QHostAddress::Any, this->settings->value("websocketport").toInt());
}

void Modbus_DataAcquire::onNewWebSocketConnection(){
    QWebSocket* pSocket = websocketserver->nextPendingConnection();
    this->websocketConnections.append(pSocket);

    connect(pSocket,&QWebSocket::disconnected,this,&Modbus_DataAcquire::socketDisconnected);
}

void Modbus_DataAcquire::socketDisconnected(){
    auto pSocket = qobject_cast<QWebSocket *>(sender());
    if (!pSocket)
        return;

    this->websocketConnections.removeOne(pSocket);

    disconnect(pSocket,0,0,0);
}

void Modbus_DataAcquire::start(int _counter){
    this->leftCounter = _counter;

    if (!this->modbusClient->connectDevice()) {
        qDebug("Connection Failed");
        return;
    }

    qDebug("Connection Done");

    if(!this->isDone())
    {
        this->doDataAcquire();
        qDebug("doDataAcquire Once. %d left",this->leftCounter);
    }
    else{
        this->modbusClient->disconnectDevice();
    }



}

void Modbus_DataAcquire::doDataAcquire()
{
    try
    {
        //起始地址
        int startAddress =  settings->value("startAddress").toInt();
        //寄存器长度
        int numberOfEntries =  settings->value("numberOfEntries").toInt();
        //从设备id
        int slaveUnitID = settings->value("slaveUnitID").toInt();

        if(this->unit == nullptr)
            this->unit = new QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAddress, numberOfEntries);
        auto *reply = this->modbusClient->sendReadRequest(*unit,slaveUnitID);

        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &Modbus_DataAcquire::onReplayFinished);

    }
    catch(QException ex)
    {
        qDebug(ex.what());
        //如果连接出现异常，尝试重新连接
        if(this->modbusClient->state() == QModbusDevice::State::UnconnectedState){
            this->modbusClient->connectDevice();
        }

    }

    this->counterDown();
}

void Modbus_DataAcquire::onReplayFinished(){
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    disconnect(reply,0,0,0);

    QString json = "[";
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        int valueCount = unit.valueCount();
        for (uint i = 0; i < valueCount; i++) {
            const QString entry = tr("{\"channel\":%1,\"value\":%2,\"count\":%3}")
                    .arg(unit.startAddress() + i)
                    .arg(QString::number(unit.value(i),unit.registerType() <= QModbusDataUnit::Coils ? 10 : 10).toInt()+qrand()%8)
                    .arg(valueCount);//
            qDebug()<<entry;
            if( json.length()>1)
                json = json + ","+entry;
            else {
                json = json + entry;
            }

        }

        if(valueCount > 0){
            json = json + "]";
            for(int j=this->websocketConnections.length()-1;j>=0;j--){
                this->websocketConnections.at(j)->sendTextMessage(json);
            }
        }


    }


    delete reply;

    if(!this->isDone())
    {
        this->doDataAcquire();
        qDebug("doDataAcquire Once. %d left",this->leftCounter);
        //msleep(0);
    }
    else{
        this->modbusClient->disconnectDevice();
    }
}

void Modbus_DataAcquire::stop(){
    this->leftCounter = 0;
}

