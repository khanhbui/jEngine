//
//  GL.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__GL__
#define __jEngine__GL__

#include "Ref.h"
#include <map>

NAMESPACE_BEGIN

class Texture;
class Mat4;

class GL : public Ref
{
public:
    int __fitWidth;
    int __fitHeight;

    int __offsetX;
    int __offsetY;

private:
    std::map<unsigned int,short *> __vertex_buffer_map;
    std::map<unsigned int,float *> __texcoord_buffer_map;
    std::map<unsigned int,float *> __alpha_buffer_map;
    std::map<unsigned int,int> __queue_index_map;

    int __width;
    int __height;

    int __upScaleWidth;
    int __upScaleHeight;
    
    float __upscaleTextureU;
    float __upscaleTextureV;
    
    float __offscreenTextureU;
    float __offscreenTextureV;

    unsigned int __texture;

    float __ratio;
    
    int __pictScaleRatio;

    unsigned int __colorRenderBufferId;
    unsigned int __defaultFrameBufferId;
    unsigned int __offscreenFrameBufferId;
    unsigned int __offscreenTextureId;
    
    unsigned int __offscreenFrameBufferId_1;
    unsigned int __offscreenTextureId_1;
    
    unsigned int __progLoc;
    unsigned int __posLoc;
    unsigned int __texLoc;
    unsigned int __sampLoc;
    unsigned int __matLoc;
    unsigned int __alpLoc;
    
    unsigned int __progLoc_1;
    unsigned int __posLoc_1;
    unsigned int __texLoc_1;
    unsigned int __sampLoc_1;
    unsigned int __matLoc_1;
    unsigned int __alpLoc_1;
    
    unsigned short * __orderBuffer;
    short * __vertexBuffer;
    float * __texcoordBuffer;
    float * __alphaBuffer;
    int __queueIndex;

    Mat4 * __ortho2dMatrixObjects; // use in logic rendering and render to offscreen buffer

    Mat4 * __ortho2dMatrixScale;
    Mat4 * __ortho2dMatrixScreen; // use to render offscreen to screen
    
    float __colorf;

    void __renderToOffsceen();
    void __renderToUpscale();
    void __renderToSceen();

public:
    GL();
    virtual ~GL();

    void setupGL();
    void setTextureAtlas(unsigned int TextureId);

    void resize(int width, int height);

    void render();

    void cmdRender(int dest_x0, int dest_y0, int dest_x1, int dest_y1, float tex_u0, float tex_v0, float tex_u1, float tex_v1, float alpha, unsigned int texture = 0);
    void cmdRender(Mat4 * Mvp, Texture * Image, int W, int H, float Alpha);

    Mat4 * mvp();
};

NAMESPACE_END

#endif /* defined(__jEngine__GL__) */
