//
//  NotifierInterface.h
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__NotifierInterface__
#define __jEngine__NotifierInterface__

#include "Macro.h"

#include <vector>

NAMESPACE_BEGIN

class Ref;
class Message;
class Notifier;
class ObserverInterface;

class NotifierInterface
{
private:
    NotifierInterface();

    int __notifierID;
    std::vector<ObserverInterface *> * __observers;

    void __notify(Message * Message);

    int __addObserver();
    void __deleteObserver(int Id);

protected:

public:
    ~NotifierInterface();

    void sendNotification(char * Cmd, void * Data, Ref * From);
    void postNotification(char * Cmd, void * Data, Ref * From);

    friend Notifier;
    friend ObserverInterface;
};

NAMESPACE_END

#endif /* defined(__jEngine__NotifierInterface__) */
