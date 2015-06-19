/*******************************************************************************************************/
/*
 *
 * @File:           FPB2DebugDrawLayer.cpp
 * @Group:          Libraries
 * @Version:        Revision: 1.2.9   (Cocos2d-x V3.X)
 * @Last changed:   Date: 2014/08/06 08:03:06
 * @Created:        Date: 2014/08/05 05:03:06
 * @Purpose:        Process for FPB2DebugDrawLayer object.
 * @Author:         Harion
 * @Copyright:      (C)2012-2015 CreativeGame - G2TVN.,Corp
 * @Product:        :HerosWorld:
 *
 */
/*******************************************************************************************************/

#include "FPB2DebugDrawLayer.h"

#include "FPB2dWorld.h"

using namespace cocos2d;

/*====================================================================================================*/
/*
 * @Name:      create
 * @Class:     FPB2DebugDrawLayer
 * @Desc:      Construction B2 Debug Draw Layer object
 * @Param:     b2World* pB2World, float pPtmRatio
 * @Return:    operator
 */
/*----------------------------------------------------------------------------------------------------*/

FPB2DebugDrawLayer * FPB2DebugDrawLayer::create(b2World* pB2World, float pPtmRatio, int mod) {
    if( pB2World == nullptr )
    {
        pB2World = FPB2dWorld::getInstance()->getWorld();
    }
    
     FPB2DebugDrawLayer *pRet = new FPB2DebugDrawLayer(pB2World, pPtmRatio);
     if (pRet && pRet->init(mod)) {
          pRet->autorelease();
          return pRet;
     } else {
          delete pRet;
          pRet = NULL;
          return NULL;
     }
}


/*====================================================================================================*/
/*
 * @Name:      Constructor
 * @Class:     FPB2DebugDrawLayer
 * @Desc:      Construction B2 Debug Draw Layer object
 * @Param:     b2World* pB2World, float pPtmRatio
 * @Return:    FPB2DebugDrawLayer
 */
/*----------------------------------------------------------------------------------------------------*/

FPB2DebugDrawLayer::FPB2DebugDrawLayer(b2World* pB2World, float pPtmRatio) : mB2World(pB2World), mPtmRatio(pPtmRatio) {
}


/*====================================================================================================*/
/*
 * @Name:      init
 * @Class:     FPB2DebugDrawLayer
 * @Desc:      Initialization B2 Debug Draw Layer object
 * @Param:     void
 * @Return:    bool
 */
/*----------------------------------------------------------------------------------------------------*/

bool FPB2DebugDrawLayer::init(int mod) {
     if(!Layer::init()) {
          return false;
     }
     mB2DebugDraw = new b2DebugDraw( mPtmRatio );
     mB2World->SetDebugDraw(mB2DebugDraw);
     uint32 flags = 0;
     switch (mod) {
          case 1: {
               flags += b2Draw::e_shapeBit;
              flags += b2Draw::e_centerOfMassBit;
          } break;
          case 2: {
               flags += b2Draw::e_shapeBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_centerOfMassBit;
          } break;
          case 3: {
               flags += b2Draw::e_shapeBit;
               flags += b2Draw::e_jointBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_pairBit;
          } break;
          case 4: {
               flags += b2Draw::e_jointBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_pairBit;
          } break;
          case 5: {
               flags += b2Draw::e_shapeBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_centerOfMassBit;
          } break;
          case 6: {
               flags += b2Draw::e_jointBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_pairBit;
               flags += b2Draw::e_centerOfMassBit;
          } break;
          case 7: {
               flags += b2Draw::e_shapeBit;
               flags += b2Draw::e_jointBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_centerOfMassBit;
          } break;
          case 8: {
               flags += b2Draw::e_shapeBit;
               flags += b2Draw::e_jointBit;
               flags += b2Draw::e_pairBit;
               flags += b2Draw::e_centerOfMassBit;
          } break;
          case 9: {
               flags += b2Draw::e_shapeBit;
               flags += b2Draw::e_jointBit;
               flags += b2Draw::e_aabbBit;
               flags += b2Draw::e_pairBit;
               flags += b2Draw::e_centerOfMassBit;
          } break;
          default: flags += b2Draw::e_shapeBit;
               break;
     }
     mB2DebugDraw->SetFlags(flags);     
     return true;
}


/*====================================================================================================*/
/*
 * @Name:      draw
 * @Class:     FPB2DebugDrawLayer
 * @Desc:      Draw shader object
 * @Param:     Renderer *renderer, const Mat4& transform, bool transformUpdated
 * @Return:    void
 */
/*----------------------------------------------------------------------------------------------------*/

void FPB2DebugDrawLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
     GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
     Director* director = Director::getInstance();
     CCASSERT(nullptr != director, "Director is null when seting matrix stack");
     director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
     _modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
     _customCommand.init(_globalZOrder);
     _customCommand.func = CC_CALLBACK_0(FPB2DebugDrawLayer::onDraw, this);
     renderer->addCommand(&_customCommand);
     director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}


/*====================================================================================================*/
/*
 * @Name:      onDraw
 * @Class:     FPB2DebugDrawLayer
 * @Desc:      On draw shader object
 * @Param:     null
 * @Return:    void
 */
/*----------------------------------------------------------------------------------------------------*/

void FPB2DebugDrawLayer::onDraw() {
     Director* director = Director::getInstance();
     CCASSERT(nullptr != director, "Director is null when seting matrix stack");
     Mat4 oldMV;
     oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
     director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
     mB2World->DrawDebugData();
     director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}

