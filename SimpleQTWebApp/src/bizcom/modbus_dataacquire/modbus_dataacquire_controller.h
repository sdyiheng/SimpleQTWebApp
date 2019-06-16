#ifndef MODBUS_DATAACQUIRE_CONTROLLER_H
#define MODBUS_DATAACQUIRE_CONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include "modbus_dataacquire_thread.h"
#include "modbus_dataacquire.h"

using namespace stefanfrings;

class Modbus_DataAcquire_Controller: public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(Modbus_DataAcquire_Controller)

public:
    Modbus_DataAcquire_Controller(Modbus_DataAcquire* modbusDA, QObject* parent=nullptr);

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    bool matchUrl(QByteArray& path);

    void start(QString userid);
    void stop();

private:
    Modbus_DataAcquire* modbusDA;
};

#endif // MODBUS_DATAACQUIRE_CONTROLLER_H
