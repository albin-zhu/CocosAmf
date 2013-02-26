//
//  AMF3TraitsInfo.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-22.
//
//

#include "AMF3TraitsInfo.h"

USING_NS_CC;
US_ALBIN_AMF;
using namespace std;

AMF3TraitsInfo::AMF3TraitsInfo()
{
    properties = new vector<string>();
    dynamic = false;
    externalizable = false;
}

AMF3TraitsInfo::AMF3TraitsInfo(string& c, bool e, bool d, int count)
{
    className = c;
    properties = new vector<string>();
    dynamic = d;
    externalizable = e;
    count = count;
}

AMF3TraitsInfo::~AMF3TraitsInfo()
{
    delete properties;
    properties = NULL;
}

void AMF3TraitsInfo::addProperty(string& p)
{
    properties->push_back(p);
}

bool AMF3TraitsInfo::isEqual(const AMF3TraitsInfo* pObject)
{

    AMF3TraitsInfo *info = (AMF3TraitsInfo*) pObject;
    bool classNameIdentical = className.empty()
    ? info->className.empty()
    : info->className.compare(className);
    
    bool propertiesIdentical = properties->empty() ^ info->properties->empty();
    
    if (propertiesIdentical) {
        if(properties->capacity() > 0 && properties->capacity() == info->properties->capacity())
        {
            for(uint32_t i = 0; i < properties->capacity(); i++)
            {
                string &p = (*properties)[i];
                int x = p.compare((*info->properties)[i]);
                if(x != 0)
                {
                    propertiesIdentical = false;
                    break;
                }
            }
        }
        else
        {
            return false;
        }
    }
    
    if(classNameIdentical && propertiesIdentical && info->dynamic == dynamic && externalizable == info->externalizable)
    {
        return true;
    }
    return false;
}
/*
std::string& AMF3TraitsInfo::toString()
{
    return CCString::createWithFormat("<%s = 0x%08lx | className: %s | dynamic: %d | externalizable: %d | count: %d", this->m_uID, className, dynamic, externalizable, count);
}*/