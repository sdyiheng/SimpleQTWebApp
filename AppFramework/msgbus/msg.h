#ifndef MSG_H
#define MSG_H

#include "QtCore"

class Msg
{
public:
    Msg(){}
    virtual QString msgType()=0;

};

#endif // MSG_H
