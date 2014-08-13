//
//  Action.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/11/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Action.h"
#include "ActionPlayer.h"
#include "Node.h"

NAMESPACE_USING

Action::Action() : Ref(),
_target(NULL)
{
}

Action::~Action()
{
    RELEASE(_target);
}

void Action::setTarget(Node * target)
{
    _target = target;
    _target->retain();
}

void Action::step(float elapsed)
{
    update(elapsed);
}

void Action::play()
{
    ActionPlayer::Instance()->addAction(this, _target);
}

void Action::stop()
{
    _target = NULL;
}

bool Action::isFinished() const
{
    return true;
};
