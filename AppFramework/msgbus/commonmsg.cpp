#include "commonmsg.h"

CommonMsg::CommonMsg(const QString* msgType)
    :Msg()
{
    this->_msgType = msgType;
}

QString CommonMsg::msgType(){
    return *(this->_msgType);
}
