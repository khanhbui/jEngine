//
//  Timer.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Timer.h"

#include <sys/time.h>

NAMESPACE_USING

Timer::Timer() : Ref()
{
}

Timer::~Timer()
{
}

double Timer::getCurrentMillSecond()
{
    struct timeval stCurrentTime;
    gettimeofday(&stCurrentTime, NULL);
    double lLastTime = (1000.0 * (stCurrentTime.tv_sec)) + (0.001 * (stCurrentTime.tv_usec));
    return lLastTime;
}