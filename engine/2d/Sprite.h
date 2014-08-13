//
//  Sprite.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Sprite__
#define __jEngine__Sprite__

#include "Node.h"
#include "Macro.h"

NAMESPACE_BEGIN

class Animation;
class Texture;

class Sprite : public Node
{
protected:
    Animation * _animations[MAX_ANIMS];
    Animation * _currAnimation;

    Texture ** _images;

    int _frameCount;

    virtual void _draw();

public:
    float offsetX;
    float offsetY;
    
    Sprite();
    virtual ~Sprite();

    void load(const char * ImageName, int Width, int Height, int OffsetX, int OffsetY);

    virtual void update(float Elapsed);

    void createAnimation(int AnimId, const char * Name, int * Frames, int NumOfFrames, int FrameRate, bool Looped);
    void play(int AnimId, bool Reset);
};

NAMESPACE_END

#endif /* defined(__jEngine__Sprite__) */
