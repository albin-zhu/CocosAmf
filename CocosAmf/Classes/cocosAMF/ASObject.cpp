//
//  ASObject.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "ASObject.h"

USING_NS_CC;
US_ALBIN_AMF;

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

void ASObject::setObject(cocos2d::CCObject *value, cocos2d::CCString *key)
{
    this->properties->setObject(value, key->getCString());
}