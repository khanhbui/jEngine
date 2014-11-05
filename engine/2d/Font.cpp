//
//  Font.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/23/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Font.h"
#include "Engine.h"
#include "Texture.h"

NAMESPACE_USING

Font::Font() : Node(),
__text(NULL),
__border(0),
__align(ALIGN_TOP | ALIGN_LEFT)
{
    __images = new Texture*[255];

    __charWidth = new int[255];
    __charHeight = new int[255];
}

Font::~Font()
{
    DELETE_ARRAY(__text);
    DELETE_ARRAY(__images);
    DELETE_ARRAY(__charWidth);
    DELETE_ARRAY(__charHeight);
}

void Font::__calculateWidth()
{
    size_t len;
    if (!__text || (len = strlen(__text)) == 0)
    {
        w = 0;
        h = 0;
    }
    else
    {
        w = static_cast<int>((len - 1) * __border);
        h = 0;
        for (int i = 0; i < len; i++)
        {
            int index = __text[i];
            w += __charWidth[index];
            if (h < __charHeight[index])
            {
                h = __charHeight[index];
            }
        }
    }
}

void Font::_update(float Elapsed)
{
}

void Font::_draw()
{
    if (!__text)
    {
        return;
    }

    int xx = 0;
    if (__align & ALIGN_RIGHT)
    {
        xx = -w;
    }
    else if (__align & ALIGN_CENTER)
    {
        xx = -w >> 1;
    }

    int yy = 0;
    if (__align & ALIGN_BOTTOM)
    {
        yy = -h;
    }
    else if (__align & ALIGN_MIDDLE)
    {
        yy = -h >> 1;
    }

    for (size_t i = 0, len = strlen(__text); i < len; i++)
    {
        char index = __text[i];
        Texture * image = __images[index];

        if (i == len - 1)
        {
            Engine::Instance()->renderer()->cmdRender(_mvpMat, image, xx, yy, __charWidth[index], __charHeight[index], alpha);
        }
        else
        {
            Engine::Instance()->renderer()->cmdRender(xx, yy, xx + __charWidth[index], yy + __charHeight[index], image->u0, image->v0, image->u1, image->v1, alpha, image->textureId);
        }
        xx += __charWidth[index] + __border;
    }
}

void Font::setText(const char * Text)
{
    DELETE_ARRAY(__text);
    __text = new char[strlen(Text)];
    sprintf(__text, "%s", Text);

    __calculateWidth();
}

void Font::setText(const int &Number)
{
    DELETE_ARRAY(__text);
    __text = new char[20];
    sprintf(__text, "%d", Number);

    __calculateWidth();
}

void Font::load(const char * FileName, const int &Border, const int &Align)
{
    __border = Border;
    __align = Align;

    int count;
    Texture ** images = Engine::Instance()->findImages(FileName, count);
    for (int i = 0; i < count; i++)
    {
        char * name = images[i]->fileName;
        name += strlen(name) - 1;
        while (name[0] >= '0' && name[0] <= '9')
        {
            name--;
        }
        name++;

        int index;
        sscanf(name, "%d", &index);

        __images[index] = images[i];

        __charWidth[index] = images[i]->w;
        __charHeight[index] = images[i]->h;
    }
}