#include "msgbus.h"

void MsgBus::registerMsgHandler(QString msgChannel, MsgHandleType msgHandleType, QString msgType, MsgHandler msgHandler){
    QString k = msgType;// msgChannel + "." + msgType;
    if(!this->msgHandlerMap.contains(k))
        this->msgHandlerMap.insert(k, new QList<MsgHandler>());

    QList<MsgHandler>* list = this->msgHandlerMap.take(k);
    list->append(msgHandler);
}


void MsgBus::unRegisterMsgHandler(QString msgChannel){

}


void MsgBus::unRegisterMsgHandler(QString msgChannel, MsgHandler msgHandler){

}


void MsgBus::postMsg(const Msg& msg){
    //QString msgType = msg.msgType();
}
