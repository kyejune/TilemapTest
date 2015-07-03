//
//  Block.h
//  TilemapTest
//
//  Created by user on 6/29/15.
//
//

#ifndef __TilemapTest__Block__
#define __TilemapTest__Block__

#include "cocos2d.h"

USING_NS_CC;

class Block: public cocos2d::DrawNode{
    
public:
    Block(){};
    ~Block(){};
    CREATE_FUNC(Block);
    virtual bool init();
    
    
    
    Block* parentBlock;
    int coordX;
    int coordY;
    
private:
    
};

#endif /* defined(__TilemapTest__Block__) */
