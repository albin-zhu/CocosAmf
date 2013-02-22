//
//  BinaryUtil.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-21.
//
//

#include "BinaryUtil.h"

using namespace std;
using namespace cocos2d;

BinaryUtil::BinaryUtil(vector<char>& stream)
{
    this->m_amf3Stream = stream;
    m_position = 0;
    amfVersion = this->readShort() == kAMF0 ? kAMF0 : kAMF3;
}

BinaryUtil::BinaryUtil(std::vector<char> &stream, u_int32_t pos, AMFVERSION ver)
{
    this->m_amf3Stream = stream;
    m_position = pos;
    amfVersion = ver;
}

BinaryUtil::~BinaryUtil()
{
    this->m_amf3Stream.clear();
}

bool BinaryUtil::ensureLength(int l)
{
    CCAssert(m_position + l < m_amf3Stream.capacity(), "读到amfStream的尾部了！！");
    return true;
}

bool BinaryUtil::readBoolean()
{
    return (this->readUChar() != 0);
}

float BinaryUtil::readFloat()
{
    ensureLength(4);
    uint8_t data[4];
	data[3] = m_amf3Stream[m_position++];
	data[2] = m_amf3Stream[m_position++];
	data[1] = m_amf3Stream[m_position++];
	data[0] = m_amf3Stream[m_position++];
	return *((float *)data);
}

double BinaryUtil::readDouble()
{
    ensureLength(8);
    uint8_t data[8];
	data[7] = m_amf3Stream[m_position++];
	data[6] = m_amf3Stream[m_position++];
	data[5] = m_amf3Stream[m_position++];
	data[4] = m_amf3Stream[m_position++];
	data[3] = m_amf3Stream[m_position++];
	data[2] = m_amf3Stream[m_position++];
	data[1] = m_amf3Stream[m_position++];
	data[0] = m_amf3Stream[m_position++];
	return *((double *)data);
}

int BinaryUtil::readInt()
{
    ensureLength(4);
    u_int8_t ch1, ch2, ch3, ch4;
    ch1 = m_amf3Stream[m_position++];
    ch2 = m_amf3Stream[m_position++];
    ch3 = m_amf3Stream[m_position++];
    ch4 = m_amf3Stream[m_position++];
    
    return (ch1 << 24) + (ch2 << 16) + (ch3 << 8) + ch4;
}

u_int32_t BinaryUtil::readUInt()
{
    ensureLength(4);
    u_int8_t ch1, ch2, ch3, ch4;
    ch1 = m_amf3Stream[m_position++];
    ch2 = m_amf3Stream[m_position++];
    ch3 = m_amf3Stream[m_position++];
    ch4 = m_amf3Stream[m_position++];
    
    return ((ch1 & 0xFF) << 24) | ((ch2 & 0xFF) << 16) | ((ch3 & 0xFF) << 8) | (ch4 & 0xFF);
}

int8_t BinaryUtil::readShort()
{
    ensureLength(2);
    u_int8_t ch1, ch2;
    ch1 = m_amf3Stream[m_position++];
    ch2 = m_amf3Stream[m_position++];
    return (ch1 << 8) + ch2;
}

u_int8_t BinaryUtil::readUChar()
{
    ensureLength(1);
    return m_amf3Stream[m_position++];
}

u_int16_t BinaryUtil::readUShort()
{
    ensureLength(2);
    u_int8_t ch1, ch2;
    ch1 = m_amf3Stream[m_position++];
    ch2 = m_amf3Stream[m_position++];
    return ((ch1 & 0xFF) << 8) | (ch2 & 0xFF);
}

u_int32_t BinaryUtil::readUInt29()
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

CCString* BinaryUtil::readUTF8()
{
    return this->readUTF8WithLen(this->readUShort());
}

CCString* BinaryUtil::readUTF8WithLen(u_int32_t len)
{
    if(len == 0)
        return CCString::create("");
    this->ensureLength(len);
    
    string str;
    for (u_int32_t i = 0 ; i < len; i++)
    {
        str += m_amf3Stream[m_position++];
    }
    
    return CCString::create(str);
}

// 接口

AMF::AMFMessage* BinaryUtil::decodeAmf()
{
    if(amfVersion == kAMF0)
        return decodeAmf0();
    else
        return decodeAmf3();
}

AMF::AMFMessage* BinaryUtil::decodeAmf3()
{
    return new AMF::AMFMessage();
}

AMF::AMFMessage* BinaryUtil::decodeAmf0()
{
    cout<<"AMFEncoding:"<<amfVersion<<endl;
    short headCount = this->readShort();
    cout<<"Heades count:"<<headCount<<endl;
    short bodyCount = this->readShort();
    cout<<"Bodys count:"<<bodyCount<<endl;
    
    CCString* target = this->readUTF8();
    cout<<"Target:"<<target->getCString()<<endl;
    CCString* resp = this->readUTF8();
    cout<<"Response:"<<resp->getCString()<<endl;
    
    cout<<int(this->readUInt())<<endl;
    
    _decodeAmf0();
    
    return new AMF::AMFMessage();
}

CCObject* BinaryUtil::_decodeAmf0()
{
    AMF0Type type = (AMF0Type)readUChar();
    
    cout<<"AMF0Type:"<<int(type)<<endl;
    
    return readObject(type);
}

CCObject* BinaryUtil::_decodeAmf3()
{
    AMF3Type type = (AMF3Type)readUChar();
    
    cout<<"AMF3Type:"<<int(type)<<endl;
    
    return readObject(type);
}

CCObject* BinaryUtil::readObject(AMF0Type type)
{
    CCObject* dict;
    
    switch (type)
    {
        case kAMF0AVMPlusObjectType:
        {
            CCLog("解类型为0x%x", type);
            
            dict = _decodeAmf3();
            
        }break;
            
        default:
            CCLog("类型为0x%x的咱还不会解", type);
            break;
    }
    
    
    return dict;
}

CCObject* BinaryUtil::readObject(AMF3Type type)
{
    CCObject* dict;
    
    switch (type)
    {
        case kAMF3ObjectType:
        {
            CCLog("0x%d is a object", type);
            uint32_t ref = this->readUInt29();
            if ((ref & 1) == 0){
                ref = (ref >> 1);
                CCLog("返回一个已经存在的");
                return m_objectTabel->objectAtIndex(ref);
            }
        }break;
        default:
            CCLog("类型为0x%x的咱还不会解", type);
            break;
    }
    
    
    return dict;
}

CCObject* BinaryUtil::_findRef(u_int32_t ref)
{
    
}