#ifndef MODBUS_DATAACQUIRE_CONTROLLER_H
#define MODBUS_DATAACQUIRE_CONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"


using namespace stefanfrings;

class Modbus_DataAcquire_Controller: public HttpRequestHandler {
    Q_OBJECT
    //Q_DISABLE_COPY(Modbus_DataAcquire_Controller)

public:
    Modbus_DataAcquire_Controller( QObject* parent=nullptr);//Modbus_DataAcquire* modbusDA,

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    bool matchUrl(QByteArray& path);

    void start();
    void stop();

signals:
       void Signal_Start (int counter);
       void Signal_Stop();

private:
    //Modbus_DataAcquire* modbusDA;
};

#endif // MODBUS_DATAACQUIRE_CONTROLLER_H
