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
color(-1)
{
    setColor(Color);

    setPosition(gridX * 32, gridY * 32);

    __begin = new Sprite();
    __begin->autorelease();
    __begin->load("begin", 32, 32, 0, 0);
    addChild(__begin);

    __end = new Sprite();
    __end->autorelease();
    __end->load("end", 32, 32, 0, 0);
    addChild(__end);
}

Cell::~Cell()
{
}

void Cell::setState(int State)
{
    state = State;

    switch (state)
    {
        case 0:
        {
            __begin->visible = true;
            __end->visible = false;
            break;
        }

        case 1:
        {
            __begin->visible = false;
            __end->visible = true;
            break;
        }

        default:
        {
            __begin->visible = false;
            __end->visible = false;
            break;
        }
    }
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
            load("yellow", 32, 32, 0, 0);
        break;

        case 1:
            load("red", 32, 32, 0, 0);
        break;

        case 2:
            load("blue", 32, 32, 0, 0);
        break;

        case 3:
            load("dark", 32, 32, 0, 0);
        break;

        default:
        break;
    }
}