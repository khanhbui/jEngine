//
//  ObserverInterface.h
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__ObserverInterface__
#define __jEngine__ObserverInterface__

#include "Ref.h"
#include <vector>

NAMESPACE_BEGIN

class Message;
class NotifierInterface;

class ObserverRecord : public Ref
{
public:
    ObserverRecord(int Id, NotifierInterface * Target);

    int id;
    NotifierInterface * target;
};

class ObserverInterface
{
private:
    std::vector<ObserverRecord *> * __observeRecord;

    int __checkObserveRecord(NotifierInterface * Target);

protected:
    ObserverInterface();

public:
    ~ObserverInterface();

    virtual void onNotify(Message * Message) = 0;

    void beginObserve(NotifierInterface * Target);
    void endObserve(NotifierInterface * Target);
    void terminateObserve();
};

NAMESPACE_END

#endif /* defined(__jEngine__ObserverInterface__) */
