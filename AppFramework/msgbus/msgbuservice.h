#ifndef MSGBUSERVICE_H
#define MSGBUSERVICE_H

class msgcontroller;

class msgbuservice
{
public:
    msgbuservice();
    void registerController(const msgcontroller& controller);
};

#endif // MSGBUSERVICE_H
