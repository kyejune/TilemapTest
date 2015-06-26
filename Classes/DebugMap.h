//
//  DebugMap.h
//  TilemapTest
//
//  Created by user on 6/25/15.
//
//

#ifndef __TilemapTest__DebugMap__
#define __TilemapTest__DebugMap__

#include <stdio.h>

class DebugMap: public cocos2d::DrawNode{
  
public:
    DebugMap(){};
    ~DebugMap(){};
    CREATE_FUNC(DebugMap);
    virtual bool init();
    
    
    void parseMap( cocos2d::TMXTiledMap* tmap );
    void getColorMap();
    
    
private:
    
    float PTM_RATIO = 32;
    
    bool testLine( float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y );
    
};


#endif /* defined(__TilemapTest__DebugMap__) */
