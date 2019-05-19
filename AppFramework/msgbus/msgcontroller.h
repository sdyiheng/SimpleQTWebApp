#ifndef MSGCONTROLLER_H
#define MSGCONTROLLER_H

class MsgBus;

class MsgController
{
public:
    MsgController(){}
    virtual void msgRegister() = 0;
};

#endif // MSGCONTROLLER_H
