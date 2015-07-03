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
#include "D.h"
#include <unordered_map>
USING_NS_CC;

class DebugMap: public cocos2d::DrawNode{
  
public:
    DebugMap(){};
    ~DebugMap(){};
    CREATE_FUNC(DebugMap);
    virtual bool init();
    
    
    void parseMap( cocos2d::TMXTiledMap* tmap );
    D::Load checkLoad( int startCoordX, int startCoordY, int endCoordX, int endCoordY );
    int getFastDistance( int startCoordX, int startCoordY, int endCoordX, int endCoordY );
    
    Size tilesLen;
    Size tilesSize;
    
    
    
private:
    float PTM_RATIO = 32;
    Vec2 goal;
    Vec2 start;
    std::vector< std::vector<Vec2>> wallSeg;
    std::unordered_map< int, Vec2 > parentMap = {};
    std::vector<Vec2> closedCoords;
    std::vector<Vec2> getOpenTiles( Vec2 coord );
    bool findNext( Vec2 coord );
    bool isMarkedCoord( Vec2 coord );
    
//    std::vector<Vec2> dummy;
//    Map<int, Vec2> pmap;
//    DictElement map;
    
    int accumulateDistance( std::string xy, int accumulatedDistance );
};


#endif /* defined(__TilemapTest__DebugMap__) */
