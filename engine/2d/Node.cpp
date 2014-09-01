//
//  Node.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Node.h"
#include "Mat4.h"
#include "Engine.h"
#include "GL.h"

NAMESPACE_USING

Node::Node() : Ref("Node"),
_x(0.0f),
_y(0.0f),
_z(0.0f),
_angle(0.0f),
_scaleX(1.0f),
_scaleY(1.0f),
_anchorX(0.0f),
_anchorY(0.0f),
w(0.0f),
h(0.0f),
alpha(1.0f),
active(false),
visible(true),
_parent(NULL),
_isDirty(true)
{
    _childrend = new std::vector<Node *>();
    _childrend->reserve(10);

    _mvpMat = new Mat4(Mat4::IDENTITY);
}

Node::~Node()
{
    while (!_childrend->empty())
    {
        Node * child = *(_childrend->begin());
        _childrend->erase(_childrend->begin());

        RELEASE(child);
    }

    RELEASE(_mvpMat);

    _parent = NULL;
}

void Node::update(float Elapsed)
{
    if (!active)
    {
        return;
    }

    _update(Elapsed);
}

void Node::render()
{
    if (!visible)
    {
        return;
    }

    if (_isDirty)
    {
        __updateMatrix();
        
        for (std::vector<Node *>::iterator it = _childrend->begin(); it != _childrend->end(); it++)
        {
            (*it)->_isDirty = true;
        }
        _isDirty = false;
    }

    _draw();

    for (std::vector<Node *>::iterator it = _childrend->begin(); it != _childrend->end(); it++)
    {
        (*it)->render();
    }
}

void Node::addChild(Node * Child)
{
    for (std::vector<Node *>::iterator it = _childrend->begin(); it != _childrend->end(); it++)
    {
        if (*it == Child)
        {
            ASSERT(false, "Child %s has been added.", Child->getName());
        }
    }

    _childrend->push_back(Child);

    Child->_parent = this;
    Child->retain();
}

void Node::removeChild(Node * Child)
{
    for (std::vector<Node *>::iterator it = _childrend->begin(); it != _childrend->end(); it++)
    {
        if (*it == Child)
        {
            _childrend->erase(it);

            Child->_parent = NULL;
            Child->release();

            return;
        }
    }

    ASSERT(false, "Child %s has not been added.", Child->getName());
}

void Node::removeChildren()
{
    for (std::vector<Node *>::iterator it = _childrend->begin(); it != _childrend->end(); it++)
    {
        _childrend->erase(it);

        Node * child = *it;
        child->_parent = NULL;
        child->release();
    }
}

void Node::setPosition(float X, float Y)
{
    _x = X;
    _y = Y;

    _isDirty = true;
}

float Node::getX() const
{
    return _x;
}

float Node::getY() const
{
    return _y;
}

void Node::setRotation(float Angle)
{
    _angle = Angle;
    _isDirty = true;
}

float Node::getRotation() const
{
    return _angle;
}

void Node::setScale(float Sx, float Sy)
{
    _scaleX = Sx;
    _scaleY = Sy;
    _isDirty = true;
}

void Node::setScale(float S)
{
    setScale(S, S);
}

float Node::getScaleX() const
{
    return _scaleX;
}

float Node::getScaleY() const
{
    return _scaleY;
}

void Node::setAnchor(float AnchorX, float AnchorY)
{
    _anchorX = GET_MAX(0.0f, GET_MIN(AnchorX, 1.0f));
    _anchorY = GET_MAX(0.0f, GET_MIN(AnchorY, 1.0f));
    _isDirty = true;
}

Node * Node::getParent() const
{
    return _parent;
}



void Node::_update(float Elapsed)
{
}

void Node::_draw()
{
}



void Node::__updateMatrix()
{
    if (_parent)
    {
        _mvpMat->set(_parent->_mvpMat);
    }
    else
    {
        _mvpMat->set(Engine::Instance()->renderer()->mvp());
    }

    float dx = _anchorX * w;
    float dy = _anchorY * h;

    _mvpMat->translate(_x, _y, _z);

    _mvpMat->translate(dx, dy, 0.0f);
    _mvpMat->rotateZ(PI / 180.0f * _angle);
    _mvpMat->translate(-dx, -dy, 0.0f);

    _mvpMat->translate(dx, dy, 0.0f);
    _mvpMat->scale(_scaleX, _scaleY, 1.0f);
    _mvpMat->translate(-dx, -dy, 0.0f);
}
