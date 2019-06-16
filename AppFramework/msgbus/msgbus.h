#ifndef MSGBUS_H
#define MSGBUS_H

#include "msg.h"
#include "msgcontroller.h"
#include <QtCore>

typedef void (*MsgHandler)();

enum MsgHandleType{
    Sync,
    Async,
    Queue,
};

class MsgBus
{

public:
    static MsgBus& getInstance(){
        static MsgBus instance;
        return instance;
    }

private:
    MsgBus(){}
    MsgBus(MsgBus const&);              // Don't Implement
    void operator=(MsgBus const&);      // Don't implement
    QMap<QString, QList<MsgHandler>* > msgHandlerMap;

public:
    void registerMsgHandler(QString msgChannel, MsgHandleType msgHandleType, QString msgType, MsgHandler msgHandler);
    void unRegisterMsgHandler(QString msgChannel);
    void unRegisterMsgHandler(QString msgChannel, MsgHandler msgHandler);

    void postMsg();

};

#endif // MSGBUS_H
