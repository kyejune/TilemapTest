//
//  UserDot.cpp
//  TilemapTest
//
//  Created by user on 4/10/15.
//
//

#include "UserDot.h"


//UserDot* UserDot::create()
//{
//    UserDot* instance = new(std::nothrow) UserDot();
//    
//    if( instance && instance->init() )
//    {
//        instance->autorelease();
//    }
//    else
//    {
//        CC_SAFE_DELETE( instance );
//    }
//    
//
//    
//    return instance;
//}


bool UserDot::init()
{
    
    
//    Director::getInstance()->getTextureCache();
    //==
//    TextureCache::getInstance();
    
//    _director->getTextureCache()
    
//    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("userdot.png");
    
//    Sprite::initWithTexture( texture );
    
    if( this->initWithFile("userdot.png") )
    {
//        this->initWithFile("");
        
//        float radius = this->getBoundingBox().size.width * .5;
//        float r = radius*.5;
        
        
//        DrawNode* left = DrawNode::create();
//        left->drawCircle(Vec2(-r, radius), r, 0, 4, true, 1, 1, Color4F::Color4F::BLUE );
//        this->addChild( left );
//        
//        DrawNode* right = DrawNode::create();
//        right->drawCircle(Vec2(radius*2 + r, radius), r, 0, 4, true, 1, 1, Color4F::Color4F::BLUE );
//        this->addChild( right );
        
        float w = this->getBoundingBox().size.width;
        float h = this->getBoundingBox().size.height;
        
        DrawNode* frame = DrawNode::create();
        frame->drawRect(Vec2(0,0), Vec2(w, h ), Color4F::Color4F::ORANGE );
        
        
        DrawNode* head = DrawNode::create();
        head->drawTriangle( Vec2( w/2, h + 8 ), Vec2( 10, h ), Vec2( w-10, h ), Color4F::Color4F::GREEN );
        
        
        
        this->addChild(frame);
        this->addChild( head );
        
        
        return true;
    };
    
    
    return  false;
}
