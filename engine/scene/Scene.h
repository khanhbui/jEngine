//
//  Scene.h
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Scene__
#define __jEngine__Scene__

#include "Node.h"

NAMESPACE_BEGIN

class Scene : public Node
{
private:

protected:

public:
    Scene();
    virtual ~Scene();

    virtual void touchPressed(int X, int Y);
    virtual void touchReleased(int PrevX, int PrevY, int X, int Y);
    virtual void touchMoved(int SrcX, int SrcY, int X, int Y);
};

NAMESPACE_END

#endif /* defined(__jEngine__Scene__) */
