//
//  Texture.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Texture.h"

NAMESPACE_USING

unsigned int Texture::__id = 0;

Texture::Texture() : Ref("Texture"),
id(__id++),
fileName(NULL),
w(0),
h(0),
u0(0.0f),
v0(0.0f),
u1(0.0f),
v1(0.0f),
m_textureId(0)
{
}

Texture::Texture(const char * FileName, int W, int H, float U0, float V0, float U1, float V1, unsigned int TextureId) : Ref("Texture"),
id(__id++),
w(W),
h(H),
u0(U0),
v0(V0),
u1(U1),
v1(V1),
m_textureId(TextureId)
{
    fileName = new char[strlen(FileName)];
    strcpy(fileName, FileName);
}

Texture::~Texture()
{
    DELETE_ARRAY(fileName);
}