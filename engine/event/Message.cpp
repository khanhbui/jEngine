//
//  Message.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/12/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Message.h"

NAMESPACE_USING

Message::Message(int Cmd, Ref * From, void * Data) : Ref("Message"),
cmd(Cmd),
from(From),
data(Data)
{
    from->retain();
}

Message::~Message()
{
    data = NULL;

    RELEASE(from);
    RELEASE(origin);
}

Message * Message::clone()
{
    Message * clone = new Message(cmd, from, data);
    clone->origin = origin;
    origin->retain();
    return clone;
}