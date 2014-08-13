//
//  Math.cpp
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "MathUtil.h"
#include "Random.h"

NAMESPACE_USING

MathUtil * MathUtil::_instance = NULL;

Random * MathUtil::__randomizer = NULL;

float * MathUtil::m_sin = NULL;
float * MathUtil::m_easeQuadIn = NULL;
float * MathUtil::m_easeQuadOut = NULL;
float * MathUtil::m_easeQuadInOut = NULL;
float * MathUtil::m_easeCubeIn = NULL;
float * MathUtil::m_easeCubeOut = NULL;
float * MathUtil::m_easeCubeInOut = NULL;
float * MathUtil::m_easeQuartIn = NULL;
float * MathUtil::m_easeQuartOut = NULL;
float * MathUtil::m_easeQuartInOut = NULL;
float * MathUtil::m_easeQuintIn = NULL;
float * MathUtil::m_easeQuintOut = NULL;
float * MathUtil::m_easeQuintInOut = NULL;
float * MathUtil::m_easeElasticIn = NULL;
float * MathUtil::m_easeElasticOut = NULL;
float * MathUtil::m_easeElasticInOut = NULL;
float * MathUtil::m_easeBackIn = NULL;
float * MathUtil::m_easeBackOut = NULL;
float * MathUtil::m_easeBackInOut = NULL;
float * MathUtil::m_easeBounceIn = NULL;
float * MathUtil::m_easeBounceOut = NULL;
float * MathUtil::m_easeBounceInOut = NULL;

MathUtil::MathUtil()
{
    int SizeOfFloat = sizeof(float);
    InitSin();
    
    m_easeQuadIn = (float *)malloc(100 * SizeOfFloat);
    m_easeQuadOut = (float *)malloc(100 * SizeOfFloat);
    m_easeQuadInOut = (float *)malloc(100 * SizeOfFloat);
    
    m_easeCubeIn = (float *)malloc(100 * SizeOfFloat);
    m_easeCubeOut = (float *)malloc(100 * SizeOfFloat);
    m_easeCubeInOut = (float *)malloc(100 * SizeOfFloat);
    
    m_easeQuartIn = (float *)malloc(100 * SizeOfFloat);
    m_easeQuartOut = (float *)malloc(100 * SizeOfFloat);
    m_easeQuartInOut = (float *)malloc(100 * SizeOfFloat);
    
    m_easeQuintIn = (float *)malloc(100 * SizeOfFloat);
    m_easeQuintOut = (float *)malloc(100 * SizeOfFloat);
    m_easeQuintInOut = (float *)malloc(100 * SizeOfFloat);
    
    m_easeElasticIn = (float *)malloc(100 * SizeOfFloat);
    m_easeElasticOut = (float *)malloc(100 * SizeOfFloat);
    m_easeElasticInOut = (float *)malloc(100 * SizeOfFloat);
    
    m_easeBackIn = (float *)malloc(100 * SizeOfFloat);
    m_easeBackOut = (float *)malloc(100 * SizeOfFloat);
    m_easeBackInOut = (float *)malloc(100 * SizeOfFloat);
    
    m_easeBounceIn = (float *)malloc(100 * SizeOfFloat);
    m_easeBounceOut = (float *)malloc(100 * SizeOfFloat);
    m_easeBounceInOut = (float *)malloc(100 * SizeOfFloat);
    
    for (int i = 0; i < 100; i++)
    {
        double t = i / 100.0;
        m_easeQuadIn[i] = QuadraticEaseIn(t);
        m_easeQuadOut[i] = QuadraticEaseOut(t);
        m_easeQuadInOut[i] = QuadraticEaseInOut(t);
        
        m_easeCubeIn[i] = CubicEaseIn(t);
        m_easeCubeOut[i] = CubicEaseOut(t);
        m_easeCubeInOut[i] = CubicEaseInOut(t);
        
        m_easeQuartIn[i] = QuarticEaseIn(t);
        m_easeQuartOut[i] = QuarticEaseOut(t);
        m_easeQuartInOut[i] = QuarticEaseInOut(t);
        
        m_easeQuintIn[i] = QuinticEaseIn(t);
        m_easeQuintOut[i] = QuinticEaseOut(t);
        m_easeQuintInOut[i] = QuinticEaseInOut(t);
        
        m_easeElasticIn[i] = ElasticEaseIn(t);
        m_easeElasticOut[i] = ElasticEaseOut(t);
        m_easeElasticInOut[i] = ElasticEaseInOut(t);
        
        m_easeBackIn[i] = BackEaseIn(t);
        m_easeBackOut[i] = BackEaseOut(t);
        m_easeBackInOut[i] = BackEaseInOut(t);
        
        m_easeBounceIn[i] = BounceEaseIn(t);
        m_easeBounceOut[i] = BounceEaseOut(t);
        m_easeBounceInOut[i] = BounceEaseInOut(t);
    }
}

MathUtil::~MathUtil()
{
    RELEASE(MathUtil::__randomizer);

    free(m_sin);
    //    free(m_cos);
    
    free(m_easeQuadIn);
    free(m_easeQuadOut);
    free(m_easeQuadInOut);
    
    free(m_easeCubeIn);
    free(m_easeCubeOut);
    free(m_easeCubeInOut);
    
    free(m_easeQuartIn);
    free(m_easeQuartOut);
    free(m_easeQuartInOut);
    
    free(m_easeQuintIn);
    free(m_easeQuintOut);
    free(m_easeQuintInOut);
    
    free(m_easeElasticIn);
    free(m_easeElasticOut);
    free(m_easeElasticInOut);
    
    free(m_easeBackIn);
    free(m_easeBackOut);
    free(m_easeBackInOut);
    
    free(m_easeBounceIn);
    free(m_easeBounceOut);
    free(m_easeBounceInOut);
    
    m_sin = NULL;
    //    m_cos = NULL;
    
    m_easeQuadIn = NULL;
    m_easeQuadOut = NULL;
    m_easeQuadInOut = NULL;
    
    m_easeCubeIn = NULL;
    m_easeCubeOut = NULL;
    m_easeCubeInOut = NULL;
    
    m_easeQuartIn = NULL;
    m_easeQuartOut = NULL;
    m_easeQuartInOut = NULL;
    
    m_easeQuintIn = NULL;
    m_easeQuintOut = NULL;
    m_easeQuintInOut = NULL;
    
    m_easeElasticIn = NULL;
    m_easeElasticOut = NULL;
    m_easeElasticInOut = NULL;
    
    m_easeBackIn = NULL;
    m_easeBackOut = NULL;
    m_easeBackInOut = NULL;
    
    m_easeBounceIn = NULL;
    m_easeBounceOut = NULL;
    m_easeBounceInOut = NULL;
};

void MathUtil::randomize(long newSeed)
{
    __randomizer->randomize(newSeed);
};

int MathUtil::rand()
{
    return __randomizer->rand();
};

float MathUtil::randf()
{
    return __randomizer->randf();
}

int MathUtil::randomInf(float Low, float High)
{
    return __randomizer->randomInf(Low, High);
}

int MathUtil::randomIn(int Low, int High)
{
    return __randomizer->randomIn(Low, High);
}

// Modeled after the line y = x
double MathUtil::LinearInterpolation(double p)
{
    return p;
}

// Modeled after the parabola y = x^2
double MathUtil::QuadraticEaseIn(double p)
{
    return p * p;
}

// Modeled after the parabola y = -x^2 + 2x
double MathUtil::QuadraticEaseOut(double p)
{
    return -(p * (p - 2));
}

// Modeled after the piecewise quadratic
// y = (1/2)((2x)^2)             ; [0, 0.5)
// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
double MathUtil::QuadraticEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 2 * p * p;
    }
    else
    {
        return (-2 * p * p) + (4 * p) - 1;
    }
}

// Modeled after the cubic y = x^3
double MathUtil::CubicEaseIn(double p)
{
    return p * p * p;
}

// Modeled after the cubic y = (x - 1)^3 + 1
double MathUtil::CubicEaseOut(double p)
{
    double f = (p - 1);
    return f * f * f + 1;
}

// Modeled after the piecewise cubic
// y = (1/2)((2x)^3)       ; [0, 0.5)
// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
double MathUtil::CubicEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 4 * p * p * p;
    }
    else
    {
        double f = ((2 * p) - 2);
        return 0.5 * f * f * f + 1;
    }
}

// Modeled after the quartic x^4
double MathUtil::QuarticEaseIn(double p)
{
    return p * p * p * p;
}

// Modeled after the quartic y = 1 - (x - 1)^4
double MathUtil::QuarticEaseOut(double p)
{
    double f = (p - 1);
    return f * f * f * (1 - p) + 1;
}

// Modeled after the piecewise quartic
// y = (1/2)((2x)^4)        ; [0, 0.5)
// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
double MathUtil::QuarticEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 8 * p * p * p * p;
    }
    else
    {
        double f = (p - 1);
        return -8 * f * f * f * f + 1;
    }
}

// Modeled after the quintic y = x^5
double MathUtil::QuinticEaseIn(double p)
{
    return p * p * p * p * p;
}

// Modeled after the quintic y = (x - 1)^5 + 1
double MathUtil::QuinticEaseOut(double p)
{
    double f = (p - 1);
    return f * f * f * f * f + 1;
}

// Modeled after the piecewise quintic
// y = (1/2)((2x)^5)       ; [0, 0.5)
// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
double MathUtil::QuinticEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 16 * p * p * p * p * p;
    }
    else
    {
        double f = ((2 * p) - 2);
        return  0.5 * f * f * f * f * f + 1;
    }
}

// Modeled after quarter-cycle of sine wave
double MathUtil::SineEaseIn(double p)
{
    return sin((p - 1) * M_PI_2) + 1;
}

// Modeled after quarter-cycle of sine wave (different phase)
double MathUtil::SineEaseOut(double p)
{
    return sin(p * M_PI_2);
}

// Modeled after half sine wave
double MathUtil::SineEaseInOut(double p)
{
    return 0.5 * (1 - cos(p * M_PI));
}

// Modeled after shifted quadrant IV of unit circle
double MathUtil::CircularEaseIn(double p)
{
    return 1 - sqrt(1 - (p * p));
}

// Modeled after shifted quadrant II of unit circle
double MathUtil::CircularEaseOut(double p)
{
    return sqrt((2 - p) * p);
}

// Modeled after the piecewise circular function
// y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
double MathUtil::CircularEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 0.5 * (1 - sqrt(1 - 4 * (p * p)));
    }
    else
    {
        return 0.5 * (sqrt(-((2 * p) - 3) * ((2 * p) - 1)) + 1);
    }
}

// Modeled after the exponential function y = 2^(10(x - 1))
double MathUtil::ExponentialEaseIn(double p)
{
    return (p == 0.0) ? p : pow(2, 10 * (p - 1));
}

// Modeled after the exponential function y = -2^(-10x) + 1
double MathUtil::ExponentialEaseOut(double p)
{
    return (p == 1.0) ? p : 1 - pow(2, -10 * p);
}

// Modeled after the piecewise exponential
// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
double MathUtil::ExponentialEaseInOut(double p)
{
    if(p == 0.0 || p == 1.0) return p;
    
    if(p < 0.5)
    {
        return 0.5 * pow(2, (20 * p) - 10);
    }
    else
    {
        return -0.5 * pow(2, (-20 * p) + 10) + 1;
    }
}

// Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
double MathUtil::ElasticEaseIn(double p)
{
    return sin(13 * M_PI_2 * p) * pow(2, 10 * (p - 1));
}

// Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
double MathUtil::ElasticEaseOut(double p)
{
    return sin(-13 * M_PI_2 * (p + 1)) * pow(2, -10 * p) + 1;
}

// Modeled after the piecewise exponentially-damped sine wave:
// y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
// y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
double MathUtil::ElasticEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 0.5 * sin(13 * M_PI_2 * (2 * p)) * pow(2, 10 * ((2 * p) - 1));
    }
    else
    {
        return 0.5 * (sin(-13 * M_PI_2 * ((2 * p - 1) + 1)) * pow(2, -10 * (2 * p - 1)) + 2);
    }
}

// Modeled after the overshooting cubic y = x^3-x*sin(x*pi)
double MathUtil::BackEaseIn(double p)
{
    return p * p * p - p * sin(p * M_PI);
}

// Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
double MathUtil::BackEaseOut(double p)
{
    double f = (1 - p);
    return 1 - (f * f * f - f * sin(f * M_PI));
}

// Modeled after the piecewise overshooting cubic function:
// y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
// y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
double MathUtil::BackEaseInOut(double p)
{
    if(p < 0.5)
    {
        double f = 2 * p;
        return 0.5 * (f * f * f - f * sin(f * M_PI));
    }
    else
    {
        double f = (1 - (2*p - 1));
        return 0.5 * (1 - (f * f * f - f * sin(f * M_PI))) + 0.5;
    }
}

double MathUtil::BounceEaseIn(double p)
{
    return 1 - BounceEaseOut(1 - p);
}

double MathUtil::BounceEaseOut(double p)
{
    if(p < 4/11.0)
    {
        return (121 * p * p)/16.0;
    }
    else if(p < 8/11.0)
    {
        return (363/40.0 * p * p) - (99/10.0 * p) + 17/5.0;
    }
    else if(p < 9/10.0)
    {
        return (4356/361.0 * p * p) - (35442/1805.0 * p) + 16061/1805.0;
    }
    else
    {
        return (54/5.0 * p * p) - (513/25.0 * p) + 268/25.0;
    }
}

double MathUtil::BounceEaseInOut(double p)
{
    if(p < 0.5)
    {
        return 0.5 * BounceEaseIn(p*2);
    }
    else
    {
        return 0.5 * BounceEaseOut(p * 2 - 1) + 0.5;
    }
}
void MathUtil::InitSin()
{
    //Pre-calculated sin array (1/4 degree each step)
    float sin_arr[] = {0.000,0.004,0.009,0.013,0.017,0.022,0.026,0.031,0.035,0.039,0.044,0.048,0.052,0.057,0.061,0.065,0.070,0.074,0.078,0.083,0.087,0.092,0.096,0.100,0.105,0.109,0.113,0.118,0.122,0.126,0.131,0.135,0.139,0.143,0.148,0.152,0.156,0.161,0.165,0.169,0.174,0.178,0.182,0.187,0.191,0.195,0.199,0.204,0.208,0.212,0.216,0.221,0.225,0.229,0.233,0.238,0.242,0.246,0.250,0.255,0.259,0.263,0.267,0.271,0.276,0.280,0.284,0.288,0.292,0.297,0.301,0.305,0.309,0.313,0.317,0.321,0.326,0.330,0.334,0.338,0.342,0.346,0.350,0.354,0.358,0.362,0.367,0.371,0.375,0.379,0.383,0.387,0.391,0.395,0.399,0.403,0.407,0.411,0.415,0.419,0.423,0.427,0.431,0.434,0.438,0.442,0.446,0.450,0.454,0.458,0.462,0.466,0.469,0.473,0.477,0.481,0.485,0.489,0.492,0.496,0.500,0.504,0.508,0.511,0.515,0.519,0.522,0.526,0.530,0.534,0.537,0.541,0.545,0.548,0.552,0.556,0.559,0.563,0.566,0.570,0.574,0.577,0.581,0.584,0.588,0.591,0.595,0.598,0.602,0.605,0.609,0.612,0.616,0.619,0.623,0.626,0.629,0.633,0.636,0.639,0.643,0.646,0.649,0.653,0.656,0.659,0.663,0.666,0.669,0.672,0.676,0.679,0.682,0.685,0.688,0.692,0.695,0.698,0.701,0.704,0.707,0.710,0.713,0.716,0.719,0.722,0.725,0.728,0.731,0.734,0.737,0.740,0.743,0.746,0.749,0.752,0.755,0.758,0.760,0.763,0.766,0.769,0.772,0.774,0.777,0.780,0.783,0.785,0.788,0.791,0.793,0.796,0.799,0.801,0.804,0.806,0.809,0.812,0.814,0.817,0.819,0.822,0.824,0.827,0.829,0.831,0.834,0.836,0.839,0.841,0.843,0.846,0.848,0.850,0.853,0.855,0.857,0.859,0.862,0.864,0.866,0.868,0.870,0.872,0.875,0.877,0.879,0.881,0.883,0.885,0.887,0.889,0.891,0.893,0.895,0.897,0.899,0.901,0.903,0.904,0.906,0.908,0.910,0.912,0.914,0.915,0.917,0.919,0.921,0.922,0.924,0.926,0.927,0.929,0.930,0.932,0.934,0.935,0.937,0.938,0.940,0.941,0.943,0.944,0.946,0.947,0.948,0.950,0.951,0.952,0.954,0.955,0.956,0.958,0.959,0.960,0.961,0.962,0.964,0.965,0.966,0.967,0.968,0.969,0.970,0.971,0.972,0.973,0.974,0.975,0.976,0.977,0.978,0.979,0.980,0.981,0.982,0.982,0.983,0.984,0.985,0.986,0.986,0.987,0.988,0.988,0.989,0.990,0.990,0.991,0.991,0.992,0.993,0.993,0.994,0.994,0.995,0.995,0.995,0.996,0.996,0.997,0.997,0.997,0.998,0.998,0.998,0.998,0.999,0.999,0.999,0.999,0.999,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,0.999,0.999,0.999,0.999,0.999,0.998,0.998,0.998,0.998,0.997,0.997,0.997,0.996,0.996,0.995,0.995,0.995,0.994,0.994,0.993,0.993,0.992,0.991,0.991,0.990,0.990,0.989,0.988,0.988,0.987,0.986,0.986,0.985,0.984,0.983,0.982,0.982,0.981,0.980,0.979,0.978,0.977,0.976,0.975,0.974,0.973,0.972,0.971,0.970,0.969,0.968,0.967,0.966,0.965,0.964,0.962,0.961,0.960,0.959,0.958,0.956,0.955,0.954,0.952,0.951,0.950,0.948,0.947,0.946,0.944,0.943,0.941,0.940,0.938,0.937,0.935,0.934,0.932,0.930,0.929,0.927,0.926,0.924,0.922,0.921,0.919,0.917,0.915,0.914,0.912,0.910,0.908,0.906,0.904,0.903,0.901,0.899,0.897,0.895,0.893,0.891,0.889,0.887,0.885,0.883,0.881,0.879,0.877,0.875,0.872,0.870,0.868,0.866,0.864,0.862,0.859,0.857,0.855,0.853,0.850,0.848,0.846,0.843,0.841,0.839,0.836,0.834,0.831,0.829,0.827,0.824,0.822,0.819,0.817,0.814,0.812,0.809,0.806,0.804,0.801,0.799,0.796,0.793,0.791,0.788,0.785,0.783,0.780,0.777,0.774,0.772,0.769,0.766,0.763,0.760,0.758,0.755,0.752,0.749,0.746,0.743,0.740,0.737,0.734,0.731,0.728,0.725,0.722,0.719,0.716,0.713,0.710,0.707,0.704,0.701,0.698,0.695,0.692,0.688,0.685,0.682,0.679,0.676,0.672,0.669,0.666,0.663,0.659,0.656,0.653,0.649,0.646,0.643,0.639,0.636,0.633,0.629,0.626,0.623,0.619,0.616,0.612,0.609,0.605,0.602,0.598,0.595,0.591,0.588,0.584,0.581,0.577,0.574,0.570,0.566,0.563,0.559,0.556,0.552,0.548,0.545,0.541,0.537,0.534,0.530,0.526,0.522,0.519,0.515,0.511,0.508,0.504,0.500,0.496,0.492,0.489,0.485,0.481,0.477,0.473,0.469,0.466,0.462,0.458,0.454,0.450,0.446,0.442,0.438,0.434,0.431,0.427,0.423,0.419,0.415,0.411,0.407,0.403,0.399,0.395,0.391,0.387,0.383,0.379,0.375,0.371,0.367,0.362,0.358,0.354,0.350,0.346,0.342,0.338,0.334,0.330,0.326,0.321,0.317,0.313,0.309,0.305,0.301,0.297,0.292,0.288,0.284,0.280,0.276,0.271,0.267,0.263,0.259,0.255,0.250,0.246,0.242,0.238,0.233,0.229,0.225,0.221,0.216,0.212,0.208,0.204,0.199,0.195,0.191,0.187,0.182,0.178,0.174,0.169,0.165,0.161,0.156,0.152,0.148,0.143,0.139,0.135,0.131,0.126,0.122,0.118,0.113,0.109,0.105,0.100,0.096,0.092,0.087,0.083,0.078,0.074,0.070,0.065,0.061,0.057,0.052,0.048,0.044,0.039,0.035,0.031,0.026,0.022,0.017,0.013,0.009,0.004,0.000,-0.004,-0.009,-0.013,-0.017,-0.022,-0.026,-0.031,-0.035,-0.039,-0.044,-0.048,-0.052,-0.057,-0.061,-0.065,-0.070,-0.074,-0.078,-0.083,-0.087,-0.092,-0.096,-0.100,-0.105,-0.109,-0.113,-0.118,-0.122,-0.126,-0.131,-0.135,-0.139,-0.143,-0.148,-0.152,-0.156,-0.161,-0.165,-0.169,-0.174,-0.178,-0.182,-0.187,-0.191,-0.195,-0.199,-0.204,-0.208,-0.212,-0.216,-0.221,-0.225,-0.229,-0.233,-0.238,-0.242,-0.246,-0.250,-0.255,-0.259,-0.263,-0.267,-0.271,-0.276,-0.280,-0.284,-0.288,-0.292,-0.297,-0.301,-0.305,-0.309,-0.313,-0.317,-0.321,-0.326,-0.330,-0.334,-0.338,-0.342,-0.346,-0.350,-0.354,-0.358,-0.362,-0.367,-0.371,-0.375,-0.379,-0.383,-0.387,-0.391,-0.395,-0.399,-0.403,-0.407,-0.411,-0.415,-0.419,-0.423,-0.427,-0.431,-0.434,-0.438,-0.442,-0.446,-0.450,-0.454,-0.458,-0.462,-0.466,-0.469,-0.473,-0.477,-0.481,-0.485,-0.489,-0.492,-0.496,-0.500,-0.504,-0.508,-0.511,-0.515,-0.519,-0.522,-0.526,-0.530,-0.534,-0.537,-0.541,-0.545,-0.548,-0.552,-0.556,-0.559,-0.563,-0.566,-0.570,-0.574,-0.577,-0.581,-0.584,-0.588,-0.591,-0.595,-0.598,-0.602,-0.605,-0.609,-0.612,-0.616,-0.619,-0.623,-0.626,-0.629,-0.633,-0.636,-0.639,-0.643,-0.646,-0.649,-0.653,-0.656,-0.659,-0.663,-0.666,-0.669,-0.672,-0.676,-0.679,-0.682,-0.685,-0.688,-0.692,-0.695,-0.698,-0.701,-0.704,-0.707,-0.710,-0.713,-0.716,-0.719,-0.722,-0.725,-0.728,-0.731,-0.734,-0.737,-0.740,-0.743,-0.746,-0.749,-0.752,-0.755,-0.758,-0.760,-0.763,-0.766,-0.769,-0.772,-0.774,-0.777,-0.780,-0.783,-0.785,-0.788,-0.791,-0.793,-0.796,-0.799,-0.801,-0.804,-0.806,-0.809,-0.812,-0.814,-0.817,-0.819,-0.822,-0.824,-0.827,-0.829,-0.831,-0.834,-0.836,-0.839,-0.841,-0.843,-0.846,-0.848,-0.850,-0.853,-0.855,-0.857,-0.859,-0.862,-0.864,-0.866,-0.868,-0.870,-0.872,-0.875,-0.877,-0.879,-0.881,-0.883,-0.885,-0.887,-0.889,-0.891,-0.893,-0.895,-0.897,-0.899,-0.901,-0.903,-0.904,-0.906,-0.908,-0.910,-0.912,-0.914,-0.915,-0.917,-0.919,-0.921,-0.922,-0.924,-0.926,-0.927,-0.929,-0.930,-0.932,-0.934,-0.935,-0.937,-0.938,-0.940,-0.941,-0.943,-0.944,-0.946,-0.947,-0.948,-0.950,-0.951,-0.952,-0.954,-0.955,-0.956,-0.958,-0.959,-0.960,-0.961,-0.962,-0.964,-0.965,-0.966,-0.967,-0.968,-0.969,-0.970,-0.971,-0.972,-0.973,-0.974,-0.975,-0.976,-0.977,-0.978,-0.979,-0.980,-0.981,-0.982,-0.982,-0.983,-0.984,-0.985,-0.986,-0.986,-0.987,-0.988,-0.988,-0.989,-0.990,-0.990,-0.991,-0.991,-0.992,-0.993,-0.993,-0.994,-0.994,-0.995,-0.995,-0.995,-0.996,-0.996,-0.997,-0.997,-0.997,-0.998,-0.998,-0.998,-0.998,-0.999,-0.999,-0.999,-0.999,-0.999,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-1.000,-0.999,-0.999,-0.999,-0.999,-0.999,-0.998,-0.998,-0.998,-0.998,-0.997,-0.997,-0.997,-0.996,-0.996,-0.995,-0.995,-0.995,-0.994,-0.994,-0.993,-0.993,-0.992,-0.991,-0.991,-0.990,-0.990,-0.989,-0.988,-0.988,-0.987,-0.986,-0.986,-0.985,-0.984,-0.983,-0.982,-0.982,-0.981,-0.980,-0.979,-0.978,-0.977,-0.976,-0.975,-0.974,-0.973,-0.972,-0.971,-0.970,-0.969,-0.968,-0.967,-0.966,-0.965,-0.964,-0.962,-0.961,-0.960,-0.959,-0.958,-0.956,-0.955,-0.954,-0.952,-0.951,-0.950,-0.948,-0.947,-0.946,-0.944,-0.943,-0.941,-0.940,-0.938,-0.937,-0.935,-0.934,-0.932,-0.930,-0.929,-0.927,-0.926,-0.924,-0.922,-0.921,-0.919,-0.917,-0.915,-0.914,-0.912,-0.910,-0.908,-0.906,-0.904,-0.903,-0.901,-0.899,-0.897,-0.895,-0.893,-0.891,-0.889,-0.887,-0.885,-0.883,-0.881,-0.879,-0.877,-0.875,-0.872,-0.870,-0.868,-0.866,-0.864,-0.862,-0.859,-0.857,-0.855,-0.853,-0.850,-0.848,-0.846,-0.843,-0.841,-0.839,-0.836,-0.834,-0.831,-0.829,-0.827,-0.824,-0.822,-0.819,-0.817,-0.814,-0.812,-0.809,-0.806,-0.804,-0.801,-0.799,-0.796,-0.793,-0.791,-0.788,-0.785,-0.783,-0.780,-0.777,-0.774,-0.772,-0.769,-0.766,-0.763,-0.760,-0.758,-0.755,-0.752,-0.749,-0.746,-0.743,-0.740,-0.737,-0.734,-0.731,-0.728,-0.725,-0.722,-0.719,-0.716,-0.713,-0.710,-0.707,-0.704,-0.701,-0.698,-0.695,-0.692,-0.688,-0.685,-0.682,-0.679,-0.676,-0.672,-0.669,-0.666,-0.663,-0.659,-0.656,-0.653,-0.649,-0.646,-0.643,-0.639,-0.636,-0.633,-0.629,-0.626,-0.623,-0.619,-0.616,-0.612,-0.609,-0.605,-0.602,-0.598,-0.595,-0.591,-0.588,-0.584,-0.581,-0.577,-0.574,-0.570,-0.566,-0.563,-0.559,-0.556,-0.552,-0.548,-0.545,-0.541,-0.537,-0.534,-0.530,-0.526,-0.522,-0.519,-0.515,-0.511,-0.508,-0.504,-0.500,-0.496,-0.492,-0.489,-0.485,-0.481,-0.477,-0.473,-0.469,-0.466,-0.462,-0.458,-0.454,-0.450,-0.446,-0.442,-0.438,-0.434,-0.431,-0.427,-0.423,-0.419,-0.415,-0.411,-0.407,-0.403,-0.399,-0.395,-0.391,-0.387,-0.383,-0.379,-0.375,-0.371,-0.367,-0.362,-0.358,-0.354,-0.350,-0.346,-0.342,-0.338,-0.334,-0.330,-0.326,-0.321,-0.317,-0.313,-0.309,-0.305,-0.301,-0.297,-0.292,-0.288,-0.284,-0.280,-0.276,-0.271,-0.267,-0.263,-0.259,-0.255,-0.250,-0.246,-0.242,-0.238,-0.233,-0.229,-0.225,-0.221,-0.216,-0.212,-0.208,-0.204,-0.199,-0.195,-0.191,-0.187,-0.182,-0.178,-0.174,-0.169,-0.165,-0.161,-0.156,-0.152,-0.148,-0.143,-0.139,-0.135,-0.131,-0.126,-0.122,-0.118,-0.113,-0.109,-0.105,-0.100,-0.096,-0.092,-0.087,-0.083,-0.078,-0.074,-0.070,-0.065,-0.061,-0.057,-0.052,-0.048,-0.044,-0.039,-0.035,-0.031,-0.026,-0.022,-0.017,-0.013,-0.009,-0.004};
    
    int SizeOfFloat = sizeof(float);
    m_sin = (float *)malloc(4*360*SizeOfFloat);
    //    m_cos = (float *)malloc(360 * SizeOfFloat);
    memcpy(m_sin, sin_arr, 4*360*SizeOfFloat);
    
}
void MathUtil::InitLinearInterpolation(){}
void MathUtil::InitQuadraticEaseIn(){}
void MathUtil::InitQuadraticEaseOut(){}
void MathUtil::InitQuadraticEaseInOut(){}
void MathUtil::InitCubicEaseIn(){}
void MathUtil::InitCubicEaseOut(){}
void MathUtil::InitCubicEaseInOut(){}
void MathUtil::InitQuarticEaseIn(){}
void MathUtil::InitQuarticEaseOut(){}
void MathUtil::InitQuarticEaseInOut(){}
void MathUtil::InitQuinticEaseIn(){}
void MathUtil::InitQuinticEaseOut(){}
void MathUtil::InitQuinticEaseInOut(){}
void MathUtil::InitSineEaseIn(){}
void MathUtil::InitSineEaseOut(){}
void MathUtil::InitSineEaseInOut(){}
void MathUtil::InitCircularEaseIn(){}
void MathUtil::InitCircularEaseOut(){}
void MathUtil::InitCircularEaseInOut(){}
void MathUtil::InitExponentialEaseIn(){}
void MathUtil::InitExponentialEaseOut(){}
void MathUtil::InitExponentialEaseInOut(){}
void MathUtil::InitElasticEaseIn(){}
void MathUtil::InitElasticEaseOut(){}
void MathUtil::InitElasticEaseInOut(){}
void MathUtil::InitBackEaseIn(){}
void MathUtil::InitBackEaseOut(){}
void MathUtil::InitBackEaseInOut(){}
void MathUtil::InitBounceEaseIn(){}
void MathUtil::InitBounceEaseOut(){}
void MathUtil::InitBounceEaseInOut(){}
