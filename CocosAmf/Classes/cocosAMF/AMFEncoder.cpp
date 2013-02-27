//
//  AMFEncoder.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-27.
//
//

#include "AMFEncoder.h"

using namespace std;
US_ALBIN_AMF;

AMFEncoder::AMFEncoder()
{
    m_position = 0;
    m_bytes = *new vector<char>();
}

void AMFEncoder::operator+(bool v)
{
    uint8_t d = v ? 1 : 0;
    *this + d;
}

void AMFEncoder::operator+(uint8_t v)
{
    _apb(v, m_position++);
}

void AMFEncoder::operator+(int8_t v)
{
    _apb(v, m_position++);
}

void AMFEncoder::operator+(double v)
{
    _before(&v, sizeof(double));
}

void AMFEncoder::operator+(float v)
{
    _before(&v, sizeof(float));
}

void AMFEncoder::operator+(int32_t v)
{
    _before(&v, sizeof(int32_t));
}

void AMFEncoder::operator+(uint16_t v)
{
    _apb(((v >> 8) & 0xFF), m_position++);
	_apb((v & 0xFF), m_position++);
}

void AMFEncoder::_apply(const void* bytes, uint32_t len)
{
    uint8_t *chars = (uint8_t*)bytes;
    for (uint32_t i = 0; i < len; i++)
    {
        _apb(chars[i], m_position++);
    }
}

void AMFEncoder::_before(const void *bytes, uint32_t len)
{
    uint8_t *chars = (uint8_t*)bytes;
    for (int i = len - 1; i >= 0; i--)
    {
        _apb(chars[i], m_position++);
    }
}

void AMFEncoder::_apb(uint8_t v, uint32_t p)
{
    if(p < m_bytes.size())
    {
        m_bytes[p] = v;
    }
    else
    {
        m_bytes.push_back(v);
    }
}

void AMFEncoder::operator+(std::string &v)
{
    if(&v == NULL)
    {
        uint16_t d = 0;
        *this + d;
    }
    
    uint16_t len = v.length();
    *this + len;
    _apply(v.c_str(), len);
}

void AMFEncoder::operator*(int32_t v)
{
    v &= 0x1fffffff;
    if(v < 0x80)
    {
        _apb(v, m_position++);
    }
    else if (v < 0x4000)
    {
        _apb(((v >> 7) & 0x7F) | 0x80, m_position++);
        _apb(v &0x7f, m_position++);
    }
    else if (v < 0x200000)
    {
        _apb(((v >> 14) & 0x7F) | 0x80, m_position++);
		_apb(((v >> 7) & 0x7F) | 0x80, m_position++);
		_apb((v & 0x7F), m_position++);
    }
    else
    {
        _apb(((v >> 22) & 0x7F) | 0x80, m_position++);
		_apb(((v >> 15) & 0x7F) | 0x80, m_position++);
		_apb(((v >> 8) & 0x7F) | 0x80, m_position++);
		_apb((v & 0xFF), m_position++);
    }
}

void AMFEncoder::operator+(uint32_t v)
{
    _before(&v, sizeof(uint32_t));
}


// virtual protected
void AMFEncoder::_encode(ALBObject &data)
{
    switch (data.getDataType())
    {
        case kAMF3ArrayType:
            _encodeArray(data);
            break;
            
        case kAMF3NullType:
            _encodeNull();
            break;
            
        case kAMF3StringType:
            _encodeString(data);
            
        case kAMF3ObjectType:
            _encodeCustomObject(data);
            
        default:
            break;
    }
}

void AMFEncoder::_encodeCustomObject(AMF::ALBObject &data)
{
    
}

void AMFEncoder::_encodeNull()
{
    
}

void AMFEncoder::_encodeArray(std::vector<ALBObject *> data)
{
    
}

void AMFEncoder::_encodeNumber(double data)
{
    
}

void AMFEncoder::_encodeString(std::string data)
{
    
}

void AMFEncoder::_encodeDictionary(std::map<std::string, ALBObject *> data)
{
    
}