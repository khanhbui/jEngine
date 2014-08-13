//
//  SceneDirector.cpp
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "SceneDirector.h"
#include "Scene.h"

NAMESPACE_USING

SceneDirector * SceneDirector::_instance = NULL;

SceneDirector::SceneDirector() : Ref("SceneDirector"),
currentScene(NULL)
{
}

SceneDirector::~SceneDirector()
{
    RELEASE(currentScene);
}

void SceneDirector::update(float Elapsed)
{
    if (currentScene)
    {
        currentScene->update(Elapsed);
    }
}

void SceneDirector::render()
{
    if (currentScene)
    {
        currentScene->render();
    }
}

void SceneDirector::push(Scene * scene)
{
    currentScene = scene;
    scene->retain();
}

void SceneDirector::touchPressed(int X, int Y)
{
    if (currentScene)
    {
        currentScene->touchPressed(X, Y);
    }
}

void SceneDirector::touchReleased(int PrevX, int PrevY, int X, int Y)
{
    if (currentScene)
    {
        currentScene->touchReleased(PrevX, PrevY, X, Y);
    }
}

void SceneDirector::touchMoved(int SrcX, int SrcY, int X, int Y)
{
    if (currentScene)
    {
        currentScene->touchMoved(SrcX, SrcY, X, Y);
    }
}

SceneDirector * SceneDirector::Instance()
{
    return SceneDirector::_instance;
}

void SceneDirector::Create()
{
    if (!SceneDirector::_instance)
    {
        SceneDirector::_instance = new SceneDirector();
    }
}

void SceneDirector::Delete()
{
    RELEASE(SceneDirector::_instance);
}
