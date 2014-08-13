//
//  Random.h
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Random__
#define __jEngine__Random__

#include "Ref.h"

NAMESPACE_BEGIN

class Random : public Ref
{
private:
    int __seedW;
    int __seedZ;

public:
    Random();
    ~Random();
    
    void randomize(long newSeed);

    int rand();
    int randomIn(int Low, int High);

    float randf();
    float randomInf(float Low, float High);
};

NAMESPACE_END

#endif /* defined(__jEngine__Random__) */
