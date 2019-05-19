#ifndef COMMONMSG_H
#define COMMONMSG_H

#include "msg.h"

class CommonMsg:public Msg
{
public:
    CommonMsg(const QString* msgType);
    QString msgType();

private:
    const QString* _msgType;
};

#endif // COMMONMSG_H
