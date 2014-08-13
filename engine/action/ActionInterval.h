//
//  ActionInterval.h
//  jEngine
//
//  Created by Khanh Bui on 8/11/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__ActionInterval__
#define __jEngine__ActionInterval__

#include <iostream>
#include <functional>

#include "Action.h"

NAMESPACE_BEGIN

class Node;

class ActionInterval : public Action
{
public:
    ActionInterval();
    virtual ~ActionInterval();
    
    virtual bool isFinished() const;
    
    virtual void step(float elapsed);
    virtual void update(float t);
    
    virtual void setTarget(Node * target);
    virtual bool initWithDuration(float d);
    
    virtual void reset();
    
    float getElapsed() const;
    float getDuration() const;
    bool isFistTick() const;
    
    virtual void stop();
    
protected:
    float _elapsed;
    bool _firstTick;
    
    float _duration;
    
    Node * _target;
};

class Wait : public ActionInterval
{
private:
    
protected:
    
public:
    Wait();
    virtual ~Wait();
};

class MoveBy : public ActionInterval
{
private:
    
protected:
    float _deltaY;
    float _deltaX;
    
    float _startX;
    float _startY;
    
    float _previousX;
    float _previousY;
    
public:
    MoveBy();
    virtual ~MoveBy();
    
    virtual bool initWithDuration(float duration, float dx, float dy);
    
    virtual void setTarget(Node * target);
    virtual void update(float t);
};

class MoveTo : public MoveBy
{
private:
    
protected:
    float _endX;
    float _endY;
    
public:
    MoveTo();
    virtual ~MoveTo();
    
    virtual bool initWithDuration(float duration, float x, float y);
    
    virtual void setTarget(Node * target);
};

class ScaleTo : public ActionInterval
{
private:
    
protected:
    float _scaleX;
    float _scaleY;
    
    float _startScaleX;
    float _startScaleY;
    
    float _endScaleX;
    float _endScaleY;
    
    float _deltaX;
    float _deltaY;
    
public:
    ScaleTo();
    virtual ~ScaleTo();
    
    virtual bool initWithDuration(float duration, float s);
    virtual bool initWithDuration(float duration, float sx, float sy);
    
    virtual void setTarget(Node * target);
    virtual void update(float t);
};

class ScaleBy : public ScaleTo
{
private:
    
protected:
    
public:
    ScaleBy();
    virtual ~ScaleBy();
    
    virtual void setTarget(Node * target);
};

class RotateBy : public ActionInterval
{
private:
    float _deltaAngle;
    
    float _startAngle;
    
    float _previousAngle;
    
    float _endAngle;
    
protected:
    
public:
    RotateBy();
    virtual ~RotateBy();
    
    virtual bool initWithDuration(float duration, float deltaAngle);
    
    virtual void setTarget(Node * target);
    virtual void update(float t);
};

class CallFunc : public ActionInterval
{
protected:
	std::function<void()> _function;
    
public:
    CallFunc();
    virtual ~CallFunc();
    
    virtual void execute();
    virtual void update(float t);
    
    bool initWithFunction(const std::function<void()>& func);
};

class Sequence : public ActionInterval
{
private:
    ActionInterval * _actions[2];
    
    int _last;
    float _halfPart;
    
    bool _initWithTwoActions(ActionInterval * action1, ActionInterval * action2);
    
protected:
    
public:
    Sequence();
    virtual ~Sequence();
    
    virtual void step(float elapsed);
    virtual void update(float t);
    
    bool initWithActions(ActionInterval * action, ...);
    
    virtual void reset();
    virtual void setTarget(Node * target);
};

class Parallel : public ActionInterval
{
private:
    ActionInterval * _actions[2];
    
    bool _initWithTwoActions(ActionInterval * action1, ActionInterval * action2);
    
protected:
    
public:
    Parallel();
    ~Parallel();
    
    virtual void update(float t);
    
    bool initWithActions(ActionInterval * action, ...);
    
    virtual void reset();
    virtual void setTarget(Node * target);
};

class RepeatForever : public ActionInterval
{
private:
    
protected:
    ActionInterval * _inner;
    
public:
    RepeatForever();
    virtual ~RepeatForever();
    
    bool initWithAction(ActionInterval * action);
    
    virtual void setTarget(Node * target);
    virtual void step(float elapsed);
    virtual bool isFinished(void) const;
};

class FadeTo : public ActionInterval
{
private:
    
protected:
    float _fromAlpha;
    float _toAlpha;
    
public:
    FadeTo();
    virtual ~FadeTo();
    
    virtual bool initWithDuration(float duration);
    virtual bool initWithDuration(float duration, float alpha);
    
    virtual void setTarget(Node * target);
    virtual void update(float t);
};

class FadeIn : public FadeTo
{
private:
    
protected:
    
public:
    FadeIn();
    virtual ~FadeIn();
    
    virtual void setTarget(Node * target);
};

class FadeOut : public FadeTo
{
private:
    
protected:
    
public:
    FadeOut();
    virtual ~FadeOut();
    
    virtual void setTarget(Node * target);
};

class ActionEase : public ActionInterval
{
private:
    
protected:
    ActionInterval * _inner;
    
public:
    ActionEase();
    virtual ~ActionEase();
    
    virtual bool initWithAction(ActionInterval * action);
    
    virtual void setTarget(Node * target);
    
    virtual void stop();
    virtual void update(float time);
};

class EaseElastic : public ActionEase
{
private:
    
protected:
    float _period;
    
public:
    EaseElastic();
    virtual ~EaseElastic();
    
    virtual bool initWithAction(ActionInterval * action, float period = 0.3f);
};

class EaseElasticOut : public EaseElastic
{
private:
    
protected:
    
public:
    EaseElasticOut();
    virtual ~EaseElasticOut();
    
    virtual void update(float t);
};

class JumpTo: public ActionInterval
{
private:
    float _height;
    float _jumps;
    
    float _deltaY;
    float _deltaX;
    
    float _startX;
    float _startY;
    
    float _previousX;
    float _previousY;
    
protected:
    
public:
    JumpTo();
    virtual ~JumpTo();
    
    virtual bool initWithDuration(float duration, float x, float y, float height, int jumps);
    
    virtual void setTarget(Node * target);
    virtual void update(float t);
};

NAMESPACE_END

#endif /* defined(__jEngine__ActionInterval__) */
