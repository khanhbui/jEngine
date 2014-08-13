//
//  Timer.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Timer__
#define __jEngine__Timer__

#include "Ref.h"

NAMESPACE_BEGIN

class Timer : public Ref
{
protected:
    Timer();

public:
    ~Timer();

    static double getCurrentMillSecond();
};

NAMESPACE_END

#endif /* defined(__jEngine__Timer__) */
