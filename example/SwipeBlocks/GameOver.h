//
//  GameOver.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/24/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__GameOver__
#define __jEngine__GameOver__

#include "Node.h"
#include "NotifierInterface.h"
#include "Button.h"

NAMESPACE_USING

class GameOver : public Node, public NotifierInterface
{
private:
    Button * __playBtn;
    Button * __leaderboardBtn;

protected:
    void _update(float Elapsed);

public:
    GameOver();
    virtual ~GameOver();
};

#endif /* defined(__jEngine__GameOver__) */
