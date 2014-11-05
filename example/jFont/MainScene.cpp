//
//  MainScene.cpp
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "MainScene.h"
#include "Message.h"
#include "MathUtil.h"
#include "Engine.h"

NAMESPACE_USING;
using namespace jFont;

MainScene::MainScene(bool Tutorial) : Scene(), ObserverInterface()
{
    active = true;

    Sprite * bg = new Sprite();
    bg->autorelease();
    bg->load("gray", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    bg->setPosition(0, 0);
    addChild(bg);

    __font = new FontTTF();
    __font->autorelease();
    __font->setPosition(SCREEN_WIDTH >> 1, 50);
    addChild(__font);

    __reset();
}

MainScene::~MainScene()
{
}

void MainScene::__reset()
{
    __font->setText("0");

    Engine::Instance()->highScore = 0;
}

void MainScene::_update(float Elapsed)
{
}

void MainScene::touchPressed(int X, int Y)
{
}

void MainScene::touchReleased(int PrevX, int PrevY, int X, int Y)
{
}

void MainScene::touchMoved(int SrcX, int SrcY, int X, int Y)
{
}

void MainScene::onNotify(Message * Message)
{
    LOGD("MainScene::onNotify cmd=%d", Message->cmd);
    switch (Message->cmd)
    {
        default:
            break;
    }
}
