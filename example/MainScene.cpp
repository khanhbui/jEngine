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
#include "GameOver.h"
#include "Font.h"
#include "MathUtil.h"

#include <vector>
#include <cmath>

NAMESPACE_USING;

MainScene::MainScene() : Scene(), ObserverInterface(),
__shakingTutorial(0.0f)
{
    active = true;

    Sprite * bg = new Sprite();
    bg->autorelease();
    bg->load("gray", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    bg->setPosition(0, 0);
    addChild(bg);

    Sprite * container = new Sprite();
    container->autorelease();
    container->load("white", MAX_W * Cell::SIZE, MAX_H * Cell::SIZE, 0, 0);
    container->setPosition((SCREEN_WIDTH - container->w) >> 1, (SCREEN_HEIGHT - container->h + Cell::SIZE) >> 1);
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
    colorContainer->setPosition(container->getX(), container->getY() - Cell::SIZE - 5);
    addChild(colorContainer);

    __color0 = new Cell(0, 0, 0);
    __color0->autorelease();
    colorContainer->addChild(__color0);

    __color1 = new Cell(1, 0, 0);
    __color1->autorelease();
    __color1->setPosition(__color1->getX() + 1, __color1->getY());
    colorContainer->addChild(__color1);

    __color2 = new Cell(2, 0, 0);
    __color2->autorelease();
    __color2->setPosition(__color2->getX() + 2, __color2->getY());
    colorContainer->addChild(__color2);

    Sprite * arrow = new Sprite();
    arrow->autorelease();
    arrow->load("arrow", 0, 0, 0, 0);
    arrow->setPosition((__color0->w - arrow->w) >> 1, -arrow->h);
    __color0->addChild(arrow);

    __font = new Font();
    __font->autorelease();
    __font->load("font", 1, Font::ALIGN_TOP | Font::ALIGN_RIGHT);
    __font->setPosition(container->getX() + container->w, colorContainer->getY());
    addChild(__font);

    __gameOver = new GameOver();
    __gameOver->autorelease();
    beginObserve(__gameOver);
    addChild(__gameOver);

    __begin = new Sprite();
    __begin->autorelease();
    __begin->load("begin", Cell::SIZE, Cell::SIZE, 0, 0);
    container->addChild(__begin);
    
    __end = new Sprite();
    __end->autorelease();
    __end->load("end", Cell::SIZE, Cell::SIZE, 0, 0);
    container->addChild(__end);

    __tutorial = new Sprite();
    __tutorial->autorelease();
    __tutorial->load("swipe_up", 0, 0, 0, 0);
    __tutorial->setPosition((SCREEN_WIDTH - __tutorial->w) >> 1, 350);
    addChild(__tutorial);

    __tutorialFlag = 0x0;
    
    __reset();
}

MainScene::~MainScene()
{
    endObserve(__gameOver);
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
    __current->setColor(MathUtil::Instance()->randomIn(0, Cell::MAX_COLORS));

    __begin->setPosition(__current->getX(), __current->getY());
    __end->visible = false;

    if (__tutorialFlag)
    {
        __color0->setColor(MathUtil::Instance()->randomIn(0, Cell::MAX_COLORS));
        __color1->setColor(MathUtil::Instance()->randomIn(0, Cell::MAX_COLORS));
        __color2->setColor(MathUtil::Instance()->randomIn(0, Cell::MAX_COLORS));
    }
    else
    {
        __color0->setColor((__current->color + 1) % Cell::MAX_COLORS);
        __color1->setColor(__current->color);
        __color2->setColor(__current->color);
    }

    __isOver = false;
    
    __score = 0;
    __font->setText(__score);

    __gameOver->visible = false;
    __gameOver->active = false;
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
        __current = next;
        __current->setColor(__color0->color);
        __current->setIsBomb(__color0->getIsBomb());
        __current->visible = true;

        __begin->setPosition(__current->getX(), __current->getY());

        __checkScore();

        for (int i = 0; i < MAX_W; i++)
        {
            for (int j = 0; j < MAX_H; j++)
            {
                Cell * cell = __grid[i][j];
                if (cell->visible && cell->getIsBomb() > 0)
                {
                    cell->countDownBomb();
                }
            }
        }

        __color0->setColor(__color1->color);
        __color0->setIsBomb(__color1->getIsBomb());
        
        __color1->setColor(__color2->color);
        __color1->setIsBomb(__color2->getIsBomb());

        __color2->setColor(MathUtil::Instance()->randomIn(0, Cell::MAX_COLORS));
        __color2->setIsBomb(MathUtil::Instance()->randf() <= 0.05f ? Cell::MAX_BOMBS : 0);

        if (!(__tutorialFlag & 1))
        {
            __tutorialFlag |= 1;
            __tutorial->load("swipe_right", 0, 0, 0, 0);

            __color2->setColor(__color1->color);
            __color2->setIsBomb(__color1->getIsBomb());
        }
        else if (!(__tutorialFlag & (1 << 1)))
        {
            __tutorialFlag |= (1 << 1);
            __tutorial->load("swipe_down", 0, 0, 0, 0);
        }
        else if (!(__tutorialFlag & (1 << 2)))
        {
            __tutorialFlag |= (1 << 2);
            __tutorial->load("swipe_left", 0, 0, 0, 0);
        }
        else if (!(__tutorialFlag & (1 << 3)))
        {
            __tutorialFlag |= (1 << 3);
            removeChild(__tutorial);
        }
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
        bool hasBomb = false;
        for (std::vector<Cell *>::iterator it = queue.begin(); it != queue.end(); it++)
        {
            if ((*it)->getIsBomb())
            {
                hasBomb = true;
                break;
            }
        }
        if (hasBomb)
        {
            queue.clear();
            for (int i = 0; i < MAX_W; i++)
            {
                for (int j = 0; j < MAX_H; j++)
                {
                    Cell * cell = __grid[i][j];
                    if (cell->visible && cell->color == __current->color)
                    {
                        queue.push_back(cell);
                    }
                }
            }
        }

        for (std::vector<Cell *>::iterator it = queue.begin(); it != queue.end(); it++)
        {
            (*it)->visible = false;
        }
        __current->visible = true;
        __current->setIsBomb(false);

        __score += queue.size() + 3 * (queue.size() - 3);
        __font->setText(__score);
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
        __current->setIsBomb(0);

        __end->visible = true;
        __end->setPosition(__current->getX(), __current->getY());

        __gameOver->visible = true;
        __gameOver->active = true;
    }
}

void MainScene::_update(float Elapsed)
{
    __gameOver->update(Elapsed);

    if (__shakingTutorial > 0)
    {
        __shakingTutorial -= Elapsed;
        if (__shakingTutorial <= 0)
        {
            __tutorial->setPosition((SCREEN_WIDTH - __tutorial->w) >> 1, 350);
        }
        else
        {
            __tutorial->setPosition(((SCREEN_WIDTH - __tutorial->w) >> 1) + MathUtil::Instance()->randomIn(-2, 2), 350 + MathUtil::Instance()->randomIn(-2, 2));
        }
    }
}

void MainScene::touchPressed(int X, int Y)
{
    if (__isOver)
    {
        return;
    }

    __pressedX = X;
    __pressedY = Y;
}

void MainScene::touchReleased(int PrevX, int PrevY, int X, int Y)
{
    int dx = X - __pressedX;
    int dy = Y - __pressedY;

    if (std::abs(dx) < 5 && std::abs(dy) < 5)
    {
        return;
    }

    int dir = 0;
    if (std::abs(dx) > std::abs(dy))
    {
        if (dx > 0)
        {
            dir = 0;
        }
        else
        {
            dir = 1;
        }
    }
    else
    {
        if (dy > 0)
        {
            dir = 2;
        }
        else
        {
            dir = 3;
        }
    }

    if (!(__tutorialFlag & 1))
    {
        if (dir != 3)
        {
            __shakingTutorial = 0.3f;
            return;
        }
    }
    else if (!(__tutorialFlag & (1 << 1)))
    {
        if (dir != 0)
        {
            __shakingTutorial = 0.3f;
            return;
        }
    }
    else if (!(__tutorialFlag & (1 << 2)))
    {
        if (dir != 2)
        {
            __shakingTutorial = 0.3f;
            return;
        }
    }
    else if (!(__tutorialFlag & (1 << 3)))
    {
        if (dir != 1)
        {
            __shakingTutorial = 0.3f;
            return;
        }
    }

    __move(dir);
}

void MainScene::touchMoved(int SrcX, int SrcY, int X, int Y)
{
}

void MainScene::onNotify(Message * Message)
{
    LOGD("MainScene::onNotify cmd=%d", Message->cmd);
    switch (Message->cmd)
    {
        case 0:
            __reset();
            break;

        case 1:
            break;

        default:
            break;
    }
}