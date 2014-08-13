//
//  Animation.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Animation.h"

NAMESPACE_USING

Animation::Animation(int AnimId, const char * Name, int * Frames, int NumOfFrames, int FrameRate, bool Looped) : Ref(Name),
animId(AnimId),
__numOfFrames(NumOfFrames),
__frameTick(1000 / FrameRate),
looped(Looped)
{
    __frames = (int *) malloc(__numOfFrames * sizeof(int));
    memcpy(__frames, Frames, __numOfFrames * sizeof(int));
}

Animation::~Animation()
{
    free(__frames);
    __frames = NULL;
}

void Animation::update(float Elapsed)
{
    if (finished)
    {
        return;
    }
    
    Elapsed *= 1000;//Convert to milli seconds
    if (Elapsed > FRAME_TICK)
    {
        Elapsed = FRAME_TICK;
    }
    __tick += Elapsed;
    if (__tick >= __frameTick)
    {
        __tick = 0;
        __index++;
        if (__index >= __numOfFrames)
        {
            //Make sure it's not going out of bound. Reset the index anyway.
            __index = 0;
            if (!looped)
            {
                finished = true;
            }
        }
        frameIndex = __frames[__index];
    }
}

void Animation::reset()
{
    __tick = 0;
    __index = 0;
    frameIndex = __frames[0];
}

void Animation::play()
{
    if (!looped && finished)
    {
        reset();
    }
    finished = false;
}

void Animation::stop()
{
    finished = true;
}