//
//  GameOver.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/24/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "GameOver.h"
#include "Sprite.h"
#include "Font.h"

GameOver::GameOver() : Node(), NotifierInterface()
{
    Sprite * bg = new Sprite();
    bg->autorelease();
    bg->load("gray", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    bg->alpha = 0.6f;
    addChild(bg);
    
    w = bg->w;
    h = bg->h;

    Font * gameOverText = new Font();
    gameOverText->autorelease();
    gameOverText->load("font", 0, Font::ALIGN_MIDDLE | Font::ALIGN_CENTER);
    gameOverText->setText("GAME OVER");
    gameOverText->setPosition(w >> 1, 200);
    addChild(gameOverText);

    __playBtn = new Button();
    __playBtn->autorelease();
    __playBtn->load("button_play");
    __playBtn->setPosition((w - (__playBtn->w * 2 + 10)) >> 1, gameOverText->getY() + __playBtn->h + 10);
    __playBtn->active = true;
    addChild(__playBtn);

    __leaderboardBtn = new Button();
    __leaderboardBtn->autorelease();
    __leaderboardBtn->load("button_leaderboard");
    __leaderboardBtn->setPosition(__playBtn->getX() + __playBtn->w + 10, __playBtn->getY());
    __leaderboardBtn->active = true;
    addChild(__leaderboardBtn);
}

GameOver::~GameOver()
{
}

void GameOver::_update(float Elapsed)
{
    __playBtn->update(Elapsed);
    if (__playBtn->justReleased)
    {
        sendNotification(0, NULL, this);
    }

    __leaderboardBtn->update(Elapsed);
    if (__leaderboardBtn->justReleased)
    {
        sendNotification(1, NULL, this);
    }
}