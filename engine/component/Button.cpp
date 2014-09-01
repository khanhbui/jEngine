//
//  Button.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/28/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Button.h"
#include "Engine.h"
#include "Texture.h"

NAMESPACE_USING

Button::Button() : Node(),
__image(NULL),
isPressed(false),
isPressing(false),
justPressed(false),
justReleased(false)
{
}

Button::~Button()
{
    __image = NULL;
}

void Button::_update(float Elapsed)
{
    int numOfTouches = Engine::Instance()->numOfTouches;
    int * touchesX = Engine::Instance()->touchesX;
    int * touchesY = Engine::Instance()->touchesY;
    
    isPressing = false;
    for (int i = 0; i < numOfTouches; i++)
    {
        if (touchesX[i] > _x && touchesX[i] < _x + w && touchesY[i] > _y && touchesY[i] < _y + h)
        {
            isPressing = true;
            break;
        }
    }
    
    justPressed = false;
    justReleased = false;
    
    if (isPressing != isPressed)
    {
        if (isPressed)
        {
            justReleased = true;
            isPressed = false;
        }
        else
        {
            justPressed = true;
            isPressed = true;
        }
    }
}

void Button::_draw()
{
    if (isPressed)
    {
        Engine::Instance()->renderer()->cmdRender(_mvpMat, __image, 0, 2, w, h, alpha);
    }
    else
    {
        Engine::Instance()->renderer()->cmdRender(_mvpMat, __image, w, h, alpha);
    }
}

void Button::load(const char * FileName)
{
    __image = Engine::Instance()->findImage(FileName);

    w = __image->w;
    h = __image->h;
}