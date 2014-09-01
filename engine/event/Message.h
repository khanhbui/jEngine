//
//  Message.h
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Message__
#define __jEngine__Message__

#include "Ref.h"

NAMESPACE_BEGIN

class Message : public Ref
{
public:
    Message(int Cmd, Ref * From, void * Data);
    ~Message();

    int cmd;
    Ref * from;
    Ref * origin;
    void * data;

    Message * clone();
};

NAMESPACE_END

#endif /* defined(__jEngine__Message__) */
