//
//  Cell.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/13/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Cell.h"

Cell::Cell(int GridX, int GridY, int Color) : Sprite(),
gridX(GridX),
gridY(GridY),
color(-1),
__isBomb(0)
{
    setColor(Color);

    setPosition(gridX * SIZE, gridY * SIZE);

    __bomb = new Sprite();
    __bomb->load("bomb", SIZE, SIZE, 0, 0);
}

Cell::~Cell()
{
    RELEASE(__bomb);
}

void Cell::setColor(int Color)
{
    if (color == Color)
    {
        return;
    }
    color = Color;

    switch (color)
    {
        case 0:
            load("yellow", SIZE, SIZE, 0, 0);
        break;

        case 1:
            load("red", SIZE, SIZE, 0, 0);
        break;

        case 2:
            load("blue", SIZE, SIZE, 0, 0);
        break;

        case 3:
            load("dark", SIZE, SIZE, 0, 0);
        break;

        default:
        break;
    }
}

void Cell::setIsBomb(int IsBomb)
{
    __isBomb = IsBomb;

    if (__isBomb > 0 && !__bomb->getParent())
    {
        addChild(__bomb);
    }
    else if (__isBomb <= 0 && __bomb->getParent())
    {
        removeChild(__bomb);
    }

    __bomb->alpha = (float)__isBomb / (float)MAX_BOMBS * 2.0f;
}

int Cell::getIsBomb() const
{
    return __isBomb;
}

void Cell::countDownBomb()
{
    setIsBomb(__isBomb - 1);
}