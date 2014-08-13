//
//  SceneDirector.h
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__SceneDirector__
#define __jEngine__SceneDirector__

#include "Ref.h"

NAMESPACE_BEGIN

class Scene;

class SceneDirector : public Ref
{
private:

protected:
    SceneDirector();

    static SceneDirector * _instance;

public:
    Scene * currentScene;
    
    virtual ~SceneDirector();

    virtual void update(float Elapsed);
    virtual void render();

    virtual void push(Scene * scene);

    virtual void touchPressed(int X, int Y);
    virtual void touchReleased(int PrevX, int PrevY, int X, int Y);
    virtual void touchMoved(int SrcX, int SrcY, int X, int Y);

    static SceneDirector * Instance();
    static void Create();
    static void Delete();
};

NAMESPACE_END

#endif /* defined(__jEngine__SceneDirector__) */
