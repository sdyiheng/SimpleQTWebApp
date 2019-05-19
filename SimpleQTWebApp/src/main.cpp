/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include <QDir>
#include "httplistener.h"
#include "templatecache.h"
#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "requestmapper.h"
#include "../AppFramework/wellknown/appinfo.h"
#include <QtSerialBus/QModbusRtuSerialMaster>
#include <QtSerialPort/QSerialPort>
#include "bizcom/modbus_dataacquire/modbus_dataacquire.h"

using namespace stefanfrings;

/** Cache for template files */
 TemplateCache* templateCache;

/** Storage for session cookies */
 HttpSessionStore* sessionStore;

/** Controller for static files */
 StaticFileController* staticFileController;

/** Redirects log messages to a file */
 FileLogger* logger;


 /*Modbus_DataAcquire* modbusDA*/;


/** Search the configuration file */
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName(appName+".ini");

    QStringList searchList;

    searchList.append(QDir::toNativeSeparators(binDir+"/"+fileName));
    searchList.append(QDir::toNativeSeparators(binDir+"/etc"+"/"+fileName));
//    searchList.append(QDir::toNativeSeparators(binDir+"/../etc"+"/"+fileName));
//    searchList.append(QDir::toNativeSeparators(binDir+"/../../etc"+"/"+fileName));
//    searchList.append(binDir+"/../"+appName+"/etc"); // for development with shadow build
//    searchList.append(binDir+"/../../"+appName+"/etc"); // for development with shadow build
//    searchList.append(binDir+"/../../../"+appName+"/etc"); // for development with shadow build
//    searchList.append(binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
//    searchList.append(binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
//    searchList.append(QDir::rootPath()+"etc/opt");
//    searchList.append(QDir::rootPath()+"etc");

    foreach (QString f, searchList)
    {
        QFile file(f);
        if (file.exists())
        {
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }

    qFatal("Cannot find config file %s",qPrintable(fileName));
    return "";
}


/**
  Entry point of the program.
*/
int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    app.setApplicationName("SimpleQTWebApp");
    app.setOrganizationName("SimpleWebApp");

    // Find the configuration file
    QString configFileName=searchConfigFile();

    //
    AppInfo::getInstance().setAppConfigFileName(configFileName);
    AppInfo::getInstance().setApp(&app);

    // Configure logging into a file
    /*
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    logSettings->beginGroup("logging");
    FileLogger* logger=new FileLogger(logSettings,10000,&app);
    logger->installMsgHandler();
    */

    // Configure template loader and cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    templateSettings->beginGroup("templates");
    templateCache=new TemplateCache(templateSettings,&app);

    // Configure session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings,&app);

    // Configure static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    fileSettings->beginGroup("docroot");
    staticFileController=new StaticFileController(fileSettings,&app);


    // Configure and start the TCP listener
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);


    qWarning("Application has started");
    app.exec();
    qWarning("Application has stopped");
}
