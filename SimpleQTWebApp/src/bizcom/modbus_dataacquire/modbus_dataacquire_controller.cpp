#include "modbus_dataacquire_controller.h"
#include "modbus_dataacquire.h"
#include <QVariant>
#include <QDateTime>

///** Storage for session cookies */
//extern Modbus_DataAcquire* modbusda;

Modbus_DataAcquire_Controller::Modbus_DataAcquire_Controller(Modbus_DataAcquire* modbusDA, QObject* parent)
: HttpRequestHandler(parent)
{
    this->modbusDA = modbusDA;
}

bool Modbus_DataAcquire_Controller::matchUrl(QByteArray& path)
{
    return path.startsWith("/modbusda");
}

void Modbus_DataAcquire_Controller::start(){
    this->modbusDA->start(5000000);
}

void Modbus_DataAcquire_Controller::stop(){
    this->modbusDA->stop();
}

void Modbus_DataAcquire_Controller::service(HttpRequest& request, HttpResponse& response)
{

    QByteArray path=request.getPath();
    if(path.startsWith("/modbusda/start")){
        this->start();
        response.write("Started",true);
    }
    else if( path.startsWith("/modbusda/stop")){
        this->stop();
        response.write("Stopped",true);
    }
    else{
        response.write("UnHandled",true);
    }

}
