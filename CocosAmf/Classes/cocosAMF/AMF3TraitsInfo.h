//
//  AMF3TraitsInfo.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-22.
//
//

#ifndef __CocosAmf__AMF3TraitsInfo__
#define __CocosAmf__AMF3TraitsInfo__

#include "AMF3.h"

ALBIN_AMF

class AMF3TraitsInfo : public cocos2d::CCObject
{
public:
    AMF3TraitsInfo();
    AMF3TraitsInfo(cocos2d::CCString*, bool, bool, int);
    ~AMF3TraitsInfo();
    
    cocos2d::CCString* className;
    bool dynamic;
    bool externalizable;
    int count;
    cocos2d::CCArray *properties;
    
    void addProperty(cocos2d::CCString* p);
    
    virtual bool isEqual(const CCObject* pObject);
    cocos2d::CCString* toString();
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMF3TraitsInfo__) */
