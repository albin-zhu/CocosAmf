//
//  ASObject.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "ASObject.h"

using namespace cocos2d;

ASObject::~ASObject()
{
    CCObject::~CCObject();
    type->release();
    CC_SAFE_DELETE(data);
    CC_SAFE_DELETE(properties);
}

ASObject::ASObject()
{
    
}

void ASObject::setType(cocos2d::CCString *clz)
{
    type = clz;
    type->retain();
}

void ASObject::setObjectForKey(cocos2d::CCString *key, cocos2d::CCObject *value)
{
    this->properties->setObject(value, key->getCString());
}