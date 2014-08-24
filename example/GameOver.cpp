//
//  GameOver.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/24/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "GameOver.h"
#include "Sprite.h"

GameOver::GameOver()
{
    __gameOverText = new Font();
    __gameOverText->autorelease();
    __gameOverText->load("font", 0, Font::ALIGN_MIDDLE | Font::ALIGN_CENTER);
    __gameOverText->setText("GAME OVER");

    Sprite * bg = new Sprite();
    bg->autorelease();
    bg->load("gray", __gameOverText->w + 4, __gameOverText->h + 4, 0, 0);

    __gameOverText->setPosition(bg->w >> 1, bg->h >> 1);
    bg->addChild(__gameOverText);
    addChild(bg);

    w = bg->w;
    h = bg->h;
}

GameOver::~GameOver()
{
}