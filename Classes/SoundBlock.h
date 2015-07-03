//
//  SoundBlock.h
//  TilemapTest
//
//  Created by user on 6/30/15.
//
//

#ifndef __TilemapTest__SoundBlock__
#define __TilemapTest__SoundBlock__

#include <stdio.h>

USING_NS_CC;

class SoundBlock: public Node
{    
public:
    SoundBlock(){};
    ~SoundBlock(){};
    CREATE_FUNC(SoundBlock);
    virtual bool init();
};



#endif /* defined(__TilemapTest__SoundBlock__) */
