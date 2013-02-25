//
//  AMFStream.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "AMFStream.h"

using namespace AMF;
using namespace std;

AMFStream::AMFStream(vector<char>& stream, uint32_t pos, AMFVERSION encoding)
{
    m_amfStream = stream;
    m_position = pos;
    m_encoding = encoding;
//    m_stringTable = vector<string>();
}

AMFStream::~AMFStream()
{
    if(&m_amfStream)
    {
        m_amfStream.clear();
    }
    
//    m_stringTable.clear();
}


bool AMFStream::readBoolean()
{
    return (this->readUChar() != 0);
}

float AMFStream::readFloat()
{
    ensureLength(4);
    uint8_t data[4];
	data[3] = m_amfStream[m_position++];
	data[2] = m_amfStream[m_position++];
	data[1] = m_amfStream[m_position++];
	data[0] = m_amfStream[m_position++];
	return *((float *)data);
}

double AMFStream::readDouble()
{
    ensureLength(8);
    uint8_t data[8];
	data[7] = m_amfStream[m_position++];
	data[6] = m_amfStream[m_position++];
	data[5] = m_amfStream[m_position++];
	data[4] = m_amfStream[m_position++];
	data[3] = m_amfStream[m_position++];
	data[2] = m_amfStream[m_position++];
	data[1] = m_amfStream[m_position++];
	data[0] = m_amfStream[m_position++];
	return *((double *)data);
}

int AMFStream::readInt()
{
    ensureLength(4);
    u_int8_t ch1, ch2, ch3, ch4;
    ch1 = m_amfStream[m_position++];
    ch2 = m_amfStream[m_position++];
    ch3 = m_amfStream[m_position++];
    ch4 = m_amfStream[m_position++];
    
    return (ch1 << 24) + (ch2 << 16) + (ch3 << 8) + ch4;
}

u_int32_t AMFStream::readUInt()
{
    ensureLength(4);
    u_int8_t ch1, ch2, ch3, ch4;
    ch1 = m_amfStream[m_position++];
    ch2 = m_amfStream[m_position++];
    ch3 = m_amfStream[m_position++];
    ch4 = m_amfStream[m_position++];
    
    return ((ch1 & 0xFF) << 24) | ((ch2 & 0xFF) << 16) | ((ch3 & 0xFF) << 8) | (ch4 & 0xFF);
}

int8_t AMFStream::readShort()
{
    ensureLength(2);
    u_int8_t ch1, ch2;
    ch1 = m_amfStream[m_position++];
    ch2 = m_amfStream[m_position++];
    return (ch1 << 8) + ch2;
}

u_int8_t AMFStream::readUChar()
{
    ensureLength(1);
    u_int8_t c = m_amfStream[m_position++];
    CCLOG("%d",c);
    return c;
}

u_int16_t AMFStream::readUShort()
{
    ensureLength(2);
    u_int8_t ch1, ch2;
    ch1 = m_amfStream[m_position++];
    ch2 = m_amfStream[m_position++];
    return ((ch1 & 0xFF) << 8) | (ch2 & 0xFF);
}

u_int32_t AMFStream::readUInt29()
{
    uint32_t value;
	uint8_t ch = this->readUChar() & 0xFF;
	
	if (ch < 128){
		return ch;
	}
	
	value = (ch & 0x7F) << 7;
	ch = this->readUChar() & 0xFF;
	if (ch < 128){
		return value | ch;
	}
	
	value = (value | (ch & 0x7F)) << 7;
	ch = this->readUChar() & 0xFF;
	if (ch < 128){
		return value | ch;
	}
	
	value = (value | (ch & 0x7F)) << 8;
	ch = this->readUChar() & 0xFF;
	return value | ch;
}

cocos2d::CCString* AMFStream::readUTF()
{
//    if(m_encoding == kAMF0)
//        return this->readUTF(this->readUShort());
//    uint32_t ref = this->readUInt29();
//	if ((ref & 1) == 0){
//		ref = (ref >> 1);
//        m_stringTable[ref];
//	}
//	uint32_t length = ref >> 1;
//	if (length == 0){
//		return string("");
//	}
//	string value = this->readUTF(length);
//	m_stringTable.push_back(value);
//	return value;
    
    return NULL;
}

cocos2d::CCString* AMFStream::readUTF(u_int32_t len)
{
    if(len == 0)
        return NULL;
    this->ensureLength(len);
    
    string str;
    for (u_int32_t i = 0 ; i < len; i++)
    {
        str += m_amfStream[m_position++];
    }
    
    return cocos2d::CCString::create(str);
}
