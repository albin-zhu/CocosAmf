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
    m_objectTable = new vector<ALBObject*>();
}

AMFDecoder::~AMFDecoder()
{
    AMFStream::~AMFStream();
    m_objectTable = NULL;
    delete m_objectTable;
}

ALBObject& AMFDecoder::decodeObject()
{
    ALBObject* t = NULL;
    return *t;
}

ALBObject& AMFDecoder::beginDecode()
{
    ALBObject* t = NULL;
    return *t;
}

ALBObject& AMFDecoder::_decodeNumberForKey(string& key)
{
    return *m_currentDeserializedObject;
}

bool AMFDecoder::decodeBoolForKey(string &key)
{
    double num = this->_decodeNumberForKey(key);
    
    return num;
}

double AMFDecoder::decodeDoubleForKey(string &key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

float AMFDecoder::decodeFloatForKey(string &key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

int32_t AMFDecoder::decodeInt32ForKey(string &key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

int64_t AMFDecoder::decodeInt64ForKey(string &key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

int AMFDecoder::decodeIntForKey(string &key)
{
    double num = this->_decodeNumberForKey(key);
    return num;
}

ALBObject& AMFDecoder::decodeObjectForKey(string &key)
{
    return (*m_currentDeserializedObject)[key];
}

string& AMFDecoder::decodeMultiBytesString(uint32_t len, AMF::AMFVERSION encoding)
{
    string* tmp = new string("key");
    return *tmp;
}

void AMFDecoder::_cannotDecodeType(const char *type)
{
    printf("咱还不会解析类型为%s的数据", type);
}

ALBObject& AMFDecoder::_objAt(uint32_t index)
{
    if (m_objectTable->capacity() <= index)
    {
        char* s = new char[30];
        sprintf(s, "ALBObject& AMFDecoder::_objAt(uint32_t index) index = %d", index);
        printf("\n\nERROR: %s\n",s);
        throw s;
    }
    return *((*m_objectTable)[index]);
}