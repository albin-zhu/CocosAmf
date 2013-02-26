//
//  AMFDecoder.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "AMFDecoder.h"
#include "AMF3Decoder.h"
#include "AMF0Decoder.h"

US_ALBIN_AMF;
USING_NS_CC;

AMFDecoder* AMFDecoder::getDecoder(std::vector<char> stream)
{
    u_int8_t ch1, ch2;
    ch1 = stream[0];
    ch2 = stream[1];
    AMFVERSION encoding = (AMFVERSION)((ch1 << 8) + ch2);
    if(encoding == kAMF0)
    {
        return new AMF0Decoder(stream);
    }
    else
    {
        return new AMF3Decoder(stream);
    }
}

AMFDecoder::AMFDecoder(std::vector<char> stream, uint32_t pos, AMFVERSION encoding):AMFStream(stream, pos, encoding)
{
    m_currentDeserializedObject = NULL;
    m_objectTable = new CCArray();
    m_objectTable->retain();
}

AMFDecoder::~AMFDecoder()
{
    AMFStream::~AMFStream();
    m_objectTable->release();
}

CCObject* AMFDecoder::decodeObject()
{
    return NULL;
}

CCObject* AMFDecoder::beginDecode()
{
    return NULL;
}

double AMFDecoder::_decodeNumberForKey(cocos2d::CCString* key)
{
    CCString* obj = (CCString*)m_currentDeserializedObject->properties->objectForKey(key->getCString());
    return obj->doubleValue();
}

bool AMFDecoder::decodeBoolForKey(cocos2d::CCString *key)
{
    double num = this->_decodeNumberForKey(key);
    
    return num;
}

double AMFDecoder::decodeDoubleForKey(cocos2d::CCString *key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

float AMFDecoder::decodeFloatForKey(cocos2d::CCString *key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

int32_t AMFDecoder::decodeInt32ForKey(cocos2d::CCString *key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

int64_t AMFDecoder::decodeInt64ForKey(cocos2d::CCString *key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

int AMFDecoder::decodeIntForKey(cocos2d::CCString *key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

CCObject* AMFDecoder::decodeObjectForKey(cocos2d::CCString *key)
{
    return ((ASObject*)m_currentDeserializedObject)->properties->objectForKey(key->getCString());
}

CCString* AMFDecoder::decodeMultiBytesString(uint32_t len, AMF::AMFVERSION encoding)
{
    return NULL;
}

void AMFDecoder::_cannotDecodeType(const char *type)
{
    printf("咱还不会解析类型为%s的数据", type);
}

CCObject* AMFDecoder::_objAt(uint32_t index)
{
    if (m_objectTable->count() <= index)
    {
        char* s = new char[30];
        sprintf(s, "CCObject* AMFDecoder::_objAt(uint32_t index) index = %d", index);
        throw s;
    }
    return m_objectTable->objectAtIndex(index);
}