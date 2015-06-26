//
//  DebugMap.cpp
//  TilemapTest
//
//  Created by user on 6/25/15.
//
//

#include "DebugMap.h"
#include "DarkMazeSpec.h"

USING_NS_CC;


bool DebugMap::init()
{

    if( DrawNode::init() ){
        return true;
    }else{
        return false;
    }
}

void DebugMap::parseMap( cocos2d::TMXTiledMap* tmap  ){
    
    TMXObjectGroup* wall = tmap->getObjectGroup("linewall");
    
    Size mapSize = tmap->getContentSize();
    Size tileLen  = tmap->getMapSize();
    Size tileSize = tmap->getTileSize()/2;
    
//    for( int i=0; i<tileLen.height+1; i++ ) this->drawLine( Vec2( 0, i*(tileSize.height)), Vec2( mapSize.width, i*(tileSize.height) ), Color4F::GREEN );
//    for( int j=0; j<tileLen.width+1; j++ )  this->drawLine( Vec2( j*(tileSize.width), 0 ), Vec2( j*(tileSize.width), mapSize.height ), Color4F::GREEN );
    
    
//    CCLOG( "red %i green %i blue %i", color.r,color.g, color.b);
    
    
    CCLOG(" Line... %f, %f", tmap->getMapSize().width, tmap->getMapSize().height ); // 갯수
    CCLOG(" Line... %f, %f", tmap->getTileSize().width, tmap->getTileSize().height ); // 크기
    
    
    this->drawSolidRect( Vec2(0,0), Vec2(100,100), Color4F::RED );
//    this->drawSegment(Vec2(0,0), Vec2(100,100), 3.0f, Color4F::MAGENTA );
    
//    this->drawRect(Vec2(0,0), Vec2(100,100), Color4F::RED );
    
    for( auto obj : wall->getObjects())
    {
        auto xx = obj.asValueMap()["x"].asFloat();
        auto yy = obj.asValueMap()["y"].asFloat();
        
        // 벽 생성 및 위치
//        std::vector<b2Vec2> wallPoints;
        bool first = true;
        Vec2 prevPoint;
        for( auto value : obj.asValueMap() )
        {
            if( value.first == "polylinePoints"){
                
                auto vec = value.second.asValueVector();
                
                for( auto &p : vec ){
                    Vec2 point = Vec2( ( p.asValueMap().at("x").asFloat()*.5 ) + xx, -( p.asValueMap().at("y").asFloat()*.5 ) + yy );
                    
                    if( first == false ){
//                        CCLOG( "DrawSegment: %f x %f    %f x %f", prevPoint.x, prevPoint.y, point.x, point.y );
                        this->drawSegment( prevPoint, point, 4.0f, Color4F::ORANGE );
                    }
                    
                    
                    prevPoint = point;
                    first = false;
                }//for
            }//if
        }//for
    }
    
    
    // Get ColorMap!!
//    CCLOG( "getColorMap: %f x %f ", DarkMazeSpec::mapWidth, DarkMazeSpec::mapHeight );
    RenderTexture* texture = RenderTexture::create( mapSize.width, mapSize.height );
    texture->begin();
    this->visit();
    texture->end();
    
    
    CCLOG( "map Size is  %fx%f ", mapSize.width, mapSize.height );    
    
    Vec2 pos = Vec2( 0, 10 );
    Size s = Size( 160, 1 );
    int n = (s.width * s.height);
    Color4B *buffer = (Color4B *)malloc( sizeof(Color4B) * n );
    glReadPixels( pos.x , pos.y , s.width , s.height , GL_RGBA, GL_UNSIGNED_BYTE, buffer );
    for( int i = 0 ; i < n ; i++ )
    {
        CCLOG(" (%d) , 0x%02x%02x%02x%02x" , i ,  buffer[i].r , buffer[i].g , buffer[i].b , buffer[i].a );
    }

//    
//    for( int xx=0; xx<tileLen.width; xx++ ){
//        for( int yy=0; yy<tileLen.height; yy++ ){
//          //  CCLOG( "%dx%d:", xx, yy );
//            
//            Color4B *b = (Color4B *)malloc( sizeof(Color4B) * 1 );
//            Vec2 p = Vec2( xx*tileSize.width, yy*tileSize.height + tileSize.height/2 );
//            glReadPixels( p.x , p.y , 1 , 1 , GL_RGBA, GL_UNSIGNED_BYTE, b );
//            
//            CCLOG(" (%dx%d) (%dx%d) , 0x%02x%02x%02x%02x" , xx,yy, int(p.x), int(p.y), b[0].r , b[0].g , b[0].b , b[0].a );
//            
//        }
//    }
    
    
    //Vec2::isSegmentIntersect( a, b, c, d ) 이걸로 선분 충돌 검사를 해서 끝장을 보자!!
    Texture2D* texture2d = texture->getSprite()->getTexture();
    Sprite* sprite = Sprite::createWithTexture(texture2d );
    addChild(sprite);
    
    
    sprite->setPosition( 100, 400 );
    sprite->setScale(1, -1);
}

bool DebugMap::testLine(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y){
    
        float s1_x, s1_y, s2_x, s2_y;
        s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
        s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;
        
        float s, t;
        s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
        t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
        
        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        {
            // Collision detected
            if (i_x != NULL)
                *i_x = p0_x + (t * s1_x);
            if (i_y != NULL)
                *i_y = p0_y + (t * s1_y);
            return true;
        }
        
    return false;
}

void DebugMap::getColorMap(){
    
//    sprite->setRotation( 45 );
}