//
//  ObserverInterface.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "ObserverInterface.h"
#include "NotifierInterface.h"

NAMESPACE_USING

ObserverRecord::ObserverRecord(int Id, NotifierInterface * Target) : Ref("ObserverRecord"),
id(Id),
target(Target)
{
}

ObserverInterface::ObserverInterface()
{
    __observeRecord = new std::vector<ObserverRecord *>();
}

ObserverInterface::~ObserverInterface()
{
}

void ObserverInterface::beginObserve(NotifierInterface * Target)
{
    int index = __checkObserveRecord(Target);
    if (index < 0)
    {
        int id = Target->__addObserver();
        
        ObserverRecord * info = new ObserverRecord(id, Target);
        __observeRecord->push_back(info);
    }
}

void ObserverInterface::endObserve(NotifierInterface * Target)
{
    int index = __checkObserveRecord(Target);
    if (index >= 0)
    {
        ObserverRecord * info = __observeRecord->at(index);
        
        info->target->__deleteObserver(info->id);
        __observeRecord->erase(__observeRecord->begin() + index);
    }
}

void ObserverInterface::terminateObserve()
{
    while (!__observeRecord->empty())
    {
        ObserverRecord * info = *(__observeRecord->begin());
        info->target->__deleteObserver(info->id);
        __observeRecord->erase(__observeRecord->begin());
    }
}

int ObserverInterface::__checkObserveRecord(NotifierInterface * Target)
{
    int i = 0;
    for (std::vector<ObserverRecord *>::iterator it = __observeRecord->begin(); it != __observeRecord->end(); it++, i++)
    {
        if ((*it)->target == Target)
        {
            return i;
        }
    }
    return -1;
}
