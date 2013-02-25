//
//  ASObject.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#ifndef __CocosAmf__ASObject__
#define __CocosAmf__ASObject__

#include "AMF3.h"
ALBIN_AMF

class ASObject : public cocos2d::CCObject
{
public:
    ASObject();
    virtual ~ASObject();
    
    void addObject(ASObject*);
    void setObjectForKey(cocos2d::CCString* key, CCObject* value);
    void setType(cocos2d::CCString*);
    
public:
    cocos2d::CCString *type;
    bool externalizable;
    uint32_t count;
    cocos2d::CCArray* data;
    cocos2d::CCDictionary* properties;
    
    static ASObject* asObject(cocos2d::CCDictionary *dict);
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__ASObject__) */
