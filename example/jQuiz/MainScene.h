//
//  MainScene.h
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jExample__MainScene__
#define __jExample__MainScene__

#include "Scene.h"
#include "ObserverInterface.h"
#include "Font.h"
#include "Sprite.h"

namespace jQuiz {

NAMESPACE_USING

class Cell;

#define MAX_W 3
#define MAX_H 3

class MainScene : public Scene, public ObserverInterface
{
private:
    Cell *** __grid;
    Cell * __current;

    Font * __font;

    int __score;
    bool __isOver;
    
    float __fadeOut;
    Cell * __wrongCell;

    void __reset();

    void _update(float Elapsed);

public:
    MainScene(bool Tutorial);
    virtual ~MainScene();

    void touchPressed(int X, int Y);
    void touchReleased(int PrevX, int PrevY, int X, int Y);
    void touchMoved(int SrcX, int SrcY, int X, int Y);

    void onNotify(Message * Message);
};

}

#endif /* defined(__jExample__MainScene__) */
