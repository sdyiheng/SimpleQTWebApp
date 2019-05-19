#include "modbus_dataacquire_thread.h"

#include "QtCore"
#include <QtSerialBus/QModbusRtuSerialMaster>
#include <QtSerialPort/QSerialPort>
#include "../../../../AppFramework/wellknown/appinfo.h"

bool Modbus_DataAcquire_Thread::isDone()
{
    return this->counter == 0;
}

void Modbus_DataAcquire_Thread::run()
{
    QModbusRtuSerialMaster* modbusDevice = new QModbusRtuSerialMaster();

    QString port = "Com5";


//    port = serialportSettings->value("port").toString();
    qDebug()<<"Port:"<<port<<endl;

    //ModBus通信采用RTU传输模式，波特率115200，8数据位，1停止位，奇偶校验无。
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,port);//获取串口端口,下面设置各种参数
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::NoParity);//无较验
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,QSerialPort::Baud19200);//波特率为115200
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data8);//数据位为8位
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,QSerialPort::OneStop);//停止位为1位
    modbusDevice->setTimeout(1000);//连接超时1S
    modbusDevice->setNumberOfRetries(3);//连接失败重试三次连接

    if (!modbusDevice->connectDevice()) {
        //connection failed.
        qDebug("Connection Failed");
        delete modbusDevice;
        return;
    }

    while(!this->isDone())
    {
        this->doDataAcquire(modbusDevice);
        qDebug("doDataAcquire Once. %d left",this->counter);
        msleep(0);
    }

    modbusDevice->disconnectDevice();
    delete modbusDevice;
}


void Modbus_DataAcquire_Thread::doDataAcquire(QModbusRtuSerialMaster* modbusDevice)
{
    try
    {
        int startAddress = 0;
        //Q_ASSERT(startAddress >= 0 && startAddress < 32);

        // do not go beyond 10 entries
        int numberOfEntries = 10;//qMin(3, 32 - startAddress);
        QModbusDataUnit* unit = new QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAddress, numberOfEntries);
        modbusDevice->sendReadRequest(*unit,1);
    }
    catch(QException ex)
    {

    }

    //comple once dataacquire
    if( this->counter > 0 )
        this->counter--;
}
