//
//  ActionInterval.cpp
//  jEngine
//
//  Created by Khanh Bui on 8/11/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "ActionInterval.h"
#include "Node.h"

#include <cmath>
#include <float.h>

NAMESPACE_USING

ActionInterval::ActionInterval(): Action(), _duration(0.0f), _elapsed(0.0f)
{
}

ActionInterval::~ActionInterval()
{
    _target = NULL;
}

void ActionInterval::setTarget(Node * target)
{
    _target = target;
    
    _elapsed = 0.0f;
    _firstTick = true;
}

bool ActionInterval::initWithDuration(float d)
{
    _duration = d;
    if (_duration == 0)
    {
        _duration = FLT_EPSILON;
    }
    
    _elapsed = 0.0f;
    _firstTick = true;
    
    return true;
}

void ActionInterval::step(float elapsed)
{
    if (_firstTick)
    {
        _firstTick = false;
        _elapsed = 0.0f;
    }
    else
    {
        _elapsed += elapsed;
    }
    
    float t = GET_MAX(_duration, FLT_EPSILON);
    float t1 = GET_MIN(1.0f, _elapsed / t);
    float t2 = GET_MAX(0.0f, t1);
    
    update(t2);
}

void ActionInterval::update(float t)
{
}

bool ActionInterval::isFinished() const
{
    return _elapsed >= _duration;
}

void ActionInterval::reset()
{
    _duration = 0.0f;
    _elapsed = 0.0f;
    _firstTick = true;
}

float ActionInterval::getElapsed() const
{
    return _elapsed;
}

float ActionInterval::getDuration() const
{
    return _duration;
}

bool ActionInterval::isFistTick() const
{
    return _firstTick;
}

void ActionInterval::stop()
{
    _target = NULL;
}



Wait::Wait(): ActionInterval()
{
}

Wait::~Wait()
{
}



MoveBy::MoveBy() : ActionInterval()
{
    _deltaX = 0;
    _deltaY = 0;
    
    _startX = 0;
    _startY = 0;
    
    _previousX = 0;
    _previousY = 0;
}

MoveBy::~MoveBy()
{
}

bool MoveBy::initWithDuration(float duration, float dx, float dy)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _deltaX = dx;
        _deltaY = dy;
        
        return true;
    }
    
    return false;
}

void MoveBy::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    
    _previousX = _startX = _target->getX();
    _previousY = _startY = _target->getY();
}

void MoveBy::update(float t)
{
    if (_target)
    {
        float currentX = _target->getX();
        float currentY = _target->getY();
        
        float dx = currentX - _previousX;
        float dy = currentY - _previousY;
        
        _startX += dx;
        _startY += dy;
        
        float newX = _startX + (_deltaX * t);
        float newY = _startY + (_deltaY * t);
        
        _target->setPosition(newX, newY);
        
        _previousX = newX;
        _previousY = newY;
    }
}



MoveTo::MoveTo(): MoveBy()
{
    _endX = 0;
    _endY = 0;
}

MoveTo::~MoveTo()
{
}

bool MoveTo::initWithDuration(float duration, float x, float y)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _endX = x;
        _endY = y;
        
        return true;
    }
    
    return false;
}

void MoveTo::setTarget(Node * target)
{
    MoveBy::setTarget(target);
    
    _deltaX = _endX - _startX;
    _deltaY = _endY - _startY;
}



ScaleTo::ScaleTo() : ActionInterval()
{
}

ScaleTo::~ScaleTo()
{
}

bool ScaleTo::initWithDuration(float duration, float s)
{
    return initWithDuration(duration, s, s);
}

bool ScaleTo::initWithDuration(float duration, float sx, float sy)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _endScaleX = sx;
        _endScaleY = sy;
        
        return true;
    }
    return false;
}

void ScaleTo::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    
    _startScaleX = target->getScaleX();
    _startScaleY = target->getScaleY();
    
    _deltaX = _endScaleX - _startScaleX;
    _deltaY = _endScaleY - _startScaleY;
}

void ScaleTo::update(float t)
{
    if (_target)
    {
        _target->setScale(_startScaleX + _deltaX * t, _startScaleY + _deltaY * t);
    }
}

ScaleBy::ScaleBy() : ScaleTo()
{
}

ScaleBy::~ScaleBy()
{
}

void ScaleBy::setTarget(Node * target)
{
    ScaleTo::setTarget(target);
    
    _deltaX = _startScaleX * _endScaleX - _startScaleX;
    _deltaY = _startScaleY * _endScaleY - _startScaleY;
}



RotateBy::RotateBy()
{
}

RotateBy::~RotateBy()
{
}

bool RotateBy::initWithDuration(float duration, float deltaAngle)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _deltaAngle = deltaAngle;
        return true;
    }
    
    return false;
}

void RotateBy::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    
    _previousAngle = _startAngle = _target->getRotation();
}

void RotateBy::update(float t)
{
    if (_target)
    {
        float currentAngle = _target->getRotation();
        
        float da = currentAngle - _previousAngle;
        
        _startAngle += da;
        
        float newAngle = _startAngle + (_deltaAngle * t);
        
        _target->setRotation(newAngle);
        
        _previousAngle = newAngle;
    }
}



CallFunc::CallFunc() : ActionInterval(), _function(NULL)
{
}

CallFunc::~CallFunc()
{
    _function = NULL;
}

void CallFunc::execute()
{
}

void CallFunc::update(float t)
{
    if (_function != NULL)
    {
        _function();
    }
}

bool CallFunc::initWithFunction(const std::function<void()>& func)
{
    if (initWithDuration(0))
    {
        _function = func;
        return true;
    }
    return false;
}



Sequence::Sequence() : ActionInterval()
{
    _actions[0] = NULL;
    _actions[1] = NULL;
    
    _last = -1;
    _halfPart = 0;
}

Sequence::~Sequence()
{
    RELEASE(_actions[0]);
    RELEASE(_actions[1]);
}

bool Sequence::_initWithTwoActions(ActionInterval * action1, ActionInterval * action2)
{
    float d = action1->getDuration() + action2->getDuration();
    if (initWithDuration(d))
    {
        action1->retain();
        RELEASE(_actions[0]);
        _actions[0] = action1;
        
        action2->retain();
        RELEASE(_actions[1]);
        _actions[1] = action2;
        
        return true;
    }
    
    return false;
}

bool Sequence::initWithActions(ActionInterval * action, ...)
{
    va_list actions;
    va_start(actions, action);
    
    ActionInterval * action1 = action;
    
    while (action1 != NULL)
    {
        ActionInterval * action2 = va_arg(actions, ActionInterval *);
        if (action2 != NULL)
        {
            Sequence * tmp = new Sequence();
            tmp->autorelease();
            tmp->_initWithTwoActions(action1, action2);
            action1 = tmp;
        }
        else
        {
            ActionInterval * tmp = new ActionInterval();
            tmp->autorelease();
            _initWithTwoActions(action1, tmp);
            break;
        }
    }
    
    va_end(actions);
    
    return false;
}

void Sequence::step(float elapsed)
{
    ActionInterval::step(elapsed);
}

void Sequence::update(float t)
{
    int index = 0;
    float newT = 0.0f;
    
    if (t < _halfPart)
    {
        index = 0;
        newT = (_halfPart != 0) ? t / _halfPart : 1.0f;
    }
    else
    {
        index = 1;
        newT = (_halfPart != 1) ? (t - _halfPart) / (1.0f - _halfPart) : 1.0f;
    }
    
    if (index == 1)
    {
        if (_last == -1)
        {
            _actions[0]->setTarget(_target);
            _actions[0]->update(1.0f);
            _actions[0]->stop();
        }
        else if (_last == 0)
        {
            _actions[0]->update(1.0f);
            _actions[0]->stop();
        }
    }
    else if (index == 0 && _last == 1)
	{
		_actions[1]->update(0);
		_actions[1]->stop();
	}
    
    if (index == _last && _actions[index]->isFinished())
    {
        return;
    }
    
    if (_last != index)
    {
        _actions[index]->setTarget(_target);
        _last = index;
    }
    
    _actions[index]->update(newT);
}

void Sequence::reset()
{
    ActionInterval::reset();
    
    for (int i = 0; i < 2; i++)
    {
        if (_actions[i])
        {
            _actions[i]->reset();
        }
    }
}

void Sequence::setTarget(Node *target)
{
    ActionInterval::setTarget(target);
    
    _last = -1;
    _halfPart = _actions[0]->getDuration() / _duration;
}



Parallel::Parallel() : ActionInterval()
{
    _actions[0] = NULL;
    _actions[1] = NULL;
}

Parallel::~Parallel()
{
    RELEASE(_actions[0]);
    RELEASE(_actions[1]);
}

bool Parallel::_initWithTwoActions(ActionInterval * action1, ActionInterval * action2)
{
    float d1 = action1->getDuration();
    float d2 = action2->getDuration();
    float diff = d1 - d2;
    
    if (initWithDuration(diff > 0 ? d1 : d2))
    {
        action1->retain();
        RELEASE(_actions[0]);
        _actions[0] = action1;
        
        action2->retain();
        RELEASE(_actions[1]);
        _actions[1] = action2;
        
        return true;
    }
    
    return false;
}

bool Parallel::initWithActions(ActionInterval * action, ...)
{
    va_list actions;
    va_start(actions, action);
    
    ActionInterval * action1 = action;
    
    while (action1 != NULL)
    {
        ActionInterval * action2 = va_arg(actions, ActionInterval *);
        if (action2 != NULL)
        {
            Parallel * tmp = new Parallel();
            tmp->autorelease();
            tmp->_initWithTwoActions(action1, action2);
            action1 = tmp;
        }
        else
        {
            ActionInterval * tmp = new ActionInterval();
            tmp->autorelease();
            _initWithTwoActions(action1, tmp);
            break;
        }
    }
    
    va_end(actions);
    
    return false;
}

void Parallel::update(float t)
{
    for (int i = 0; i < 2; i++)
    {
        ActionInterval * action = _actions[i];
        if (action != NULL && !action->isFinished())
        {
            action->update(t);
        }
    }
}

void Parallel::reset()
{
    ActionInterval::reset();
    
    for (int i = 0; i < 2; i++)
    {
        if (_actions[i])
        {
            _actions[i]->reset();
        }
    }
}

void Parallel::setTarget(Node *target)
{
    ActionInterval::setTarget(target);
    
    for (int i = 0; i < 2; i++)
    {
        if (_actions[i])
        {
            _actions[i]->setTarget(_target);
        }
    }
}



RepeatForever::RepeatForever() : ActionInterval(), _inner(NULL)
{
}

RepeatForever::~RepeatForever()
{
}

bool RepeatForever::initWithAction(ActionInterval * action)
{
    _inner = action;
    return true;
}

void RepeatForever::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    _inner->setTarget(target);
}

void RepeatForever::step(float elapsed)
{
    _inner->step(elapsed);
    if (_inner->isFinished())
    {
        float duration = _inner->getDuration();
        float diff = _inner->getElapsed() - duration;
        if (diff > duration)
        {
            diff = fmodf(diff, duration);
        }
        _inner->setTarget(_target);
        _inner->step(0.0f);
        _inner->step(diff);
    }
}

bool RepeatForever::isFinished(void) const
{
    return false;
}



FadeTo::FadeTo() : ActionInterval(), _fromAlpha(0), _toAlpha(0)
{
}

FadeTo::~FadeTo()
{
}

bool FadeTo::initWithDuration(float duration)
{
    return ActionInterval::initWithDuration(duration);
}

bool FadeTo::initWithDuration(float duration, float alpha)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _toAlpha = alpha;
        return true;
    }
    return false;
}

void FadeTo::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    
    _fromAlpha = _target->alpha;
}

void FadeTo::update(float t)
{
    if (_target)
    {
        _target->alpha = _fromAlpha + (_toAlpha - _fromAlpha) * t;
    }
}



FadeIn::FadeIn() : FadeTo()
{
}

FadeIn::~FadeIn()
{
}

void FadeIn::setTarget(Node * target)
{
    FadeTo::setTarget(target);
    
    _toAlpha = 1.0f;
}



FadeOut::FadeOut() : FadeTo()
{
}

FadeOut::~FadeOut()
{
}

void FadeOut::setTarget(Node * target)
{
    FadeTo::setTarget(target);
    
    _toAlpha = 0.0f;
}



ActionEase::ActionEase() : ActionInterval(), _inner(NULL)
{
}

ActionEase::~ActionEase()
{
    _inner = NULL;
}

bool ActionEase::initWithAction(ActionInterval * action)
{
    if (ActionInterval::initWithDuration(action->getDuration()))
    {
        _inner = action;
        
        return true;
    }
    return false;
}

void ActionEase::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    _inner->setTarget(target);
}

void ActionEase::stop()
{
    _inner->stop();
    ActionInterval::stop();
}

void ActionEase::update(float t)
{
    _inner->update(t);
}



EaseElastic::EaseElastic() : ActionEase()
{
}

EaseElastic::~EaseElastic()
{
}

bool EaseElastic::initWithAction(ActionInterval * action, float period)
{
    if (ActionEase::initWithAction(action))
    {
        _period = period;
        return true;
    }
    return false;
}



EaseElasticOut::EaseElasticOut() : EaseElastic()
{
}

EaseElasticOut::~EaseElasticOut()
{
}

void EaseElasticOut::update(float t)
{
    float newT = 0;
    if (t == 0 || t == 1)
    {
        newT = t;
    }
    else
    {
        float s = _period / 4;
        newT = powf(2, -10 * t) * sinf((t - s) * M_PI * 2 / _period) + 1;
    }
    
    _inner->update(newT);
}


JumpTo::JumpTo() : ActionInterval()
{
}

JumpTo::~JumpTo()
{
}

bool JumpTo::initWithDuration(float duration, float x, float y, float height, int jumps)
{
    if (ActionInterval::initWithDuration(duration) && jumps >= 0)
    {
        _deltaX = x;
        _deltaY = y;
        
        _height = height;
        _jumps = jumps;
        
        return true;
    }
    
    return false;
}

void JumpTo::setTarget(Node * target)
{
    ActionInterval::setTarget(target);
    
    _previousX = _startX = _target->getX();
    _previousY = _startY = _target->getY();
    
    _deltaX = _deltaX - _startX;
    _deltaY = _deltaY - _startY;
}

void JumpTo::update(float t)
{
    if (_target)
    {
        float frac = fmodf(t * _jumps, 1.0f);
        float y = _height * 4.0f * frac * (1.0f - frac);
        y -= _deltaY * t;
        
        float x = _deltaX * t;
        
        float currentX = _target->getX();
        float currentY = _target->getY();
        
        float dx = currentX - _previousX;
        float dy = currentY - _previousY;
        
        _startX += dx;
        _startY += dy;
        
        float newX = _startX + x;
        float newY = _startY - y;
        
        _target->setPosition(newX, newY);
        
        _previousX = newX;
        _previousY = newY;
    }
}