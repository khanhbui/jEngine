//
//  Macro.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Macro__
#define __jEngine__Macro__

#include <assert.h>

#include "Log.h"

#define ASSERT(expr, msg, ...) \
    do\
    {\
        assert(expr);\
        LOGW(msg, __VA_ARGS__);\
    }\
    while(0);

#define DELETE(object) \
    if (object != NULL)\
    {\
        delete object;\
        object = NULL;\
    }

#define DELETE_ARRAY(object) \
    if (object != NULL)\
    {\
        delete [] object;\
        object = NULL;\
    }

#define RELEASE(object) \
    if (object != NULL)\
    {\
        object->release();\
        object = NULL;\
    }

#define NAMESPACE_BEGIN namespace j {
#define NAMESPACE_END }
#define NAMESPACE_USING using namespace j;

#define GET_MAX(a, b) ((a > b) ? a : b)
#define GET_MIN(a, b) ((a > b) ? b : a)

#define ENGINE_VERSION 2.0.0

#define FRAME_SECONDS 0.025 //0.01667f //1000/60 ~60 fps
#define FRAME_TICK 15

#define MAX_IMAGES 512
#define MAX_ANIMS 10
#define MAX_EVENTS 50
#define MAX_TOUCHES 10
#define MAX_QUADS 256

#define MATRIX_SIZE ( sizeof(float) * 16)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 568

#define OFF_SCREEN_WIDTH 1024.0f
#define OFF_SCREEN_HEIGHT 1024.0f

#define PI 3.1415926
#define HUNDRED 100;

#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_EPSILON                0.000001f

#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)

#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f

#endif /* defined(__jEngine__Macro__) */
