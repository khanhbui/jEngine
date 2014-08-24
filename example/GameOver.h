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
#include "Font.h"

NAMESPACE_USING

class GameOver : public Node
{
private:
    Font * __gameOverText;

protected:

public:
    GameOver();
    virtual ~GameOver();
};

#endif /* defined(__jEngine__GameOver__) */
