#include "modbus_dataacquire_controller.h"
#include "modbus_dataacquire.h"
#include <QVariant>
#include <QDateTime>
#include "appinfo.h"

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

void Modbus_DataAcquire_Controller::start(QString userid){
    this->modbusDA->start(10000000, userid);
}

void Modbus_DataAcquire_Controller::stop(){
    this->modbusDA->stop();
}

QString userid2FileName(QString userid){
    QDateTime time = QDateTime::currentDateTime();
    QString filename = QString("da_%1_%2.txt").arg(userid).arg(time.toString("yyyyMMddHHmmss"));
    return filename;
}

QString getDAFiles(){
    QJsonArray json_array;

    QDir dir = QDir(AppInfo::getInstance().getAppRunningFolder());
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileList = dir.entryInfoList();

    for(int i=0;i<fileList.length();i++){
        QFileInfo file_info = fileList.at(i);
        QString suffix = file_info.suffix();
        QString file_name = file_info.fileName();
        if(QString::compare(suffix, "txt", Qt::CaseInsensitive) == 0 && file_name.startsWith("da_"))
        {
            //QString file_name = file_name;// .absoluteFilePath();
            json_array.append(file_name.mid(3, file_name.length()-7));
        }
    }

    QJsonDocument document;
    document.setArray(json_array);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

QString queryHistoryRecords(QString file_name, QString start){

    int _start = start.toInt();
    QString full_file = AppInfo::getInstance().getAppRunningFolder()+"\\da_"+file_name+".txt";
    QStringList records;

    QFile file(full_file);
    if(!file.exists())
        return "[]";

    QStringList skipRecords;
    int left = 150;

    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       int lineCounter = -1;

       while (!in.atEnd() && left > 0)
       {
          QString line = in.readLine();
          lineCounter++;
          if(lineCounter<_start){
            skipRecords.append(line);
              continue;
            }

          if( line.length() <=0)
              continue;

          records.append(line.mid(15, line.length()-15));
          left--;
       }
       file.close();
    }

    while(skipRecords.length()>0 && records.length()<150){
        QString last = skipRecords.at(skipRecords.length()-1);
        skipRecords.removeAt(skipRecords.length()-1);
        records.insert(0, last);
    }

    if(records.length() == 0)
        return "[]";


    QString json;
    for(int i=0;i<records.length();i++){
        if( json.length() == 0)
            json.append(records.at(i));
        else {
            json.append(",").append(records.at(i));
        }
    }

    json.append("]").insert(0,'[');
    return json;
}

void Modbus_DataAcquire_Controller::service(HttpRequest& request, HttpResponse& response)
{

    QByteArray path=request.getPath();
    if(path.startsWith("/modbusda/start")){
        QString userid = request.getParameter("userid");
        if( userid.length() == 0)
            userid = "anonymous";

        QString filename = userid2FileName(userid);

        this->start(filename);
        response.write("Started",true);
    }
    else if( path.startsWith("/modbusda/stop")){
        this->stop();
        response.write("Stopped",true);
    }

    else if( path.startsWith("/modbusda/historyrecord")){
        QString file_name = request.getParameter("filename");
        QString start = request.getParameter("start");
        response.write(queryHistoryRecords(file_name, start).toUtf8(),true);
    }
    else if( path.startsWith("/modbusda/history")){
        response.write(getDAFiles().toUtf8(),true);
    }
    else{
        response.write("UnHandled",true);
    }

}
