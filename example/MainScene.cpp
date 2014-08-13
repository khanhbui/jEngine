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
#include "Math.h"

#include <vector>

NAMESPACE_USING;

MainScene::MainScene() : Scene(), ObserverInterface()
{
    Sprite * bg = new Sprite();
    bg->autorelease();
    bg->load("gray", SCREEN_WIDTH << 1, SCREEN_HEIGHT << 1, 0, 0);
    bg->setPosition(-SCREEN_WIDTH >> 1, -SCREEN_HEIGHT >> 1);
    addChild(bg);

    Sprite * container = new Sprite();
    container->autorelease();
    container->load("white", MAX_W * 32, MAX_H * 32, 0, 0);
    container->setPosition((SCREEN_WIDTH - container->w) >> 1, (SCREEN_HEIGHT - container->h) >> 1);
    addChild(container);

    __grid = new Cell**[MAX_W];
    for (int i = 0; i < MAX_W; i++)
    {
        __grid[i] = new Cell*[MAX_H];
        for (int j = 0; j < MAX_H; j++)
        {
            Cell * cell = new Cell(i, j, 0);
            cell->autorelease();

            __grid[i][j] = cell;

            container->addChild(cell);
        }
    }

    Node * colorContainer = new Node();
    colorContainer->autorelease();
    colorContainer->setPosition((SCREEN_WIDTH - 3 * 32 - 2) >> 1, ((SCREEN_HEIGHT - MAX_H * 32) >> 1) - 35);
    addChild(colorContainer);

    __color0 = new Cell(0, 0, 0);
    __color0->autorelease();
    __color0->setState(0);
    colorContainer->addChild(__color0);

    __color1 = new Cell(1, 0, 0);
    __color1->autorelease();
    __color1->setState(2);
    __color1->setPosition(__color1->getX() + 1, __color1->getY());
    colorContainer->addChild(__color1);

    __color2 = new Cell(2, 0, 0);
    __color2->autorelease();
    __color2->setState(2);
    __color2->setPosition(__color2->getX() + 2, __color2->getY());
    colorContainer->addChild(__color2);

    Sprite * arrow = new Sprite();
    arrow->autorelease();
    arrow->load("arrow", 0, 0, 0, 0);
    arrow->setPosition((__color0->w - arrow->w) >> 1, -arrow->h);
    colorContainer->addChild(arrow);

    __reset();
}

MainScene::~MainScene()
{
}

void MainScene::update(float Elapsed)
{
}

void MainScene::__reset()
{
    for (int i = 0; i < MAX_W; i++)
    {
        for (int j = 0; j < MAX_H; j++)
        {
            __grid[i][j]->visible = false;
        }
    }

    __current = __grid[MAX_W >> 1][MAX_H >> 1];
    __current->visible = true;
    __current->setState(0);
    __current->setColor(Math::Instance()->randomIn(0, 4));

    __color0->setColor(Math::Instance()->randomIn(0, 4));
    __color1->setColor(Math::Instance()->randomIn(0, 4));
    __color2->setColor(Math::Instance()->randomIn(0, 4));
}

void MainScene::__move(int dir)
{
    Cell * next = NULL;
    switch (dir)
    {
        case 0:
            if (__current->gridX + 1 < MAX_W)
            {
                next = __grid[__current->gridX + 1][__current->gridY];
            }
        break;

        case 1:
            if (__current->gridX - 1 >= 0)
            {
                next = __grid[__current->gridX - 1][__current->gridY];
            }
        break;

        case 2:
            if (__current->gridY + 1 < MAX_H)
            {
                next = __grid[__current->gridX][__current->gridY + 1];
            }
        break;

        case 3:
            if (__current->gridY - 1 >= 0)
            {
                next = __grid[__current->gridX][__current->gridY - 1];
            }
        break;

        default:
        break;
    }

    if (next && !next->visible)
    {
        __current->setState(2);

        __current = next;
        __current->setState(0);
        __current->setColor(__color0->color);
        __current->visible = true;

        __checkScore();

        __color0->setColor(__color1->color);
        __color1->setColor(__color2->color);
        __color2->setColor(Math::Instance()->randomIn(0, 4));
    }
}

void MainScene::__checkScore()
{
    std::vector<Cell *> queue;
    queue.push_back(__current);

    int index = 0;
    bool hasMore = true;
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    while (hasMore)
    {
        Cell * curr = *(queue.begin() + index);
        index++;

        hasMore = false;
        for (int i = 0; i < 4; i++)
        {
            if (0 <= curr->gridX + dx[i] && curr->gridX + dx[i] < MAX_W &&
                0 <= curr->gridY + dy[i] && curr->gridY + dy[i] < MAX_H)
            {
                Cell * next = __grid[curr->gridX + dx[i]][curr->gridY + dy[i]];
                if (next && next->visible && next->color == curr->color)
                {
                    bool visited = false;
                    for (std::vector<Cell *>::iterator it = queue.begin(); it != queue.end(); it++)
                    {
                        if (next == *it)
                        {
                            visited = true;
                            break;
                        }
                    }
                    if (!visited)
                    {
                        queue.push_back(next);
                        hasMore = true;
                    }
                }
            }
        }
    }

    if (queue.size() >= 3)
    {
        for (std::vector<Cell *>::iterator it = queue.begin(); it != queue.end(); it++)
        {
            (*it)->visible = false;
        }
        __current->visible = true;
    }
    else
    {
        __checkOver();
    }
    queue.clear();
}

void MainScene::__checkOver()
{
    __isOver = true;

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; i++)
    {
        if (0 <= __current->gridX + dx[i] && __current->gridX + dx[i] < MAX_W &&
            0 <= __current->gridY + dy[i] && __current->gridY + dy[i] < MAX_H)
        {
            Cell * next = __grid[__current->gridX + dx[i]][__current->gridY + dy[i]];
            if (next && !next->visible)
            {
                __isOver = false;
                break;
            }
        }
    }

    if (__isOver)
    {
        __current->setState(1);
    }
}

void MainScene::touchPressed(int X, int Y)
{
    if (__isOver)
    {
        __reset();
    }

    __pressedX = X;
    __pressedY = Y;
}

void MainScene::touchReleased(int PrevX, int PrevY, int X, int Y)
{
    int dx = X - __pressedX;
    int dy = Y - __pressedY;

    if (abs(dx) < 5 && abs(dy) < 5)
    {
        return;
    }

    if (abs(dx) > abs(dy))
    {
        if (dx > 0)
        {
            __move(0);
        }
        else
        {
            __move(1);
        }
    }
    else
    {
        if (dy > 0)
        {
            __move(2);
        }
        else
        {
            __move(3);
        }
    }
}

void MainScene::touchMoved(int SrcX, int SrcY, int X, int Y)
{
}

void MainScene::onNotify(Message * aMessage)
{
    LOGD("MainScene::onNotify cmd=%s", aMessage->cmd);
}