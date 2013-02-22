//
//  AMFMessage.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-22.
//
//

#ifndef __CocosAmf__AMFMessage__
#define __CocosAmf__AMFMessage__

#include "AMFMessage.h"
#include "cocos2d.h"

namespace AMF {
    class AMFMessage : public cocos2d::CCObject
    {
    public:
        cocos2d::CCDictionary* body;
    };
}

#endif /* defined(__CocosAmf__AMFMessage__) */
