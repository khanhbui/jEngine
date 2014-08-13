//
//  Engine.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 7/30/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Engine__
#define __jEngine__Engine__

#include "Ref.h"
#include "Macro.h"
#include "GL.h"

NAMESPACE_BEGIN

class Texture;
class Scene;

class Engine : public Ref
{
private:
    GL * __GL;

    double __lastTick;

protected:
    Texture ** _images;

    static Engine * _instance;
    Engine();
    
public:
    int touchesX[MAX_TOUCHES];
    int touchesY[MAX_TOUCHES];
    int numOfTouches;

    virtual ~Engine();

    void setup();
    void resize(int Width, int Height);

    void enterFrame();
    void render();

    void setTouches(const int * TouchesX, const int * TouchesY, int NumOfTouches);

    void touchPressed(int X, int Y);
    void touchReleased(int PrevX, int PrevY, int X, int Y);
    void touchMoved(int SrcX, int SrcY, int X, int Y);

    void onInit();
    void onPause();
    void onResume();
    void onDestroy();

    Texture ** findImages(const char * FileName, int &Count);
    Texture ** findImages(const char * FileName);
    Texture * findImage(const char * FileName);

    void resetTexture(unsigned int TextureId, unsigned int PrevTextureId);
    void setTextureAtlas(unsigned int TextureId, const char * Description = NULL, int NumOfChars = 0);
    void addTextureAtlas(unsigned int TextureId, const char * Description = NULL, int NumOfChars = 0);

    GL * renderer();

    void setDefaultScene(Scene * scene);

    static Engine * Instance();
    static void Create();
    static void Delete();
};

NAMESPACE_END

#endif /* defined(__jEngine__Engine__) */
