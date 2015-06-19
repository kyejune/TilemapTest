//
//  Wall.h
//  TilemapTest
//
//  Created by user on 4/15/15.
//
//

#ifndef __TilemapTest__Wall__
#define __TilemapTest__Wall__


#include "cocos2d.h"
#include "box2d/Box2d.h"
#include <Box2D/Collision/Shapes/b2EdgeShape.h>


class b2Util{
  
public:
    b2Util(){};
    ~b2Util(){};
    
    
   
    
   // CREATE_FUNC(b2EdgeShape);
    
//    virtual bool init();
    
    
//    b2EdgeShape addPoint4Polygon( cocos2d::Vec2 pt );
//    void makePolygon();
    
    
    
private:
    
    
    
    
// Static
public:
    
//    static float sizePerX = 0.5;
//    static float sizePerY = 0.5;
    
    static void drawEdgeAtBody( b2Body* body, std::vector<b2Vec2> points, float sx = 0.0f, float sy = 0.0f ){
        
        b2EdgeShape edge;
        b2FixtureDef def;
        
        def.shape = &edge;
        
        int len = int( points.size() );
        
        for( int i=0; i<len; i++ ){
            edge.Set(points.at(i), points.at((i+1)%len));
            body->CreateFixture(&def);
        }
    }
    
    
    static float syncValue( float inValue, float inMin, float inMax, float outMin, float outMax ){
        return ( outMax - outMin ) / ( inMax - inMin ) * ( inValue - inMin ) + outMin;
    }
    
//    static void getPointBLfromTL
    
    
//    static b2Vec2 tiledToCocosPosition
    
};







#endif /* defined(__TilemapTest__Wall__) */