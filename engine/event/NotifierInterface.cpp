//
//  NotifierInterface.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "NotifierInterface.h"
#include "ObserverInterface.h"
#include "Message.h"
#include "Notifier.h"

NAMESPACE_USING

NotifierInterface::NotifierInterface()
{
    __observers = new std::vector<ObserverInterface *>();
}

NotifierInterface::~NotifierInterface()
{
    __observers->clear();
    DELETE(__observers);
}

void NotifierInterface::__notify(Message * Message)
{
    if (!__observers->empty())
    {
        for (std::vector<ObserverInterface *>::iterator it = __observers->begin(); it != __observers->end(); it++)
        {
            (*it)->onNotify(Message);
        }
    }
}

int NotifierInterface::__addObserver()
{
    return -1;
}

void NotifierInterface::__deleteObserver(int Id)
{
}

void NotifierInterface::sendNotification(char * Cmd, void * Data, Ref * From)
{
    Message * message = new Message(Cmd, From, Data);
    
    Notifier::Instance()->sendNotification(__notifierID, message);
}

void NotifierInterface::postNotification(char * Cmd, void * Data, Ref * From)
{
    Message * message = new Message(Cmd, From, Data);
    
    Notifier::Instance()->postNotification(__notifierID, message);
}
