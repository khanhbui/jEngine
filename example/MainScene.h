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
#include "Sprite.h"

NAMESPACE_USING

class Cell;

#define MAX_W 9
#define MAX_H 13

class MainScene : public Scene, public ObserverInterface
{
private:
    Cell *** __grid;
    Cell * __current;

    int __pressedX;
    int __pressedY;

    Cell * __color0;
    Cell * __color1;
    Cell * __color2;

    bool __isOver;

    void __reset();
    void __move(int dir);
    void __checkScore();
    void __checkOver();

public:
    MainScene();
    virtual ~MainScene();

    void update(float Elapsed);

    void touchPressed(int X, int Y);
    void touchReleased(int PrevX, int PrevY, int X, int Y);
    void touchMoved(int SrcX, int SrcY, int X, int Y);

    void onNotify(Message * Message);
};

#endif /* defined(__jExample__MainScene__) */
