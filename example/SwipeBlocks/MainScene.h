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

NAMESPACE_USING

class Cell;
class GameOver;

#define MAX_W 7
#define MAX_H 9

class MainScene : public Scene, public ObserverInterface
{
private:
    Cell *** __grid;
    Cell * __current;
    
    Sprite * __begin;
    Sprite * __end;

    int __pressedX;
    int __pressedY;

    Cell * __color0;
    Cell * __color1;
    Cell * __color2;

    Font * __font;
    GameOver * __gameOver;

    Sprite * __tutorial;
    int __tutorialFlag;
    float __shakingTutorial;

    int __score;
    bool __isOver;

    void __reset();
    void __move(int dir);
    void __checkScore();
    void __checkOver();
    
    void _update(float Elapsed);

public:
    MainScene(bool Tutorial);
    virtual ~MainScene();

    void touchPressed(int X, int Y);
    void touchReleased(int PrevX, int PrevY, int X, int Y);
    void touchMoved(int SrcX, int SrcY, int X, int Y);

    void onNotify(Message * Message);
};

#endif /* defined(__jExample__MainScene__) */
