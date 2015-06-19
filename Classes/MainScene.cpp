#include "MainScene.h"
#include "UserDot.h"
#include "b2Util.h"
#include "box2d/Box2d.h"
#include "b2DebugDraw.h"
#include "GLES-Render.h"
#include "B2DebugDrawLayer.h"
#include "DarkMazeSpec.h"
#include "cocos2dx-alertview/AlertView.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

USING_NS_CC;

//using namespace cocos2d;


//Scene* MainScene::createScene()
//{
//    // 'scene' is an autorelease object
//    auto scene = Scene::create();
//
//    // 'layer' is an autorelease object
//    auto layer = MainScene::create();
//
//    // add layer as a child to scene
//    scene->addChild(layer);
//
//    // return the scene
//    return scene;
//}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    // 맵 생성 및 사이즈 맞추기
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    //    container->setAnchorPoint(Point::ANCHOR_MIDDLE);
    
    this->tmapBg = TMXTiledMap::create("./darkmaze.tmx");
    
    DarkMazeSpec::mapWidth  = tmapBg->getContentSize().width;
    DarkMazeSpec::mapHeight = tmapBg->getContentSize().height;
    DarkMazeSpec::canvasScaleX = visibleSize.width/DarkMazeSpec::mapWidth;
    DarkMazeSpec::canvasScaleY = visibleSize.height/DarkMazeSpec::mapHeight;
    DarkMazeSpec::stepW = DarkMazeSpec::mapWidth/DarkMazeSpec::stepWidthMax;
    DarkMazeSpec::stepH = DarkMazeSpec::mapHeight/DarkMazeSpec::stepHeightMax;
    
    
    
    // 유저 생성
    ud = UserDot::create();
    
    // 출구 위치 기록
    TMXObjectGroup* obj = tmapBg->getObjectGroup("obj");
    ValueMap exit = obj->getObject("exit");
    exitPos = b2Vec2( (exit["x"].asFloat() + exit["width"].asFloat()*.5)/PTM_RATIO,  (exit["y"].asFloat() - exit["height"].asFloat()*.5)/PTM_RATIO );
    //    exitX = exit["x"].asFloat() + exit["width"].asFloat()*.5;
    //    exitY = exit["y"].asFloat() - exit["height"].asFloat()*.5;
    
    
    // 맵 파싱
    settingBox2d();
    parseMap( std::string( "./darkmaze.tmx" ) );
    
    // touch 관련
    auto swipeListener = EventListenerTouchOneByOne::create();
    swipeListener->setSwallowTouches(true);
    swipeListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    swipeListener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    swipeListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    swipeListener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority( swipeListener, this );
    this->scheduleUpdate();
    
    
    this->addChild( gameLayer );
    gameLayer->addChild( tmapBg, 0 );
    gameLayer->addChild( ud, 1 );
    
    
//    this->addChild( container );
//    container->addChild( gameLayer, 0 );
    
//    container->setContentSize(visibleSize);
//    mapContainer->addChild( tmapBg );
//    container->addChild( mapContainer );
    
    
    
//    container->ignoreAnchorPointForPosition( true );
//    container->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    gameLayer->setScale( DarkMazeSpec::canvasScaleX, DarkMazeSpec::canvasScaleY );
    
    
    
    // 초기화
    setCoord( 4, 5 );
    ud->setRotation( 0 );
//    container->setRotation( 0 );
    this->setScale(.3);
    
    
    
    
    
    return true;
}

void MainScene::OnUpdate( float dt ){
    
    // 이동
    //    this->getNewPosition( this->accX, this->accY );
    //    ud->setPosition(this->curPos);
    //    this->prevPos = this->curPos;
    
    //    this->getNewPosition(this->accX, this->accY );
    //    udBody->SetTransform( b2Vec2(curPos.x/PTM_RATIO, curPos.y/PTM_RATIO), ud->getRotation());
    //    udBody->ApplyForce( b2Vec2(curPos.x/PTM_RATIO, curPos.y/PTM_RATIO), b2Vec2(ud->getPositionX()/PTM_RATIO, ud->getPositionY()/PTM_RATIO), false );
    
    //    udBody->SetLinearVelocity(offset)
    
    
    //    Vec2 simple = Vec2( accX, accY ) * 50 *  m_accelerateFactor;
    //    simple.x = int( simple.x );
    //    simple.y = int( simple.y );
    //
    //    if( simple.x == 0 && simple.y ==0 ) return;
    //
    //
    //    b2Vec2 cur = b2Vec2( curPos.x, curPos.y );
    //    b2Vec2 pre = b2Vec2( prevPos.x, prevPos.y );
    b2Vec2 offset = b2Vec2( accX *10, accY *10 );
    udBody->SetLinearVelocity(offset);
    
    this->prevPos = this->curPos;
    
    // 각 body들을 받아오고 아이콘 body에는 userData에 스프라이트를 넣어기때문에 바디의 위치변화에 따라 스프라이트도 따라 움직여줌
    for( b2Body *b = world->GetBodyList(); b; b=b->GetNext()){
        if( b->GetUserData() != nullptr ){
            Sprite *sprite = (Sprite *)b->GetUserData();
            sprite->setPosition(Vec2( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO * (1/DarkMazeSpec::canvasScaleY) ));
            sprite->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
    }
    
    // 출구와 접촉하는지 검사
    bool isContactExit = udFix->TestPoint( exitPos );
    if( isContactExit ){
        CCLOG("HIT EXIT" );
        
        auto alert = cocos2d::plugin::AlertView::create("Goal in", "추카추카" );
        alert->setPositive( "Close" );
        alert->show();
        
        getEventDispatcher()->addCustomEventListener( cocos2d::plugin::AlertView::NOTIFICATION(),  CC_CALLBACK_1( MainScene::onClosedAlert, this ));
        
        
        this->unschedule(schedule_selector( ::MainScene::OnUpdate ));
    }
    
    world->Step(dt, 100, 100);
    
    
    //    body->set
    
        CCLOG("onUpdate");
}

void MainScene::onClosedAlert( EventCustom* evt ){
    
    CCLOG("onClosedAlert: %s %d %d", evt->getEventName().c_str(), evt->getType(), cocos2d::plugin::AlertView::EventType::POSITIVE );
    
    
    auto userdata = (int)(intptr_t)evt->getUserData();
    CCLOG(">>>>> %d", userdata );
    
    this->schedule( schedule_selector(MainScene::OnUpdate), 1.0f/60.0f );
}


void MainScene::OnAcceleration( cocos2d::Acceleration *acc, Event *evt ){
    this->accX = acc->x;
    this->accY = acc->y;
}

void MainScene::getNewPosition( double ax, double ay ){
    
    Vec2 simple = Vec2( ax, ay ) * 50 *  m_accelerateFactor;
    simple.x = int( simple.x );
    simple.y = int( simple.y );
    Vec2 vec = simple * m_accelerateRate + ( 1 - m_accelerateRate ) * Vec2( prevPos.x, prevPos.y );
    
    this->curPos = prevPos+vec;
}

int MainScene::parseMap( std::string fileName ){
    
    
    TMXObjectGroup* wall = tmapBg->getObjectGroup("wallobj");
    
    
    auto mapH = DarkMazeSpec::mapHeight/PTM_RATIO;
    auto ptX = 0.0f;
    auto ptY = 0.0f;
    
    for( auto obj : wall->getObjects())
    {
        auto xx = obj.asValueMap()["x"].asFloat()/PTM_RATIO;
        auto yy = obj.asValueMap()["y"].asFloat()/PTM_RATIO;
        
        // 벽 생성 및 위치
        std::vector<b2Vec2> wallPoints;
        
        for( auto value : obj.asValueMap() )
        {
            if( value.first == "polylinePoints"){
                
                auto vec = value.second.asValueVector();
                
                for( auto &p : vec ){
                    
                    ptX = ( p.asValueMap().at("x").asFloat()*.5 )/PTM_RATIO;
                    ptY = b2Util::syncValue( ( p.asValueMap().at("y").asFloat()*.5 )/PTM_RATIO, 0, mapH, mapH, 0 );
                    
                    b2Vec2 v2 = b2Vec2( (xx + ptX)*DarkMazeSpec::canvasScaleX, (yy + ptY - mapH)*DarkMazeSpec::canvasScaleY  );
                    
                    wallPoints.push_back( v2 );
                }//for
            }//if
        }//for
        
        
        
        if( wallPoints.size() > 1 ) b2Util::drawEdgeAtBody( body, wallPoints );
    }
    
    return 0;
}

void MainScene::settingBox2d(){
    
    Size winSize = Director::getInstance()->getWinSize();
    
    
    // world세팅
    debugDraw = new GLESDebugDraw( PTM_RATIO );
    b2Vec2 gravity = b2Vec2( 0.0f, 0.0f );
    world = new b2World( gravity );
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    
    debugDraw->SetFlags(flags);
    world->SetDebugDraw( debugDraw );
    
    // body 생성
    b2BodyDef screen_body_def;
    screen_body_def.position.Set( 0.0f, 0.0f );
    body = world->CreateBody( &screen_body_def );
    
    
    // 화면을 감싸는 박스형태 엣지쉐이프 생성
    std::vector<b2Vec2> box;
    box.push_back( b2Vec2( 0.0f, 0.0f ) );
    box.push_back( b2Vec2( 0, winSize.height/PTM_RATIO ) );
    box.push_back( b2Vec2( winSize.width/PTM_RATIO, winSize.height/PTM_RATIO ) );
    box.push_back( b2Vec2( winSize.width/ PTM_RATIO, 0 ) );
    
    b2Util::drawEdgeAtBody( body, box );
    
    makeBall();
}




void MainScene::makeBall(){
    Size winSize = Director::getInstance()->getWinSize();
    
    // 위 아이콘에매칭되는 body를 생성, 위치는 meter로 변환하여 설정하여야함
    b2BodyDef icon_body_def;
    //    b2CircleShape circle;
    udShape->m_radius = ( ud->getContentSize().width*.5 * ud->getScaleX() ) / PTM_RATIO;
    
    icon_body_def.type = b2_dynamicBody;
    icon_body_def.position.Set(ud->getPositionX()/PTM_RATIO, ud->getPositionY()/PTM_RATIO);
    icon_body_def.userData = ud;
    
    udBody = world->CreateBody(&icon_body_def);
    
    
    //    circle.
    
    // polygonShape의 setAsBox는 스프라이트의 중심점을 잡아주면 됨
    b2PolygonShape icon_polygon;
    
    b2FixtureDef icon_fixture_def;
    icon_fixture_def.shape = udShape;
    udFix = udBody->CreateFixture(&icon_fixture_def);
    
    
}



// --------------------- TouchEvent

bool MainScene::onTouchBegan(Touch *touch, Event *event)
{
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    isTouchDown = true;
    
    //    CCLOG("onTouchBegan");
    
    return true;
}

void MainScene::onTouchMoved(Touch *touch, Event *event)
{
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    //    CCLOG("onTouchMoved");
}

void MainScene::onTouchEnded(Touch *touch, Event *event)
{
    isTouchDown = false;
    
    //    CCLOG("onTouchEnd");
}

void MainScene::onTouchCancelled(Touch *touch, Event *event)
{
    onTouchEnded(touch, event);
    
    //    CCLOG("onTouchCancelled");
}

void MainScene::update(float dt)
{
    
    //    CCLOG("update %s", isTouchDown?"o":"x" );
    
    
    if (true == isTouchDown)
    {
        int offsetX = std::abs( initialTouchPos[0] - currentTouchPos[0] );
        int offsetY = std::abs( initialTouchPos[1] - currentTouchPos[1] );
        
        
        
        
        
        
        //       MathUtil.roundf( offsetX );
        //        MathUtil.abs(MathUtil.roundf(offsetY) );
        
        if(  offsetX > offsetY ){
            if (initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.05)
            {
                CCLOG("SWIPED LEFT");
                
                isTouchDown = false;
                setLineOfSight( DarkMazeSpec::LEFT );
            }
            else if (initialTouchPos[0] - currentTouchPos[0] < - visibleSize.width * 0.05)
            {
                CCLOG("SWIPED RIGHT");
                
                isTouchDown = false;
                setLineOfSight( DarkMazeSpec::RIGHT );
            }
        }else{
            if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.05)
            {
                CCLOG("SWIPED DOWN");
                
                isTouchDown = false;
                setCoordByDirection( DarkMazeSpec::TOP );
            }
            else if (initialTouchPos[1] - currentTouchPos[1] < - visibleSize.width * 0.05)
            {
                CCLOG("SWIPED UP");
                
                isTouchDown = false;
                setCoordByDirection( DarkMazeSpec::BOTTOM );
            }
        }
    }
    
    
    
    // tile동기화
    //body->SetTransform(b2Vec2(0,0), container->getRotation()*M_PI/180 );
    
    //    mapContainer->setRotation( )
    
    
    //
    //    udBody->SetTransform( b2Vec2( ud->getPositionX(), ud->getPositionY() ), ud->getRotation());
    
    
    //        Layer::draw(renderer, transform, flags);
    
//    gameLayer->draw();
//    Director* director = Director::getInstance();
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION );
//    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    world->DrawDebugData();
//    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    
//    world->DrawDebugData();
//    world->Step(dt, 100, 100);
}

//void MainScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
//    Layer::draw(renderer, transform, flags);
//    Director* director = Director::getInstance();
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION );
//    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    world->DrawDebugData();
//    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//}




//std::vector<int> MainScene::getCoord( float x, float y ){
//    std::vector<int> pos(2);
//
//
//
//    pos[0] = floor(x/DarkMazeSpec::stepW);
//    pos[1] = floor(y/DarkMazeSpec::stepH);
//
//    CCLOG("getCoord: %f, %f ==> %d, %d", x, y, pos[0], pos[1] );
//
//    return pos;
//}

//
//std::vector<int> MainScene::getUserCoord(){
//    return getCoord( ud->getPositionX(), ud->getPositionY() );
//}


void MainScene::setLineOfSight( DarkMazeSpec::Direction direction ){
    switch ( direction ) {
        case DarkMazeSpec::RIGHT:
            gameLayer->setRotation( gameLayer->getRotation() + 90 );
            break;
            
        case DarkMazeSpec::LEFT:
            gameLayer->setRotation( gameLayer->getRotation() - 90 );
            break;
            
        default:
            break;
    }
    
    ud->setRotation( -gameLayer->getRotation() );
}

void MainScene::setCoord( int x, int y ){
    //    canvas->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    
    CCLOG( "setCoord %d, %d ==> %f, %f :", x, y, x*DarkMazeSpec::stepW, y*DarkMazeSpec::stepH  );
    
    Vec2 prev = ud->getPosition();
    Vec2 next = Vec2( x*DarkMazeSpec::stepW + DarkMazeSpec::stepW/2, y*DarkMazeSpec::stepH + DarkMazeSpec::stepH/2 );
    
    
    // 이전 위치와 비교해서 회전
    //    float angle  = atan2f( prev.y - next.y, prev.x - next.x );
    //    float degree = ((angle * 180) / M_PI );
    //
    //
    //    if( std::abs(degree) == 90 ) degree += 90;
    //    else                         degree -= 90;
    //
    //
    //    CCLOG( "   UD rotation: %f --> %f", (angle*180)/M_PI, degree );
    //
    //    ud->setRotation( degree );
    ud->setPosition(next);
    
    
    //    CCLOG("-----------: %d, %d", coordinates[0], coordinates[1] );
    
    coordinates[0] = x;
    coordinates[1] = y;
    
    
    // ud가 움직인만큼 map을 상대적으로 이동해서 ud를 늘 센터에 위치하게끔.. & 회전
    //    canvas->setPosition( -next + Vec2( DarkMazeSpec::mapWidth/2, DarkMazeSpec::mapHeight/2 ) );
    //    this->setContentSize(canvas->getContentSize());
    //    this->ignoreAnchorPointForPosition(true);
    //    this->setRotation( -degree );
    //    canvas->setAnchorPoint( next );
    //    canvas->setContentSize(<#const cocos2d::Size &contentSize#>)
    
    //    container->ignoreAnchorPointForPosition(true);
    //    container->setContentSize( Size( DarkMazeSpec::mapWidth, DarkMazeSpec::mapHeight));
    //    container->setAnchorPoint( next );
    
    
    //    container->setRotation(-degree);
    
    
//    CCLOG("getContentSize: %f x %f", gameLayer->getContentSize().width, canvas->getContentSize().height );
    
    
    
    //    canvas->setAnchorPoint( next );
    //    canvas->setRotation( -degree );
}


void MainScene::setCoordByDirection( DarkMazeSpec::Direction direction ){
    
    const int d[4] = { 0, 90, 180, -90 };
    int swipeR     = d[direction];
    int containerR = std::floor( gameLayer->getRotation() + 0.5 );
    
    CCLOG( "%u >>>>>>> %d, - %d, = %d", direction, containerR, swipeR, containerR-swipeR );
    
    switch ( (containerR-swipeR)%360 ) {
        case 0:
            setCoord( coordinates[0], coordinates[1] + 1 );
            break;
            
            
        case 180:
        case -180:
            setCoord( coordinates[0], coordinates[1] - 1 );
            break;
            
            
        case 90:
        case -270:
            setCoord( coordinates[0] - 1, coordinates[1] );
            break;
            
        case -90:
        case 270:
            setCoord( coordinates[0] + 1, coordinates[1] );
            break;
            
        default:
            
            break;
    }
    
    
    //    CCLOG("setCoordByDirection:%f-%f = %d", ud->getRotation(), container->getRotation(),  offsetR );
    //    getApproximateDegree( offsetR );
}



//int MainScene::getApproximateDegree( int degree ){
//    int index = 0;
//    int degrees[4] = { 0, 180, -180, 360 };
//    CCLOG( "<< getApproximateDegree %d", degree );
//    int minOffset = 1000;
//    for (int i=0; i<4; i++) {
//        if( degrees[i] == degree ){
//            CCLOG("일치");
//            break;
//            return degree;
//        }
//
//
//        if( degrees[i] - degree < minOffset ) index = i;
//    }
//
//    CCLOG(" 근사값 추출:%d,  %d", index, degrees[index]);
//    CCLOG("  " );
//    return degrees[index];
//}





//void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue ){
//
//}


//void HelloWorld::menuCloseCallback(Ref* pSender)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}
