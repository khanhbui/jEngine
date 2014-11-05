//
//  MainScene.h
//  jEngine
//
//  Created by Khanh Bui on 7/31/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#ifndef __jExample__MainScene__
#define __jExample__MainScene__

#include "Scene.h"
#include "ObserverInterface.h"
#include "FontTTF.h"
#include "Sprite.h"

namespace jFont {

NAMESPACE_USING

class MainScene : public Scene, public ObserverInterface
{
private:
    FontTTF * __font;

    void __reset();

    void _update(float Elapsed);

public:
    MainScene(bool Tutorial);
    virtual ~MainScene();

    void touchPressed(int X, int Y);
    void touchReleased(int PrevX, int PrevY, int X, int Y);
    void touchMoved(int SrcX, int SrcY, int X, int Y);

    void onNotify(Message * Message);
};

}

#endif /* defined(__jExample__MainScene__) */
