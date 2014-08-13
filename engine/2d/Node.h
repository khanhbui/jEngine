//
//  Node.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Node__
#define __jEngine__Node__

#include "Ref.h"

NAMESPACE_BEGIN

class Mat4;

class Node : public Ref
{
private:
    void __updateMatrix();

protected:
    float _x;
    float _y;
    float _z;
    
    float _angle;
    
    float _scaleX;
    float _scaleY;

    float _anchorX;
    float _anchorY;

    std::vector<Node *> * _childrend;
    Node * _parent;

    bool _isDirty;
    Mat4 * _mvpMat;

    virtual void _update(float Elapsed);
    virtual void _draw();

public:
    int w;
    int h;

    float alpha;

    bool active;
    bool visible;

    Node();
    virtual ~Node();

    virtual void update(float Elapsed);
    virtual void render();

    virtual void addChild(Node * Child);
    virtual void removeChild(Node * Child);

    virtual void setPosition(float X, float Y);
    float getX() const;
    float getY() const;

    virtual void setRotation(float Angle);
    float getRotation() const;

    virtual void setScale(float Sx, float Sy);
    virtual void setScale(float S);
    float getScaleX() const;
    float getScaleY() const;

    virtual void setAnchor(float AnchorX, float AnchorY);
};

NAMESPACE_END

#endif /* defined(__jEngine__Node__) */
