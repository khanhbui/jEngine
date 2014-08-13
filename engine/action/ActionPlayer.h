//
//  ActionPlayer.h
//  jEngine
//
//  Created by Khanh Bui on 8/11/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__ActionPlayer__
#define __jEngine__ActionPlayer__

#include "Ref.h"

NAMESPACE_BEGIN

class Action;
class Node;

class ActionPlayer : public Ref
{
private:
    static ActionPlayer * _instance;
    
    std::vector<Action *> _actions;
    
    ActionPlayer();
    ~ActionPlayer();
    
public:
    static ActionPlayer * Instance();
    static void CreateInstance();
    static void DeleteInstance();
    
    void update(float elapsed);
    
    void addAction(Action * action, Node * target);
    void removeAction(Action * action);
    void removeAction(Action * action, bool reset);
    
    void purge();
};

NAMESPACE_END

#endif /* defined(__jEngine__ActionPlayer__) */
