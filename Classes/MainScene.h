#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "UserDot.h"
#include "box2d/Box2d.h"
#include "b2DebugDraw.h"
#include "GLES-Render.h"
#include "cocos2dx-alertview/AlertView.h"
#include "DarkMazeSpec.h"
//#include "b2DebugDraw.h"
//#include "FPB2DebugDrawLayer.h"
#include "DebugMap.h"


class MainScene : public cocos2d::Scene
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(MainScene);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
//    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    
    
    // a selector callback
//    void menuCloseCallback(cocos2d::Ref* pSender);
    
//    void OnUpdate( float dt );
    
    void OnAcceleration( cocos2d::Acceleration *acc, cocos2d::Event *event );
    
    void onClosedAlert( EventCustom* evt );
    
    // implement the "static create()" method manually
    
    
    int parseMap( std::string fileName );
    
    int getApproximateDegree( int degree );
    
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    bool isTouchDown;
    bool isMoving;
    
    cocos2d::Size visibleSize;
    
    float initialTouchPos[2];
    float currentTouchPos[2];
    
    void update(float dt);
    void doneMoving();
    
    int coordinates[2];
//    std::vector<int> getCoord( float x, float y );
//    std::vector<int> getUserCoord();
    void setCoord( int x, int y );
    void setCoordByDirection( DarkMazeSpec::Direction direction );
    void setLineOfSight( DarkMazeSpec::Direction direction );
    
    
    
    //// OVERRIDE ////
//    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
//    

private:
    
    
    float MOVING_TIME = 0.4;
    
    float PTM_RATIO = 32;
    
    cocos2d::Vec2 prevPos;
    cocos2d::Vec2 curPos;
    
    float m_accelerateFactor = 100;
    float m_accelerateRate   = .1;
    
//    cocos2d::Vec2 targetPosition;
//    float targetRotation;
    
    Vec2 nextPos;
    float nextRot = 0;
    
    double accX;
    double accY;
    
    void getNewPosition( double ax, double ay );
//    bool isHitWall( float tx, float ty );
//    void settingBox2d();
    
//    void makeBall();
    
    void fixUserAtCenter();
    
    
//    GLESDebugDraw* debugDraw;
    
//    Sprite* container = Sprite::create();
//    Sprite* mapContainer = Sprite::create();
    TMXTiledMap* tmapBg;
    Layer* gameLayer;// = Layer::create();
    DebugMap* dmap;
//    Sprite* gameSprite = Sprite::create();
    
    UserDot* ud;
//    b2Body* udBody;
//    b2Fixture* udFix;
//    b2CircleShape* udShape = new b2CircleShape();
//    
//    b2World* world;
//    b2Body* body;
//    FPB2DebugDrawLayer* debugDraw;
    
    b2Vec2 exitPos;
    
    
    Vec2 rotatePoint( Vec2 anchor, Vec2 point, float angle );
    
};




#endif // __HELLOWORLD_SCENE_H__
