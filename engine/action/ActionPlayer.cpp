//
//  ActionPlayer.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/11/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "ActionPlayer.h"
#include "Action.h"

NAMESPACE_USING

ActionPlayer * ActionPlayer::_instance = NULL;

ActionPlayer::ActionPlayer() : Ref("ActionPlayer")
{
}

ActionPlayer::~ActionPlayer()
{
    _actions.clear();
}

ActionPlayer * ActionPlayer::Instance()
{
    if (!ActionPlayer::_instance)
    {
        ActionPlayer::CreateInstance();
    }
    return ActionPlayer::_instance;
}

void ActionPlayer::CreateInstance()
{
    if (!ActionPlayer::_instance)
    {
        ActionPlayer::_instance = new ActionPlayer();
    }
}

void ActionPlayer::DeleteInstance()
{
    if (!ActionPlayer::_instance)
    {
        delete ActionPlayer::_instance;
    }
    ActionPlayer::_instance = NULL;
}

void ActionPlayer::update(float elapsed)
{
    if (_actions.empty())
    {
        return;
    }
    
    std::vector<Action *>::iterator it = _actions.begin();
    while (it != _actions.end())
    {
        Action * action = *it;
        
        action->step(elapsed);
        if (action->isFinished())
        {
            it = _actions.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void ActionPlayer::addAction(Action * action, Node * target)
{
    _actions.push_back(action);
    
    action->setTarget(target);
}

void ActionPlayer::removeAction(Action * action)
{
    removeAction(action, true);
}

void ActionPlayer::removeAction(Action * action, bool reset)
{
    for (std::vector<Action *>::iterator it = _actions.begin(); it != _actions.end(); it++)
    {
        if (action == (Action *)(*it))
        {
            if (reset)
            {
                (*it)->reset();
            }
            _actions.erase(it);
            break;
        }
    }
}

void ActionPlayer::purge()
{
    std::vector<Action *>::iterator it = _actions.begin();
    while (it != _actions.end())
    {
        (*it)->reset();
        it++;
    }
    _actions.clear();
}
