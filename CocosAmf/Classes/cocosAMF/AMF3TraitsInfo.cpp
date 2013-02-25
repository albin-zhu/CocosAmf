//
//  AMF3TraitsInfo.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-22.
//
//

#include "AMF3TraitsInfo.h"

using namespace cocos2d;

AMF3TraitsInfo::AMF3TraitsInfo()
{
    properties = CCArray::create();
    dynamic = false;
    externalizable = false;
}

AMF3TraitsInfo::AMF3TraitsInfo(CCString* clazName, bool e, bool d, int count)
{
    properties = CCArray::create();
    dynamic = d;
    externalizable = e;
    count = count;
}

AMF3TraitsInfo::~AMF3TraitsInfo()
{
    properties->release();
}

void AMF3TraitsInfo::addProperty(cocos2d::CCString *p)
{
    properties->addObject(p);
}

bool AMF3TraitsInfo::isEqual(const cocos2d::CCObject *pObject)
{
    return pObject->m_uID == this->m_uID;
    
    AMF3TraitsInfo *info = (AMF3TraitsInfo*) pObject;
    bool classNameIdentical = className == NULL
    ? info->className == NULL
    : info->className->isEqual(className);
    
    bool propertiesIdentical = properties == NULL
    ? info->properties == NULL
    : info->properties->isEqual(properties);
    
    if(classNameIdentical && propertiesIdentical && info->dynamic == dynamic && externalizable == info->externalizable)
    {
        return true;
    }
    return false;
}

CCString* AMF3TraitsInfo::toString()
{
    return CCString::createWithFormat("<%s = 0x%08lx | className: %s | dynamic: %d | externalizable: %d | count: %d", this->m_uID, className, dynamic, externalizable, count);
}