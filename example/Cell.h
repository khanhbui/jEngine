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
    Sprite * __begin;
    Sprite * __end;

public:
    Cell(int GridX, int GridY, int Color);
    ~Cell();

    int gridX;
    int gridY;
    int color;
    int state;

    void setState(int State);

    void setColor(int Color);
};

#endif /* defined(__jEngine__Cell__) */
