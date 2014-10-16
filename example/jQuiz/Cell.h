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

namespace jQuiz {

NAMESPACE_USING;

class Cell : public Sprite
{
private:

public:
    Cell(int GridX, int GridY);
    virtual ~Cell();

    static const int SIZE = 64;
    static const int MAX = 71;
    
    static const char * HIRAGANA[];
    static const char * ROMANJI[];

    int gridX;
    int gridY;
    
    int index;
    
    void setChar(int Index, int Type);
};

}

#endif /* defined(__jEngine__Cell__) */
