//
//  GameScene.h
//  TilemapTest
//
//  Created by user on 6/19/15.
//
//

#ifndef __TilemapTest__GameScene__
#define __TilemapTest__GameScene__

#include <stdio.h>
#include "cocos2d.h"
#include "UserDot.h"
#include "box2d/Box2d.h"
#include "b2DebugDraw.h"
#include "GLES-Render.h"
#include "cocos2dx-alertview/AlertView.h"
#include "DarkMazeSpec.h"


class GameScene : public cocos2d::Scene {
public:
    bool init();
    
    CREATE_FUNC(GameScene);
    
};



#endif /* defined(__TilemapTest__GameScene__) */
