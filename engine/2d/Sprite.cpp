//
//  Sprite.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Sprite.h"
#include "Engine.h"
#include "Animation.h"
#include "Texture.h"
#include "GL.h"
#include "Engine.h"

NAMESPACE_USING

Sprite::Sprite() : Node(),
_currAnimation(NULL),
_frameCount(0),
_images(NULL),
offsetX(0),
offsetY(0)
{
    for (int i = 0; i < MAX_ANIMS; i++)
    {
        _animations[i] = NULL;
    }
}

Sprite::~Sprite()
{
    for (int i = 0; i < MAX_ANIMS; i++)
    {
        RELEASE(_animations[i]);
    }
    
    _images = NULL;
    _currAnimation = NULL;
}

void Sprite::load(const char * ImageName, int Width, int Height, int OffsetX, int OffsetY)
{
    _images = Engine::Instance()->findImages(ImageName, _frameCount);
    if (!_images)
    {
        LOGW("%s not found!!", ImageName);
    }
    
    if (Width && Height)
    {
        w = Width;
        h = Height;
    }
    else
    {
        w = _images[0]->w;
        h = _images[0]->h;
    }
    
    if (OffsetX && OffsetY)
    {
        offsetX = OffsetX;
        offsetY = OffsetY;
    }
    else
    {
        offsetX = (_images[0]->w - w) / 2.0;
        offsetY = (_images[0]->h - h) / 2.0;
    }
}

void Sprite::createAnimation(int AnimId, const char * Name, int * Frames, int NumOfFrames, int FrameRate, bool Looped)
{
    _animations[AnimId] = new Animation(AnimId, Name, Frames, NumOfFrames, FrameRate, Looped);
}

void Sprite::play(int AnimId, bool Reset)
{
    _currAnimation = _animations[AnimId];
    if (Reset)
    {
        _currAnimation->reset();
    }
    _currAnimation->play();
}

void Sprite::update(float Elapsed)
{
    if (!active)
    {
        return;
    }

    if (_currAnimation)
    {
        _currAnimation->update(Elapsed);
    }
}

void Sprite::_draw()
{
    Texture * currImage = NULL;
    if (currImage)
    {
        currImage = _images[_currAnimation->frameIndex];
    }
    else
    {
        currImage = _images[0];
    }

    Engine::Instance()->renderer()->cmdRender(_mvpMat, currImage, w, h, alpha);
}