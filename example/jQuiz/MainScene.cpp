//
//  MainScene.cpp
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "MainScene.h"
#include "Message.h"
#include "Cell.h"
#include "Font.h"
#include "MathUtil.h"
#include "Engine.h"

#include <vector>
#include <cmath>

NAMESPACE_USING;
using namespace jQuiz;

MainScene::MainScene(bool Tutorial) : Scene(), ObserverInterface()
{
    active = true;

    Sprite * bg = new Sprite();
    bg->autorelease();
    bg->load("gray", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    bg->setPosition(0, 0);
    addChild(bg);

    __grid = new Cell**[MAX_W];
    for (int i = 0; i < MAX_W; i++)
    {
        __grid[i] = new Cell*[MAX_H];
        for (int j = 0; j < MAX_H; j++)
        {
            Cell * cell = new Cell(i, j);
            cell->autorelease();

            __grid[i][j] = cell;

            addChild(cell);
        }
    }
    
    __current = new Cell(1, -1);
    addChild(__current);

    __font = new Font();
    __font->autorelease();
    __font->load("font", 1, Font::ALIGN_CENTER | Font::ALIGN_TOP);
    __font->setPosition(SCREEN_WIDTH >> 1, 50);
//    addChild(__font);

//    Engine::Instance()->addOutputEvent(1);

    __reset();
}

MainScene::~MainScene()
{
}

void MainScene::__reset()
{
    for (int i = 0; i < MAX_W; i++)
    {
        for (int j = 0; j < MAX_H; j++)
        {
            __grid[i][j]->setChar(MathUtil::randomIn(0, Cell::MAX), 0);
        }
    }

    __current->setChar(__grid[MathUtil::randomIn(0, MAX_W)][MathUtil::randomIn(0, MAX_H)]->index, 1);

    __isOver = false;
    
    __score = 0;
    __font->setText(__score);
    
    __fadeOut = 0;

//    Engine::Instance()->addOutputEvent(2);
    Engine::Instance()->highScore = 0;
}

void MainScene::_update(float Elapsed)
{
    if (__fadeOut > 0) {
        __fadeOut -= Elapsed;
        __wrongCell->alpha = __fadeOut / 0.5f;
        if (__fadeOut <= 0) {
            __wrongCell->alpha = 1.0f;
            __wrongCell->setChar(__wrongCell->index, 0);
        }
    }
}

void MainScene::touchPressed(int X, int Y)
{
    if (__isOver || __fadeOut > 0)
    {
        return;
    }
    
    for (int i = 0; i < MAX_W; i++)
    {
        for (int j = 0; j < MAX_H; j++)
        {
            Cell * cell = __grid[i][j];
            if (cell->getX() <= X && X <= cell->getX() + cell->w && cell->getY() <= Y && Y <= cell->getY() + cell->h) {
                if (cell->index == __current->index) {
                    cell->setChar(MathUtil::randomIn(0, Cell::MAX), 0);
                    __current->setChar(__grid[MathUtil::randomIn(0, MAX_W)][MathUtil::randomIn(0, MAX_H)]->index, 1);
                }
                else {
                    cell->setChar(cell->index, 1);
                    __fadeOut = 0.5f;
                    __wrongCell = cell;
                }
            }
        }
    }
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
