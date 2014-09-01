//
//  Notifier.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Notifier.h"
#include "NotifierInterface.h"
#include "Message.h"

NAMESPACE_USING

Notification::Notification(int Id, Message * Message, bool IsPrivate) : Ref("Notification"),
id(Id),
message(Message),
isPrivate(IsPrivate)
{
    message->retain();
}

Notification::~Notification()
{
    RELEASE(message);
}


Notifier * Notifier::__instance = NULL;

Notifier::Notifier() : Ref("Notifier"),
__currentId(0)
{
    __queue = new std::queue<Notification * >();
    __observed = new std::vector<NotifierInterface *>();
}

Notifier::~Notifier()
{
    while (!__queue->empty())
    {
        __queue->front()->release();
        __queue->pop();
    }
    DELETE(__queue);

    __observed->clear();
    DELETE(__observed);
}

Notifier * Notifier::Instance()
{
    return __instance;
}

void Notifier::Create()
{
    if (!__instance)
    {
        __instance = new Notifier();
    }
}

void Notifier::Delete()
{
    RELEASE(__instance);
}

void Notifier::update(float Elapsed)
{
    std::queue<Notification *> queue;
    while (!__queue->empty())
    {
        queue.push(__queue->front());
        __queue->pop();
    }

    while (!queue.empty())
    {
        Notification * notification = queue.front();
        queue.pop();

        NotifierInterface * obj = NULL;
        for (std::vector<NotifierInterface *>::iterator it = __observed->begin(); it != __observed->end(); it++)
        {
            if ((*it)->__notifierID == notification->id)
            {
                obj = (*it);
                obj->__notify(notification->message);
                break;
            }
        }
    }
}

void Notifier::sendNotification(int Id, Message * Message)
{
    NotifierInterface * obj = NULL;
    for (std::vector<NotifierInterface *>::iterator it = __observed->begin(); it != __observed->end(); it++)
    {
        if ((*it)->__notifierID == Id)
        {
            obj = (*it);
            obj->__notify(Message);
            break;
        }
    }
}

void Notifier::postNotification(int Id, Message * Message)
{
    __postNotification(Id, Message, false);
}

void Notifier::__postNotification(int Id, Message * aMessage, bool IsPrivate)
{
    Message * clone = aMessage->clone();

    __queue->push(new Notification(Id, clone, IsPrivate));
}

int Notifier::registerNotifier(NotifierInterface * Obj)
{
    int id = __currentId++;
    __observed->push_back(Obj);
    
    return id;
}

void Notifier::unregisterNotifier(int Id)
{
    for (std::vector<NotifierInterface *>::iterator it = __observed->begin(); it != __observed->end(); it++)
    {
        if ((*it)->__notifierID == Id)
        {
            __observed->erase(it);
            break;
        }
    }
}