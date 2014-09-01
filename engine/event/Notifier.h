//
//  Notifier.h
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Notifier__
#define __jEngine__Notifier__

#include "Ref.h"

#include <queue>
#include <vector>

NAMESPACE_BEGIN

class Message;
class NotifierInterface;

class Notification : public Ref
{
public:
    Notification(int Id, Message * Message, bool IsPrivate);
    ~Notification();

    int id;
    Message * message;
    bool isPrivate;
};

class Notifier : public Ref
{
private:
    Notifier();

    static Notifier * __instance;

    int __currentId;

    std::queue<Notification *> * __queue;
    std::vector<NotifierInterface *> * __observed;

    void __postNotification(int Id, Message * Message, bool IsPrivate);

public:
    ~Notifier();

    static Notifier * Instance();
    static void Create();
    static void Delete();

    void update(float Elapsed);

    void sendNotification(int Id, Message * Message);
    void postNotification(int Id, Message * Message);

    int registerNotifier(NotifierInterface * Obj);
    void unregisterNotifier(int Id);
};

NAMESPACE_END

#endif /* defined(__jEngine__Notifier__) */
