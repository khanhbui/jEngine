//
//  Button.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/28/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Button__
#define __jEngine__Button__

#include "Node.h"

NAMESPACE_BEGIN

class Texture;

class Button : public Node
{
private:
    Texture * __image;

protected:
    virtual void _update(float Elapsed);
    virtual void _draw();

public:
    bool isPressed;
    bool isPressing;

    bool justPressed;
    bool justReleased;

    Button();
    virtual ~Button();

    virtual void load(const char * FileName);
};

NAMESPACE_END

#endif /* defined(__jEngine__Button__) */
