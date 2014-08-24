//
//  Cell.h
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/13/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__Cell__
#define __jEngine__Cell__

#include "Sprite.h"

NAMESPACE_USING;

class Cell : public Sprite
{
private:
    Sprite * __bomb;
    
    int __isBomb;

public:
    Cell(int GridX, int GridY, int Color);
    virtual ~Cell();

    static const int SIZE = 40;
    static const int MAX_COLORS = 4;
    static const int MAX_BOMBS = 6;

    int gridX;
    int gridY;

    int color;

    void setColor(int Color);

    void setIsBomb(int IsBomb);
    int getIsBomb() const;

    void countDownBomb();
};

#endif /* defined(__jEngine__Cell__) */
