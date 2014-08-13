//
//  GL.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "GL.h"
#include "Engine.h"
#include "Texture.h"
#include "Math.h"
#include "Mat4.h"
#include "SceneDirector.h"

#include <math.h>

#ifdef IOS
#include <OpenGLES/ES2/gl.h>

#elif ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

NAMESPACE_USING

GL::GL()
{
    __orderBuffer = (GLushort *) malloc(MAX_QUADS * sizeof(GLushort) * 6);
    setTextureAtlas(0);
    
    for (int i = 0; i < MAX_QUADS; i++)
    {
        __orderBuffer[(i * 6) + 0] = i * 4;
        __orderBuffer[(i * 6) + 1] = i * 4 + 1;
        __orderBuffer[(i * 6) + 2] = i * 4 + 2;
        __orderBuffer[(i * 6) + 3] = i * 4;
        __orderBuffer[(i * 6) + 4] = i * 4 + 2;
        __orderBuffer[(i * 6) + 5] = i * 4 + 3;
    }
    
}

GL::~GL()
{
    free(__orderBuffer);

    std::map<unsigned int,int>::iterator p;
    for(p = __queue_index_map.begin(); p!=__queue_index_map.end(); ++p)
    {
        LOGD("p %d", p->first);
        free(__vertex_buffer_map[p->first]);
        free(__texcoord_buffer_map[p->first]);
        free(__alpha_buffer_map[p->first]);
        glDeleteTextures(1, &p->first);
    }
    __queue_index_map.clear();
    __vertex_buffer_map.clear();
    __texcoord_buffer_map.clear();
    __alpha_buffer_map.clear();

    RELEASE(__ortho2dMatrixObjects);
    RELEASE(__ortho2dMatrixScale);
    RELEASE(__ortho2dMatrixScreen);

    //delete all textures
    glDeleteTextures(1, &__offscreenTextureId);
    glDeleteProgram(__progLoc);
    glDeleteBuffers(1, &__colorRenderBufferId);
    glDeleteBuffers(1, &__defaultFrameBufferId);
    glDeleteBuffers(1, &__offscreenFrameBufferId);
}

void GL::setupGL()
{
    LOGD("Engine :: OpenGL Config.\n");
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    
    // Create Screen Buffer
    LOGE("Engine :: Create Screen Buffer.\n");
    glGenRenderbuffers(1, &__colorRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, __colorRenderBufferId);
#ifdef IOS
    glGenFramebuffers(1, &__defaultFrameBufferId);
#elif ANDROID
    LOGE("Create FrameBuffer for Android");
    __defaultFrameBufferId = (GLuint) 0;
#endif
    glBindFramebuffer(GL_FRAMEBUFFER, __defaultFrameBufferId);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, __colorRenderBufferId);
    
    //Create Offscreen Buffer and Offscreen Texture
    LOGD("Engine :: Create Offscreen Screen Buffer.\n");
    glGenTextures(1, &__offscreenTextureId);
    glBindTexture(GL_TEXTURE_2D, __offscreenTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, OFF_SCREEN_WIDTH, OFF_SCREEN_HEIGHT, 0,  GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#ifdef IOS
    glGenFramebuffers(1, &__offscreenFrameBufferId);
#elif ANDROID
    __offscreenFrameBufferId = (GLuint) 1;
#endif
    glActiveTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_FRAMEBUFFER, __offscreenFrameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, __offscreenTextureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    LOGD("Engine :: Offscreen Texture Id %d\n", __offscreenTextureId);
    
    //Create Offscreen Buffer and Offscreen Texture
    LOGD("Engine :: Create Offscreen Screen Buffer 2.\n");
    glGenTextures(1, &__offscreenTextureId_1);
    glBindTexture(GL_TEXTURE_2D, __offscreenTextureId_1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0,  GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#ifdef IOS
    glGenFramebuffers(1, &__offscreenFrameBufferId_1);
#elif ANDROID
    __offscreenFrameBufferId_1 = (GLuint) 2;
#endif
    glActiveTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_FRAMEBUFFER, __offscreenFrameBufferId_1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, __offscreenTextureId_1, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    LOGD("Engine :: Offscreen Texture Id 1 %d\n", __offscreenTextureId_1);
    
    //Load shader program
    LOGD("Engine :: Load program 0.\n");
    const GLchar * _vertexShaderCode[] = {
        "uniform mat4 uMatrix; \n",
        "attribute vec4 aPosition; \n",
        "attribute vec2 aTexCoord; \n",
        "attribute float aAlpha;\n",
        "varying vec2 vTexCoord; \n",
        "varying float vAlpha; \n",
        "void main() { \n",
        "gl_Position = uMatrix * aPosition; \n",
        "gl_Position.x -= 1.0; \n",
        "gl_Position.y += 1.0; \n",
        "vTexCoord = aTexCoord; \n",
        "vAlpha = aAlpha; \n",
        "}",
    };
    
    const GLchar * _fragmentShaderCode[] = {
        "precision lowp float;\n",
        "uniform lowp sampler2D sTexture;\n",
        "varying mediump vec2 vTexCoord;\n",
        "varying lowp float vAlpha;\n",
        "void main() {\n",
        "gl_FragColor = vAlpha * texture2D(sTexture, vTexCoord);\n",
        "}"
    };
    
    int _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader, 13, (const GLchar **) (_vertexShaderCode), NULL);
    glCompileShader(_vertexShader);
    
    
    int _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader,7, (const GLchar **) (_fragmentShaderCode), NULL);
    glCompileShader(_fragmentShader);
    
    __progLoc = glCreateProgram(); // create empty OpenGL Program
    glAttachShader(__progLoc, _vertexShader); // add the vertex
    glAttachShader(__progLoc, _fragmentShader); // add the fragment
    glLinkProgram(__progLoc);
    
    __matLoc = glGetUniformLocation(__progLoc, "uMatrix");
    __sampLoc = glGetUniformLocation(__progLoc, "sTexture");
    __posLoc = glGetAttribLocation(__progLoc, "aPosition");
    __texLoc = glGetAttribLocation(__progLoc, "aTexCoord");
    __alpLoc = glGetAttribLocation(__progLoc, "aAlpha");
    
    glDetachShader(__progLoc, _vertexShader);
    glDeleteShader(_vertexShader);
    glDetachShader(__progLoc, _fragmentShader);
    glDeleteShader(_fragmentShader);
    
    LOGD("Engine :: Location of program 0: m_progLoc %d\n", __progLoc);
    LOGD("Engine :: Location of variables 0: m_matLoc %d m_sampLoc %d m_posLoc %d m_texLoc %d m_alpLoc %d\n", __matLoc, __sampLoc, __posLoc, __texLoc, __alpLoc);
    
    //Load shader program
    LOGD("Engine :: Load program 1.\n");
    const GLchar * _vertexShaderCode_1[] = {
        "uniform mat4 uMatrix; \n",
        "attribute vec4 aPosition; \n",
        "attribute vec2 aTexCoord; \n",
        "varying vec2 vTexCoord; \n",
        "void main() { \n",
        "gl_Position = uMatrix * aPosition; \n",
        "gl_Position.x -= 1.0; \n",
        "gl_Position.y += 1.0; \n",
        "vTexCoord = aTexCoord; \n",
        "}",
    };
    
    const GLchar * _fragmentShaderCode_1[] = {
        "precision lowp float;\n",
        "uniform lowp sampler2D sTexture;\n",
        "varying mediump vec2 vTexCoord;\n",
        "void main() {\n",
        "gl_FragColor = texture2D(sTexture, vTexCoord);\n",
        "}"
    };
    
    int _vertexShader_1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader_1, 10, (const GLchar **) (_vertexShaderCode_1), NULL);
    glCompileShader(_vertexShader_1);
    
    
    int _fragmentShader_1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader_1,6, (const GLchar **) (_fragmentShaderCode_1), NULL);
    glCompileShader(_fragmentShader_1);
    
    __progLoc_1 = glCreateProgram(); // create empty OpenGL Program
    glAttachShader(__progLoc_1, _vertexShader_1); // add the vertex
    glAttachShader(__progLoc_1, _fragmentShader_1); // add the fragment
    glLinkProgram(__progLoc_1);
    
    __matLoc_1 = glGetUniformLocation(__progLoc_1, "uMatrix");
    __sampLoc_1 = glGetUniformLocation(__progLoc_1, "sTexture");
    __posLoc_1 = glGetAttribLocation(__progLoc_1, "aPosition");
    __texLoc_1 = glGetAttribLocation(__progLoc_1, "aTexCoord");
    __alpLoc_1 = 0;
    
    glDetachShader(__progLoc_1, _vertexShader_1);
    glDeleteShader(_vertexShader_1);
    glDetachShader(__progLoc_1, _fragmentShader_1);
    glDeleteShader(_fragmentShader_1);
    
    LOGD("Engine :: Location of program 1: m_progLoc_1 %d\n", __progLoc_1);
    LOGD("Engine :: Location of variables 1: m_matLoc_1 %d m_sampLoc_1 %d m_posLoc_1 %d m_texLoc_1 %d m_alpLoc_1 %d\n", __matLoc_1, __sampLoc_1, __posLoc_1, __texLoc_1, __alpLoc_1);
    
    LOGD("Engine :: Other initialzing.\n");
    __queueIndex = 0;
    __colorf = 0.0f;

    __ortho2dMatrixObjects = new Mat4();
    Mat4::createOrthographic(SCREEN_WIDTH, -SCREEN_HEIGHT, 0, 1000, __ortho2dMatrixObjects);

    __ortho2dMatrixScreen = new Mat4();
    __ortho2dMatrixScale = new Mat4();
}

void GL::setTextureAtlas(unsigned int TextureId)
{
    __texture = TextureId;

    GLshort * vertex_buffer = (GLshort *) malloc(MAX_QUADS * sizeof(GLshort) * 8);
    GLfloat * texcoord_buffer = (GLfloat *) malloc(MAX_QUADS * sizeof(GLfloat) * 8);
    GLfloat * alpha_buffer = (GLfloat *) malloc(MAX_QUADS * sizeof(GLfloat) * 4);
    
    __vertex_buffer_map[TextureId] = vertex_buffer;
    __texcoord_buffer_map[TextureId] = texcoord_buffer;
    __alpha_buffer_map[TextureId] = alpha_buffer;
    __queue_index_map[TextureId] = 0;
}

void GL::resize(int width, int height)
{
    glBindRenderbuffer(GL_RENDERBUFFER, __colorRenderBufferId);
	__width = width;
    __height = height;
    
    __pictScaleRatio = GET_MIN(round((double)__width / (double)SCREEN_WIDTH), round((double)__height / (double)SCREEN_HEIGHT));
    __upScaleWidth = SCREEN_WIDTH * __pictScaleRatio;
    __upScaleHeight = SCREEN_HEIGHT * __pictScaleRatio;
    
    while (__upScaleWidth > 1024 || __upScaleHeight > 1024)
    {
        __pictScaleRatio--;
        __upScaleWidth = SCREEN_WIDTH * __pictScaleRatio;
        __upScaleHeight = SCREEN_HEIGHT * __pictScaleRatio;
    }
    
    __upscaleTextureU = __upScaleWidth / OFF_SCREEN_WIDTH;
    __upscaleTextureV = __upScaleHeight / OFF_SCREEN_HEIGHT;

    __ratio = ((float)__width / (float)SCREEN_WIDTH);
    
    __fitWidth = SCREEN_WIDTH * __ratio;
    __fitHeight = SCREEN_HEIGHT * __ratio;

    __offsetX = (__width - __fitWidth) >> 1;
    __offsetY = (__height - __fitHeight) >> 1;

    __offscreenTextureU = SCREEN_WIDTH / OFF_SCREEN_WIDTH;
    __offscreenTextureV = SCREEN_HEIGHT / OFF_SCREEN_HEIGHT;

    Mat4::createOrthographic(__upScaleWidth, -__upScaleHeight, 0, 1000, __ortho2dMatrixScale);
    Mat4::createOrthographic(__width, -__height, 0, 1000, __ortho2dMatrixScreen);
    
    LOGD("Engine :: Resize %d x %d\n", width, height);
    LOGD("Enviroment Value\n");
    LOGD("Size: m_width=%d m_height=%d\n",__width,__height);
    LOGD("Fit Size: m_fitWidth=%d m_fitHeight=%d\n",__fitWidth,__fitHeight);
    LOGD("Ratio: m_ratio_width=%f m_ratio_height=%f\n", __ratio, ((float)__height / (float)SCREEN_HEIGHT));
    LOGD("Offset: offset_x=%d offset_y=%d\n",__offsetX,__offsetY);
}

void GL::__renderToOffsceen()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindFramebuffer(GL_FRAMEBUFFER, __offscreenFrameBufferId);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SceneDirector::Instance()->render();
    /*
    for(std::map<unsigned int,int>::iterator p = __queue_index_map.begin(); p != __queue_index_map.end(); ++p)
    {
        __queueIndex = p->second;
        if (__queueIndex > 0)
        {
            __vertexBuffer = __vertex_buffer_map[p->first];
            __texcoordBuffer = __texcoord_buffer_map[p->first];
            __alphaBuffer = __alpha_buffer_map[p->first];

            glUseProgram(__progLoc);

            glEnableVertexAttribArray(__posLoc);
            glVertexAttribPointer(__posLoc, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), __vertexBuffer);

            glEnableVertexAttribArray(__texLoc);
            glVertexAttribPointer(__texLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), __texcoordBuffer);

            glEnableVertexAttribArray(__alpLoc);
            glVertexAttribPointer(__alpLoc, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), __alphaBuffer);

            Mat4::createOrthographic(SCREEN_WIDTH, -SCREEN_HEIGHT, 0, 1000, __ortho2dMatrixObjects);
            glUniformMatrix4fv(__matLoc, 1, GL_FALSE, __ortho2dMatrixObjects->m);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, p->first);
            glUniform1i(__sampLoc, 0);
            glDrawElements(GL_TRIANGLES, __queueIndex * 6, GL_UNSIGNED_SHORT, __orderBuffer);
            
            glDisableVertexAttribArray(__posLoc);
            glDisableVertexAttribArray(__texLoc);
            glDisableVertexAttribArray(__alpLoc);
        }
        __queue_index_map[p->first] = 0;
        __queueIndex = 0;
    }
    */
    
    glDisable(GL_BLEND);
}

void GL::__renderToUpscale()
{
    glBindFramebuffer(GL_FRAMEBUFFER, __offscreenFrameBufferId_1);
    glViewport(0, 0, __upScaleWidth, __upScaleHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (__queue_index_map[0] > 0)
    {
        glUseProgram(__progLoc_1);
        glEnableVertexAttribArray(__posLoc_1);
        glVertexAttribPointer(__posLoc_1, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), __vertexBuffer);
        
        glEnableVertexAttribArray(__texLoc_1);
        glVertexAttribPointer(__texLoc_1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), __texcoordBuffer);

        glUniformMatrix4fv(__matLoc_1, 1, GL_FALSE, __ortho2dMatrixScale->m);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, __offscreenTextureId);
        glUniform1i(__sampLoc_1, 0);
        glDrawElements(GL_TRIANGLES, __queueIndex * 6, GL_UNSIGNED_SHORT, __orderBuffer);
        
        glDisableVertexAttribArray(__posLoc_1);
        glDisableVertexAttribArray(__texLoc_1);
    }
    __queue_index_map[0] = 0;
}

void GL::__renderToSceen()
{
    glViewport(0, 0, __width, __height);
    glBindFramebuffer(GL_FRAMEBUFFER, __defaultFrameBufferId);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (__queue_index_map[0] > 0)
    {
        //LOGD("render to screen");
        glUseProgram(__progLoc_1);
        glEnableVertexAttribArray(__posLoc_1);
        glVertexAttribPointer(__posLoc_1, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), __vertexBuffer);
        
        glEnableVertexAttribArray(__texLoc_1);
        glVertexAttribPointer(__texLoc_1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), __texcoordBuffer);

        glUniformMatrix4fv(__matLoc_1, 1, GL_FALSE, __ortho2dMatrixScreen->m);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, __offscreenTextureId_1);
        glUniform1i(__sampLoc_1, 0);
        glDrawElements(GL_TRIANGLES, __queueIndex * 6, GL_UNSIGNED_SHORT, __orderBuffer);
        
        glDisableVertexAttribArray(__posLoc_1);
        glDisableVertexAttribArray(__texLoc_1);
    }
    __queue_index_map[0] = 0;
    __queueIndex = 0;
    
    glBindRenderbuffer(GL_RENDERBUFFER, __colorRenderBufferId);
}

void GL::render()
{
    __renderToOffsceen();
    
    cmdRender(__upScaleWidth, 0.0f, 0.0f, __upScaleHeight, __offscreenTextureU, __offscreenTextureV, 0.0f, 0.0f, 1.0f);
    __renderToUpscale();
    
//    cmdRender(__width - ((__width - __fitWidth) >> 1), ((__height - __fitHeight) >> 1), ((__width - __fitWidth) >> 1), __height - ((__height - __fitHeight) >> 1), __upscaleTextureU, __upscaleTextureV, 0.0f, 0.0f, 1.0f);
    cmdRender(__fitWidth, 0, 0, __fitHeight, __upscaleTextureU, __upscaleTextureV, 0.0f, 0.0f, 1.0f);
    __renderToSceen();
}

void GL::cmdRender(int dest_x0, int dest_y0, int dest_x1, int dest_y1, float tex_u0, float tex_v0, float tex_u1, float tex_v1, float alpha, unsigned int texture)
{
    __vertexBuffer = __vertex_buffer_map[texture];
    __texcoordBuffer = __texcoord_buffer_map[texture];
    __alphaBuffer = __alpha_buffer_map[texture];
    __queueIndex = __queue_index_map[texture];
    if(!__vertexBuffer)
    {
        LOGE("Texture not found %d\n", texture);
        return;
    }
    
    __vertexBuffer[(__queueIndex * 8) + 0] = dest_x0;
    __vertexBuffer[(__queueIndex * 8) + 1] = dest_y1;
    __vertexBuffer[(__queueIndex * 8) + 2] = dest_x1;
    __vertexBuffer[(__queueIndex * 8) + 3] = dest_y1;
    __vertexBuffer[(__queueIndex * 8) + 4] = dest_x1;
    __vertexBuffer[(__queueIndex * 8) + 5] = dest_y0;
    __vertexBuffer[(__queueIndex * 8) + 6] = dest_x0;
    __vertexBuffer[(__queueIndex * 8) + 7] = dest_y0;
    
    __texcoordBuffer[(__queueIndex * 8) + 0] = tex_u0;
    __texcoordBuffer[(__queueIndex * 8) + 1] = tex_v1;
    __texcoordBuffer[(__queueIndex * 8) + 2] = tex_u1;
    __texcoordBuffer[(__queueIndex * 8) + 3] = tex_v1;
    __texcoordBuffer[(__queueIndex * 8) + 4] = tex_u1;
    __texcoordBuffer[(__queueIndex * 8) + 5] = tex_v0;
    __texcoordBuffer[(__queueIndex * 8) + 6] = tex_u0;
    __texcoordBuffer[(__queueIndex * 8) + 7] = tex_v0;
    
    __alphaBuffer[(__queueIndex * 4) + 0] = alpha;
    __alphaBuffer[(__queueIndex * 4) + 1] = alpha;
    __alphaBuffer[(__queueIndex * 4) + 2] = alpha;
    __alphaBuffer[(__queueIndex * 4) + 3] = alpha;
    
    __queue_index_map[texture] ++;
    __queueIndex++;//may not need
}

void GL::cmdRender(Mat4 * Mvp, Texture *Image, int W, int H, float Alpha)
{
    cmdRender(0, 0, W, H, Image->u0, Image->v0, Image->u1, Image->v1, Alpha, Image->m_textureId);

    __queueIndex = __queue_index_map[Image->m_textureId];
    if (__queueIndex > 0)
    {
        __vertexBuffer = __vertex_buffer_map[Image->m_textureId];
        __texcoordBuffer = __texcoord_buffer_map[Image->m_textureId];
        __alphaBuffer = __alpha_buffer_map[Image->m_textureId];
            
        glUseProgram(__progLoc);
            
        glEnableVertexAttribArray(__posLoc);
        glVertexAttribPointer(__posLoc, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), __vertexBuffer);
            
        glEnableVertexAttribArray(__texLoc);
        glVertexAttribPointer(__texLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), __texcoordBuffer);
            
        glEnableVertexAttribArray(__alpLoc);
        glVertexAttribPointer(__alpLoc, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), __alphaBuffer);

        glUniformMatrix4fv(__matLoc, 1, GL_FALSE, Mvp->m);
            
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Image->m_textureId);
        glUniform1i(__sampLoc, 0);
        glDrawElements(GL_TRIANGLES, __queueIndex * 6, GL_UNSIGNED_SHORT, __orderBuffer);
            
        glDisableVertexAttribArray(__posLoc);
        glDisableVertexAttribArray(__texLoc);
        glDisableVertexAttribArray(__alpLoc);
    }
    __queue_index_map[Image->m_textureId] = 0;
    __queueIndex = 0;
}

Mat4 * GL::mvp()
{
    return __ortho2dMatrixObjects;
}