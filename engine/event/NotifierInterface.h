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
    int __notifierID;
    std::vector<ObserverInterface *> * __observers;

    void __notify(Message * Message);

    size_t __addObserver(ObserverInterface * obj);
    void __deleteObserver(size_t Id);

protected:
    NotifierInterface();

public:
    ~NotifierInterface();

    void sendNotification(int Cmd, void * Data, Ref * From);
    void postNotification(int Cmd, void * Data, Ref * From);

    friend Notifier;
    friend ObserverInterface;
};

NAMESPACE_END

#endif /* defined(__jEngine__NotifierInterface__) */
