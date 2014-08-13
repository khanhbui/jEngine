//
//  Animation.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Animation__
#define __jEngine__Animation__

#include "Ref.h"

NAMESPACE_BEGIN

class Animation : public Ref
{
private:
    int __index;

    int * __frames;
    int __numOfFrames;

    int __tick;
    int __frameTick;

public:
    Animation(int AnimId, const char * Name, int * Frames, int NumOfFrames, int FrameRate, bool Looped);
    virtual ~Animation();
    
    void update(float Elapsed);

    void reset();

    void play();
    void stop();
    
    int animId;

    int frameIndex;
    int frameRate;

    bool looped;
    bool finished;
};

NAMESPACE_END

#endif /* defined(__jEngine__Animation__) */
