//
//  Math.h
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Math__
#define __jEngine__Math__

#include "Ref.h"
#include "Random.h"

#include <cmath>

NAMESPACE_BEGIN

class Random;

class Math : public Ref
{
private:
    static float * m_sin;
    //    static float * m_cos;
    
    static float * m_easeQuadIn;
    static float * m_easeQuadOut;
    static float * m_easeQuadInOut;
    
    static float * m_easeCubeIn;
    static float * m_easeCubeOut;
    static float * m_easeCubeInOut;
    
    static float * m_easeQuartIn;
    static float * m_easeQuartOut;
    static float * m_easeQuartInOut;
    
    static float * m_easeQuintIn;
    static float * m_easeQuintOut;
    static float * m_easeQuintInOut;
    
    static float * m_easeElasticIn;
    static float * m_easeElasticOut;
    static float * m_easeElasticInOut;
    
    static float * m_easeBackIn;
    static float * m_easeBackOut;
    static float * m_easeBackInOut;
    
    static float * m_easeBounceIn;
    static float * m_easeBounceOut;
    static float * m_easeBounceInOut;
    
    static Random * __randomizer;

protected:
    static Math * _instance;

    Math();

public:
    ~Math();

    static void randomize(long newSeed);

    static int rand();
    static int randomIn(int Low, int High);

    static float randf();
    static int randomInf(float Low, float High);
    
    static float fix(float Angle)
    {
        while (Angle > 360) Angle -= 360;
        while (Angle < 0) Angle += 360;
        return Angle;
    };
    
    static float sin(float Angle)
    {
        Angle = fix(Angle);
        return Math::m_sin[(int)(Angle*4)];
    };
    
    static float cos(float Angle)
    {
        return sin(90-Angle);
    }

    static  float max(float number1, float number2)
    {
        if(number1 > number2) return number1;
        else return number2;
    }
    
    static double LinearInterpolation(double p);
    static double QuadraticEaseIn(double p);
    static double QuadraticEaseOut(double p);
    static double QuadraticEaseInOut(double p);
    static double CubicEaseIn(double p);
    static double CubicEaseOut(double p);
    static double CubicEaseInOut(double p);
    static double QuarticEaseIn(double p);
    static double QuarticEaseOut(double p);
    static double QuarticEaseInOut(double p);
    static double QuinticEaseIn(double p);
    static double QuinticEaseOut(double p);
    static double QuinticEaseInOut(double p);
    static double SineEaseIn(double p);
    static double SineEaseOut(double p);
    static double SineEaseInOut(double p);
    static double CircularEaseIn(double p);
    static double CircularEaseOut(double p);
    static double CircularEaseInOut(double p);
    static double ExponentialEaseIn(double p);
    static double ExponentialEaseOut(double p);
    static double ExponentialEaseInOut(double p);
    static double ElasticEaseIn(double p);
    static double ElasticEaseOut(double p);
    static double ElasticEaseInOut(double p);
    static double BackEaseIn(double p);
    static double BackEaseOut(double p);
    static double BackEaseInOut(double p);
    static double BounceEaseIn(double p);
    static double BounceEaseOut(double p);
    static double BounceEaseInOut(double p);
    
    static float ease_quadIn(int percentage)
    {
        return Math::m_easeQuartIn[percentage];
    }
    
    static float ease_quadOut(int percentage)
    {
        return Math::m_easeQuadOut[percentage];
    }
    
    static float ease_quadInOut(int percentage)
    {
        return Math::m_easeQuadInOut[percentage];
    }
    
    static float ease_cubeIn(int percentage)
    {
        return Math::m_easeCubeIn[percentage];
    }
    
    static float ease_cubeOut(int percentage)
    {
        return Math::m_easeCubeOut[percentage];
    }
    
    static float ease_cubeInOut(int percentage)
    {
        return Math::m_easeCubeInOut[percentage];
    }
    
    static float ease_quartIn(int percentage)
    {
        return Math::m_easeQuartIn[percentage];
    }
    
    static float ease_quartOut(int percentage)
    {
        return Math::m_easeQuartOut[percentage];
    }
    
    static float ease_quartInOut(int percentage)
    {
        return Math::m_easeQuartInOut[percentage];
    }
    
    static float ease_quintIn(int percentage)
    {
        return Math::m_easeQuintIn[percentage];
    }
    
    static float ease_quintOut(int percentage)
    {
        return Math::m_easeQuintOut[percentage];
    }
    
    static float ease_quintInOut(int percentage)
    {
        return Math::m_easeQuintInOut[percentage];
    }
    
    static float ease_elasticIn(int percentage)
    {
        return Math::m_easeElasticIn[percentage];
    }
    
    static float ease_elasticOut(int percentage)
    {
        return Math::m_easeElasticOut[percentage];
    }
    
    static float ease_elasticInOut(int percentage)
    {
        return Math::m_easeElasticInOut[percentage];
    }
    
    static float ease_backIn(int percentage)
    {
        return Math::m_easeElasticIn[percentage];
    }
    
    static float ease_backOut(int percentage)
    {
        return Math::m_easeElasticOut[percentage];
    }
    
    static float ease_backInOut(int percentage)
    {
        return Math::m_easeElasticInOut[percentage];
    }
    
    static float ease_bounceIn(int percentage)
    {
        return Math::m_easeBounceIn[percentage];
    }
    
    static float ease_bounceOut(int percentage)
    {
        return Math::m_easeBounceOut[percentage];
    }
    
    static float ease_bounceInOut(int percentage)
    {
        return Math::m_easeBounceInOut[percentage];
    }
    
    static void rotatePoint(int &X, int &Y, float Pivot_x, float Pivot_y, int Angle)
    {
        float dx = (X - Pivot_x);
        float dy = (Y - Pivot_y);
        
        Angle = fix(Angle);
        
        X = Pivot_x + (cos(Angle) * dx - sin(Angle) * dy);
        Y = Pivot_y + (sin(Angle) * dx + cos(Angle) * dy);
    }
    
    static Math * Instance()
    {
        return Math::_instance;
    }
    
    static void Create()
    {
        if (!Math::_instance)
        {
            Math::__randomizer = new Random();
            Math::_instance = new Math();
        }
    }
    
    static void Delete()
    {
        RELEASE(Math::_instance);
    }

private:
    static void InitSin();
    static void InitLinearInterpolation();
    static void InitQuadraticEaseIn();
    static void InitQuadraticEaseOut();
    static void InitQuadraticEaseInOut();
    static void InitCubicEaseIn();
    static void InitCubicEaseOut();
    static void InitCubicEaseInOut();
    static void InitQuarticEaseIn();
    static void InitQuarticEaseOut();
    static void InitQuarticEaseInOut();
    static void InitQuinticEaseIn();
    static void InitQuinticEaseOut();
    static void InitQuinticEaseInOut();
    static void InitSineEaseIn();
    static void InitSineEaseOut();
    static void InitSineEaseInOut();
    static void InitCircularEaseIn();
    static void InitCircularEaseOut();
    static void InitCircularEaseInOut();
    static void InitExponentialEaseIn();
    static void InitExponentialEaseOut();
    static void InitExponentialEaseInOut();
    static void InitElasticEaseIn();
    static void InitElasticEaseOut();
    static void InitElasticEaseInOut();
    static void InitBackEaseIn();
    static void InitBackEaseOut();
    static void InitBackEaseInOut();
    static void InitBounceEaseIn();
    static void InitBounceEaseOut();
    static void InitBounceEaseInOut();
};

NAMESPACE_END

#endif /* defined(__jEngine__Math__) */
