//
//  Font.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/23/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Font__
#define __jEngine__Font__

#include "Node.h"

NAMESPACE_BEGIN

class Texture;

class Font : public Node
{
private:
    char * __text;
    Texture ** __images;
    
    int * __charWidth;
    int * __charHeight;
    int __border;
    unsigned int __align;

    void __calculateWidth();

protected:
    virtual void _update(float Elapsed);
    virtual void _draw();

public:
    enum {
        ALIGN_TOP = 1 << 0,
        ALIGN_MIDDLE = 1 << 1,
        ALIGN_BOTTOM = 1 << 2,
        ALIGN_LEFT = 1 << 3,
        ALIGN_CENTER = 1 << 4,
        ALIGN_RIGHT = 1 << 5,
    };

    Font();
    virtual ~Font();

    void load(const char * FileName, const int &Border, const int &Align);

    void setText(const char * Text);
    void setText(const int &Number);
};

NAMESPACE_END

#endif /* defined(__jEngine__Font__) */
