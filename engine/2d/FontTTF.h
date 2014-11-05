//
//  FontTTF.h
//  jEngine
//
//  Created by Khanh Bui on 10/16/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jEngine__FontTTF__
#define __jEngine__FontTTF__

#include "Node.h"
#include "Texture.h"

#include <string>
#include <unordered_map>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_STROKER_H

NAMESPACE_BEGIN

class FontTTF : public Node
{
private:
    char * __text;
    
protected:
//    virtual void _update(float Elapsed);
//    virtual void _draw();
    
public:
    FontTTF();
    virtual ~FontTTF();

    void setText(const char * Text);
};

NAMESPACE_END

#endif /* defined(__jEngine__FontTTF__) */
