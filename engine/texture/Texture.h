//
//  Texture.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Texture__
#define __jEngine__Texture__

#include "Ref.h"

NAMESPACE_BEGIN

class Texture : public Ref
{
private:
    static unsigned int __id;

protected:

public:
    Texture();
    Texture(const char * FileName, int W, int H, float U0, float V0, float U1, float V1, unsigned int TextureId);
    ~Texture();

    int id;

    int w;
    int h;

    char * fileName;

    unsigned int m_textureId;

    float u0;
    float v0;

    float u1;
    float v1;
};

NAMESPACE_END

#endif /* defined(__jEngine__Texture__) */
