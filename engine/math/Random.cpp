//
//  Random.cpp
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Random.h"

NAMESPACE_USING

Random::Random() : Ref("Random"),
__seedW(0),
__seedZ(0)
{
}

Random::~Random()
{
}

void Random::randomize(long newSeed)
{
    __seedW = newSeed % 32000;
    __seedZ = newSeed % 65535;
};

int Random::rand()
{
    __seedZ = 36969 * (__seedZ & 65535) + (__seedZ >> 16);
    __seedW = 18000 * (__seedW & 65535) + (__seedW >> 16);
    int r = std::abs(((__seedZ << 16) + __seedW));
    return r;
};

int Random::randomIn(int Low, int High)
{
    return Low + rand() % (High - Low);
}

float Random::randf()
{
    return 0.1f * (rand() % 10);
}

float Random::randomInf(float Low, float High)
{
    return Low + (High - Low) * randf();
}
