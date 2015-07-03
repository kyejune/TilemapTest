//
//  DebugMap.cpp
//  TilemapTest
//
//  Created by user on 6/25/15.
//
//

#include "DebugMap.h"
#include "D.h"

USING_NS_CC;


bool DebugMap::init()
{

    if( DrawNode::init() ){
        
//        ways.push_back( *new std::vector<Vec2> );
        
        return true;
    }else{
        return false;
    }
}

void DebugMap::parseMap( cocos2d::TMXTiledMap* tmap  ){
    
    TMXObjectGroup* wall = tmap->getObjectGroup("linewall");
    
    Size mapSize  = tmap->getContentSize();
    tilesLen = tmap->getMapSize();
    tilesSize= tmap->getTileSize()/2;
    
    
    CCLOG("parseMap %f %f", D::mapSize.width, D::mapSize.height );
    
    for( int i=0; i<tilesLen.height+1; i++ ) this->drawLine( Vec2( 0, i*(tilesSize.height)), Vec2( mapSize.width, i*(tilesSize.height) ), Color4F::BLUE );
    for( int j=0; j<tilesLen.width+1; j++ )  this->drawLine( Vec2( j*(tilesSize.width), 0 ), Vec2( j*(tilesSize.width), mapSize.height ), Color4F::BLUE );

    
//    CCLOG(" Line... %f, %f", tmap->getMapSize().width, tmap->getMapSize().height ); // 갯수
//    CCLOG(" Line... %f, %f", tmap->getTileSize().width, tmap->getTileSize().height ); // 크기
    
    DrawNode* wallNode = DrawNode::create();
    this->addChild( wallNode );
    
    int lineCount = 0;
    for( auto obj : wall->getObjects())
    {
        bool first = true;
        Vec2 prevPoint;
        
        
        auto xx = obj.asValueMap()["x"].asFloat();
        auto yy = obj.asValueMap()["y"].asFloat();
        
        // 벽 생성 및 위치
        for( auto value : obj.asValueMap() )
        {
            if( value.first == "polylinePoints"){
                wallSeg.push_back( *new std::vector<Vec2> );
                auto vec = value.second.asValueVector();
                
                CCLOG("catch Line %d", lineCount );
                first = true;
                for( auto &p : vec ){
                    Vec2 point = Vec2( ( p.asValueMap().at("x").asFloat()*.5 ) + xx, -( p.asValueMap().at("y").asFloat()*.5 ) + yy );
                    
                    if( first == false ){
                        wallNode->drawSegment( prevPoint, point, 3.0f, Color4F::WHITE );
                    }
                    
                    wallSeg[lineCount].push_back(point);
                    prevPoint = point;
                    first = false;
                }//for
                
                CCLOG( "(%d) line length is %lu", lineCount, wallSeg[lineCount].size());
                lineCount += 1;
            }//if
            
        }//for
    }
    
    
    parentMap.reserve(100);
    getFastDistance( 8,0, 6,8 );
}

D::Load DebugMap::checkLoad( int startCoordX, int startCoordY, int endCoordX, int endCoordY ){
    
    if( endCoordX < 0 || endCoordY < 0 || endCoordX > tilesLen.width || endCoordY > tilesLen.height ) return D::Load::BLOCKED;
    
    Vec2 loadStart = Vec2( startCoordX*tilesSize.width + tilesSize.width/2, startCoordY*tilesSize.height + tilesSize.height/2 );
    Vec2 loadEnd   = Vec2( endCoordX*tilesSize.width + tilesSize.width/2, endCoordY*tilesSize.height + tilesSize.height/2 );
    
    long wallLen = wallSeg.size();
    for( int i=0; i<wallLen; i++ ){
        for( int j=0; j<wallSeg[i].size()-1; j++ ){
            Vec2 SegS = wallSeg[i][j];
            Vec2 SegE = wallSeg[i][j+1];
            bool blocked = Vec2::isSegmentIntersect( loadStart, loadEnd, SegS, SegE );
            if( blocked ) return D::Load::BLOCKED;
        }
    }
    
    return D::Load::PASS;
}

int DebugMap::getFastDistance( int startCoordX, int startCoordY, int endCoordX, int endCoordY ){
    start.x = startCoordX;
    start.y = startCoordY;
    goal.x = endCoordX;
    goal.y = endCoordY;
    
    // 클리어
    closedCoords.clear();
    
    
    // 시작점 기록 및 탐색 시작
    Vec2 start = Vec2( startCoordX, startCoordY );
    bool result = findNext( start );
    
    
    long l = parentMap.size();
    CCLOG("맵 사이즈는 %lu", l );
    
//    if( result ){
//        int dis = accumulateDistance( StringUtils::format("%dx%d", int(goal.x), int(goal.y)), -1 );
//        CCLOG( "길찾았응 %d", dis );
//        return dis;
//    }else{
//        CCLOG( "못찾겠어" );
//        return -1;
//    }
    
    return 0;
}

int DebugMap::accumulateDistance( std::string xy, int accumulatedDistance ){
//    CCLOG("ACC");
//    if( xy == StringUtils::format("%dx%d", int(start.x), int(start.y)).c_str() ) return accumulatedDistance + 1;
//    else                                                                         return accumulateDistance(parentMap[xy], accumulatedDistance + 1 );
}

std::vector<Vec2> DebugMap::getOpenTiles( Vec2 coord ){
    std::vector<Vec2> result;
    
    // 검색결과 저장 및 재검색때 사용하는 코드 추가
    
    // 중심을 기준으로 4방향중 이동가능한 타일 좌표를 추출
    Vec2 t = Vec2( coord.x, coord.y+1 );
    Vec2 r = Vec2( coord.x+1, coord.y );
    Vec2 b = Vec2( coord.x, coord.y-1 );
    Vec2 l = Vec2( coord.x-1, coord.y );
    
    if( !isMarkedCoord( t ) &&  checkLoad( coord.x, coord.y, t.x, t.y ) == D::PASS ) result.push_back( t );
    if( !isMarkedCoord( r ) &&  checkLoad( coord.x, coord.y, r.x, r.y ) == D::PASS ) result.push_back( r );
    if( !isMarkedCoord( b ) &&  checkLoad( coord.x, coord.y, b.x, b.y ) == D::PASS ) result.push_back( b );
    if( !isMarkedCoord( l ) &&  checkLoad( coord.x, coord.y, l.x, l.y ) == D::PASS ) result.push_back( l );
    
    return result;
}


bool DebugMap::findNext( Vec2 coord ){
    CCLOG("F");
    closedCoords.push_back( coord );
    // 진행가능한 좌표 추출
    std::vector<Vec2> nextEntries = getOpenTiles( coord );
    
    // 추가되는 길마다 번호를 지정해서 각각 기록
    if( nextEntries.size() == 0 ){
//        CCLOG(" === %d번길 막다른길 도착 ", wayNo );
        return false;
    }
//    else if( nextEntries.size() > 1 ){
//         CCLOG("-------- 전방에 갈림길 %d개 등장!!", int(nextEntries.size()) );
//        
//    }
    
    for( int i=0; i<nextEntries.size(); i++ ){
//        parentMap[ StringUtils::format("%dx%d", int(nextEntries[i].x), int(nextEntries[i].y) ).c_str() ] = StringUtils::format( "%dx%d", int(coord.x), int(coord.y) ).c_str();
        
        Vec2 n = nextEntries[i];
        
        if( nextEntries.size() > 1 ){
//            CCLOG("%lu길의 부모는 %d길", ways.size(), wayNo );
            
//           parentMap[ StringUtils::format("%dx%d", int(nextEntries[i].x), int(nextEntries[i].y) ).c_str() ] = coord;
//            CCLOG("dksljfksjdflk;sjdfk;lsj------ %s", StringUtils::format("%dx%d", int(nextEntries[i].x), int(nextEntries[i].y) ).c_str() );
//            parentMap.insert( std::string(StringUtils::format("%fx%f", coord.x, coord.y )), wayNo );
            
//            parentWay.push_back( wayNo );
//            ways.push_back( *new std::vector<Vec2> );
        }
        
        // 기록..
//        ways[ways.size()-1].push_back( nextEntries[i] );
//        parentMap[nextEntries[i].x + "x" + nextEntries[i].y ] = coord;
        parentMap[int(n.x + n.y*tilesLen.width)] = coord;
        
        
//        CCLOG( "    (%lu)길 기록 %d x %d", ways.size()-1, int(nextEntries[i].x), int(nextEntries[i].y) );
        
        // 목표에 도착하면 종료
        if( nextEntries[i].equals( goal ) == true ){
//            successWay.push_back( int(ways.size())-1 );
            return true;
        }
        else // 아니면 계속 검색
        {
            if( findNext( nextEntries[i] ) ) return true;
        }
    }
    
    return false;
}


// 외길일때는 문제 없는데 아니면 course 번호별로 검색하는 루틴 추가
bool DebugMap::isMarkedCoord( Vec2 coord ){
    for( int i=0; i<closedCoords.size(); i++ ){
        if( closedCoords[i].equals( coord ) ){
            return true;
        }
    }
    return false;
}
