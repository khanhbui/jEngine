//
//  Engine.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Engine.h"
#include "GL.h"
#include "Texture.h"
#include "Timer.h"
#include "SceneDirector.h"
#include "Notifier.h"
#include "Math.h"

#include <math.h>
#include <sys/time.h>

NAMESPACE_USING

Engine * Engine::_instance = NULL;

Engine::Engine() : Ref("Engine"),
__lastTick(0.0f)
{
    _images = (Texture **) malloc(MAX_IMAGES * sizeof(Texture *));
    memset(_images, 0, MAX_IMAGES * sizeof(Texture *));

    __GL = new GL();
    __GL->setTextureAtlas(0);
}


Engine::~Engine()
{
    RELEASE(__GL);

    for (int i = 0; i < MAX_IMAGES; i++)
    {
        RELEASE(_images[i]);
    }

    PoolManager::Delete();
    Notifier::Delete();
}

void Engine::setup()
{
    __GL->setupGL();

    numOfTouches = 0;
}

void Engine::resize(int Width, int Height)
{
    __GL->resize(Width, Height);
}

void Engine::enterFrame()
{
    double tick = FRAME_SECONDS;
    double currtime = Timer::getCurrentMillSecond();
    if (__lastTick != 0)
    {
        tick = (currtime - __lastTick) / 1000.0;
    }
    __lastTick = currtime;
    
    if (tick > FRAME_SECONDS)
    {
        tick = FRAME_SECONDS;//adjust the tick time
    }

    
    Notifier::Instance()->update(tick);

    SceneDirector::Instance()->update(tick);
    render();

    PoolManager::Instance()->getCurrentPool()->clear();
}

void Engine::render()
{
    __GL->render();
}

void Engine::touchPressed(int X, int Y)
{
    SceneDirector::Instance()->touchPressed(X, Y);
}

void Engine::touchReleased(int PrevX, int PrevY, int X, int Y)
{
    SceneDirector::Instance()->touchReleased(PrevX, PrevY, X, Y);
}

void Engine::touchMoved(int SrcX, int SrcY, int X, int Y)
{
    SceneDirector::Instance()->touchMoved(SrcX, SrcY, X, Y);
}

void Engine::onInit()
{
}

void Engine::onPause()
{
}

void Engine::onResume()
{
}

void Engine::onDestroy()
{
}

Texture ** Engine::findImages(const char * FileName, int &Count)
{
    int startIndex = -1;
    Count = 0;
    for (int i = 0; i < MAX_IMAGES; i++)
    {
        if (_images[i])
        {
            LOGD("=== %s\n", _images[i]->fileName);
            int j = 0;

            for (; j < strlen(FileName); j++)
            {
                if (FileName[j] != _images[i]->fileName[j])
                {
                    break;
                }
            }
            
            if (j == strlen(FileName))
            {
                Count++;
                if (startIndex < 0)
                {
                    startIndex = i;
                }
            }
        }
    }
    
    if (startIndex < 0)
    {
        return NULL;
    }
    else
    {
        return _images + startIndex;
    }
}

Texture * Engine::findImage(const char * FileName)
{
    for (int i = 0; i < MAX_IMAGES; i++)
    {
        if (_images[i])
        {
            int c = strcmp(FileName, _images[i]->fileName);
            if (c == 0)
            {
                return _images[i];
            }
        }
    }
    LOGE("Image not found: %s \n", FileName);
    return NULL;
}

void Engine::setTouches(const int * TouchesX, const int * TouchesY, int NumOfTouches)
{
    numOfTouches = GET_MIN(NumOfTouches, MAX_TOUCHES);
    for (int i = 0; i < numOfTouches; i++)
    {
        touchesX[i] = TouchesX[i];
        touchesY[i] = TouchesY[i];
    }
}

GL * Engine::renderer()
{
    return __GL;
}

void Engine::setDefaultScene(Scene * scene)
{
    SceneDirector::Instance()->push(scene);
}

Engine * Engine::Instance()
{
    return Engine::_instance;
}

void Engine::Create()
{
    if (!Engine::_instance)
    {
        Math::Create();
        Math::Instance()->randomize((int)Timer::getCurrentMillSecond());

        PoolManager::Create();
        Notifier::Create();

        Engine::_instance = new Engine();
    
        SceneDirector::Create();
    }
}

void Engine::Delete()
{
    RELEASE(Engine::_instance);
}

void Engine::setTextureAtlas(unsigned int TextureId, const char * Description, int NumOfChars)
{
    addTextureAtlas(TextureId, Description, NumOfChars);
}

void Engine::addTextureAtlas(unsigned int TextureId, const char * Description, int NumOfChars)
{
    __GL->setTextureAtlas(TextureId);
    
    if (!Description)
    {
        int i=0;
        while (_images[i] && i < MAX_IMAGES)
        {
            if (_images[i]->m_textureId <= 0)
            {
                _images[i]->m_textureId = TextureId;
            }
            i ++;
        }
        return;
    }
    int char_i = 0;
    char curLine[256];
    char * _line = curLine;
    
    char _file_name[64];
    float _tex_u0 = 0.0, _tex_v0 = 0.0, _tex_u1 = 0, _tex_v1 = 0;
    int _width = 0, _height = 0;
    
    while (char_i < NumOfChars)
    {
        if (Description[char_i] != '\r' && Description[char_i] != '\n' && Description[char_i] != 0)
        {
            _line[0] = Description[char_i];
            _line++;
        }
        else
        {
            _line[0] = 0;
            _line = curLine;
            
            if (strlen(_line) > 1)
            {
                sscanf(_line, "%s %d %d %f %f %f %f", _file_name, &_width, &_height, &_tex_u0, &_tex_v0, &_tex_u1, &_tex_v1);
                Texture * image = new Texture(_file_name, _width, _height, _tex_u0, _tex_v0, _tex_u0 + _tex_u1, _tex_v0 + _tex_v1, TextureId);
                _images[image->id] = image;
            }
            
            _line[0] = 0;
        }
        char_i++;
    }
};
