/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "requestmapper.h"
#include "filelogger.h"
#include "staticfilecontroller.h"
#include "controller/dumpcontroller.h"
#include "controller/templatecontroller.h"
#include "controller/formcontroller.h"
#include "controller/fileuploadcontroller.h"
#include "controller/sessioncontroller.h"

#include "bizcom/modbus_dataacquire/modbus_dataacquire_controller.h"
#include "../AppFramework/wellknown/appinfo.h"
#include "bizcom/modbus_dataacquire/modbus_dataacquire.h"

/** Redirects log messages to a file */
extern FileLogger* logger;

/** Controller for static files */
extern StaticFileController* staticFileController;

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");

    this->handlersCount = 6;
    this->handlers = new HttpRequestHandler*[this->handlersCount];

    this->handlers[0] = new DumpController(parent);
    this->handlers[1] = new TemplateController(parent);
    this->handlers[2] = new FormController(parent);
    this->handlers[3] = new FileUploadController(parent);
    this->handlers[4] = new SessionController(parent);


    Modbus_DataAcquire_Controller *controller = new Modbus_DataAcquire_Controller(parent);
    this->handlers[5] = controller;

    //Configure modbus ds
    QSettings* modbusDASettings=new QSettings(AppInfo::getInstance().getAppConfigFileName(),QSettings::IniFormat,AppInfo::getInstance().getApp());
    modbusDASettings->beginGroup("modbusda");
    Modbus_DataAcquire* modbusDA = new Modbus_DataAcquire(modbusDASettings, controller, AppInfo::getInstance().getApp());
    //modbusDA->moveToThread(new QThread());

    this->modbusDA = modbusDA;

    modbusDA->init();
}



RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}

bool RequestMapper::matchUrl(QByteArray& path)
{
    return true;
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    bool handled = false;

    for(int i=0;i<this->handlersCount;i++)
    {
        if(this->handlers[i]->matchUrl(path))
        {
            this->handlers[i]->service(request, response);
            handled = true;
            break;
        }
    }

    if(!handled){
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
    if (logger)
    {
       logger->clear();
    }
}
