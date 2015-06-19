//
//  Wall.cpp
//  TilemapTest
//
//  Created by user on 4/15/15.
//
//

#include "b2Util.h"



//bool EdgeShapeDef::init()
//{
//    return  true;
//}



//void EdgeShapeDef::addPointForPolygon( cocos2d::Vec2 pt ){
//    CCLOG("addPoint: %f x %f", pt.x, pt.y );
//}


//void EdgeShapeDef::makePolygon(){
//    
//}

//static void drawEdgeAtBody( b2Body* body, cocos2d::Vector<b2Vec2> points ){
//    
//    b2EdgeShape edge;
//    b2FixtureDef def;
//    
//    def.shape = &edge;
//    
//    auto len = points.size();
//    for ( int i=0; i<len-1; i+=2 ) {
//        edge.Set( points.at(i), points.at(i+1));
//        body->CreateFixture( &def );
//    }
//}