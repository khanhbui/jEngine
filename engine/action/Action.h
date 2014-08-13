//
//  Action.h
//  jEngine
//
//  Created by Khanh Bui on 8/11/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Action__
#define __jEngine__Action__

#include "Ref.h"

NAMESPACE_BEGIN

class Node;

class Action : public Ref
{
private:
    
protected:
    Node * _target;
    
public:
    Action();
    virtual ~Action();
    
    virtual void setTarget(Node * target);
    
    virtual void step(float elapsed);
    virtual void update(float t) = 0;
    
    virtual void reset() = 0;
    
    virtual void play();
    virtual void stop();
    
    virtual bool isFinished() const;
};

NAMESPACE_END

#endif /* defined(__jEngine__Action__) */
