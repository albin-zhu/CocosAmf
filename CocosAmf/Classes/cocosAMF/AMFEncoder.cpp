//
//  AMFEncoder.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-27.
//
//

#include "AMFEncoder.h"
#include <sstream>

using namespace std;
US_ALBIN_AMF;

AMFEncoder::AMFEncoder()
{
//    m_position = 0;
//    m_bytes.clear();
    stringArr.clear();
    objects = new ALBObject();
    traits.clear();
    byte_arrays.clear();
}

void ByteBuffer::writeBoll(bool v)
{
    uint8_t d = v ? 1 : 0;
    this->writeByte(d);
}

void ByteBuffer::writeByte(uint8_t v)
{
    _apb(v, m_position++);
}

void ByteBuffer::writeChar(int8_t v)
{
    _apb(v, m_position++);
}

void ByteBuffer::writeDouble(double v)
{
    _before(&v, sizeof(double));
}

void ByteBuffer::writeFloat(float v)
{
    _before(&v, sizeof(float));
}

void ByteBuffer::writeInt(int32_t v)
{
    _before(&v, sizeof(int32_t));
}

void ByteBuffer::writeUShort(uint16_t v)
{
    _apb(((v >> 8) & 0xFF), m_position++);
	_apb((v & 0xFF), m_position++);
}

void ByteBuffer::_apply(const void* bytes, uint32_t len)
{
    uint8_t *chars = (uint8_t*)bytes;
    for (uint32_t i = 0; i < len; i++)
    {
        _apb(chars[i], m_position++);
    }
}

void ByteBuffer::_before(const void *bytes, uint32_t len)
{
    uint8_t *chars = (uint8_t*)bytes;
    for (int i = len - 1; i >= 0; i--)
    {
        _apb(chars[i], m_position++);
    }
}

void ByteBuffer::_apb(char v, uint32_t p)
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

void ByteBuffer::writeString(std::string v)
{
//    if(&v == NULL)
//    {
//        uint16_t d = 0;
//        this->writeUShort(d);
//    }
//    
//    uint16_t len = v.length();
//    this->writeUShort(len);
    _apply(v.c_str(), v.length());
}

void ByteBuffer::writeU29(int32_t value)
{
    value &= 0x1fffffff;
	if (value < 0x80)
    {
		_apb(value, m_position++);
	}
    else if (value < 0x4000)
    {
		
		_apb(((value >> 7) & 0x7F) | 0x80, m_position++);
		_apb((value & 0x7F), m_position++);
	}
    else if (value < 0x200000)
    {
		_apb(((value >> 14) & 0x7F) | 0x80, m_position++);
		_apb(((value >> 7) & 0x7F) | 0x80, m_position++);
		_apb((value & 0x7F), m_position++);
	}
    else
    {
		_apb(((value >> 22) & 0x7F) | 0x80, m_position++);
		_apb(((value >> 15) & 0x7F) | 0x80, m_position++);
		_apb(((value >> 8) & 0x7F) | 0x80, m_position++);
		_apb((value & 0xFF), m_position++);
	}
}

void ByteBuffer::writeUint(uint32_t v)
{
    _before(&v, sizeof(uint32_t));
}

void AMFEncoder::_encode(AMF::ALBObject &source)
{
    switch (source.getDataType())
    {
        case kAMF3UndefinedType:
            _encodeUndefined();
            break;
            
        case kAMF3NullType:
            _encodeNull();
            break;
        
        case kAMF3FalseType:
            _encodeFalse();
            break;
        
        case kAMF3TrueType:
            _encodeTrue();
            break;
            
        case kAMF3ObjectType:
            _encodeObject(source);
            break;
        
        case kAMF3ArrayType:
            _encodeArray(source);
            break;
            
        case kAMF3IntegerType:
//            _encodeInteger(source);
            m_bytes.writeByte(kAMF3IntegerType);
            m_bytes.writeU29(source);
            break;
            
        case kAMF3DoubleType:
            _encodeDouble(source);
            break;
        case kAMF3StringType:
            _encodeString(source, false);
            break;
            
        default:
            printf("the type 0x%02x is not support", source.getDataType());
            break;
    }
}

void AMFEncoder::_encodeUndefined()
{
    m_bytes.writeByte(kAMF3UndefinedType);
}

void AMFEncoder::_encodeNull()
{
    m_bytes.writeByte(kAMF3NullType);
}

void AMFEncoder::_encodeFalse()
{
    m_bytes.writeByte(kAMF3FalseType);
}

void AMFEncoder::_encodeTrue()
{
    m_bytes.writeByte(kAMF3TrueType);
}

void AMFEncoder::_encodeInteger(AMF::ALBObject &source)
{
    m_bytes.writeByte(kAMF3IntegerType);
    m_bytes.writeInt(source);
}

void AMFEncoder::_encodeDouble(AMF::ALBObject &source)
{
    m_bytes.writeByte(kAMF3DoubleType);
    m_bytes.writeDouble(source);
}

void AMFEncoder::_encodeString(string &source, bool writeType)
{
    if (!writeType)
    {
        m_bytes.writeByte(kAMF3StringType);
    }
    
    if (&source == NULL || source.length() == 0){
		m_bytes.writeByte(((0 << 1) | 1));
		return;
	}
    uint32_t size = stringArr.size();
    
    for (uint32_t i = 0; i < size; i++)
    {
        if (stringArr[i] == source || stringArr[i].compare(source.c_str()) == 0)
        {
            m_bytes.writeU29( i << 1);
            return;
        }
    }
    stringArr.push_back(source);
    
    uint16_t len = (uint16_t)source.length();
    m_bytes.writeU29(((len << 1) | 0x01));
    m_bytes.writeString(source);
}

void AMFEncoder::_encodeArray(AMF::ALBObject &source)
{
    m_bytes.writeByte(kAMF3ArrayType);
    int32_t index = objects->indexOf(source);
    if (index >= 0)
    {
        m_bytes.writeU29((index << 1));
        return;
    }
    objects->push(source);
    uint32_t size = source.getCount();
    m_bytes.writeU29((size << 1) | 1);
    m_bytes.writeByte((0 << 1) | 1);
    
    for (uint32_t i = 0; i < size; i++)
    {
        _encode(source[i]);
    }
//    // 1. write array type 0x09
//    m_bytes.writeByte(kAMF3ArrayType);
//    
//    // 2. write array length
//    uint32_t size = source.getData()._array->size();
//    m_bytes.writeUint(((size << 1) | 0x01));
//    
//    // 3. num->value
//    stringstream ss;
//    string key;
//    for (uint32_t i = 0; i < size; i++)
//    {
//        ss<<i;
//        key = ss.str();
//        _encodeString(key);
//        _encode(source[i]);
//    }
//    ss.clear();
//    key.clear();
//    
//    // 4. write empty
//    string empty_key("");
//    _encodeString(empty_key, false);
//    
//    // 5. store the dense portion (see this as the indexed array part)
//    for (uint32_t i = 0; i < size; i++)
//    {
//        _encode(source[i]);
//    }
}

void AMFEncoder::_encodeObject(AMF::ALBObject &source)
{
    m_bytes.writeByte(kAMF3ObjectType);
    
    int32_t index = objects->indexOf(source);
    if(index >= 0)
    {
        m_bytes.writeU29((index << 1));
        return;
    }
    
    objects->push(source);
    AMF3TraitsInfo* traits = new AMF3TraitsInfo();
    traits->externalizable = source.externalizable;
    traits->dynamic = source.type.empty();
    traits->count = (traits->dynamic || traits->externalizable ? 0 : source.getCount());
    traits->className = source.type;
    vector<string>* keys = source.allKeys();
    traits->properties = (traits->dynamic ? NULL : keys);
    _encodeTraits(*traits);
    
    if (source.externalizable)
    {
        for (uint32_t i = 0; i < source.getCount(); i++) {
            _encode(source[i]);
        }
    }
    
    vector<string>::iterator it = keys->begin();
    vector<string>::iterator it_end = keys->end();
    while (it != it_end)
    {
        if (traits->dynamic)
        {
            _encodeString(*it, true);
        }
        
        _encode(source[*it]);
        ++it;
    }
    if (traits->dynamic)
    {
        m_bytes.writeU29(((0 << 1) | 1));
    }

//    
//    uint32_t traits_count = 0x0b;
//    if(!m_bytes.writeU29(traits_count))
//    {
//        printf("AMFEncoder: write _encode AMF3 Object, cannot write traits count");
//		return;
//    }
//    
//    string class_name = "";
//    _encodeString(class_name, false);
//    map<string, ALBObject*> &dict = *source.getData()._properties;
//    map<string, ALBObject*>::iterator it = dict.begin();
//    map<string, ALBObject*>::iterator it_end = dict.end();
//    
//    while (it != it_end)
//    {
//        string keyname = it->first;
//        _encodeString(keyname);
//        _encode(*it->second);
//        ++it;
//    }
//    
//    _encodeString(class_name, false);
}

void AMFEncoder::_encodeTraits(AMF::AMF3TraitsInfo &t)
{
    vector<AMF3TraitsInfo*>::iterator it = traits.begin();
    vector<AMF3TraitsInfo*>::iterator it_end = traits.end();
    int index = 0;
    while (it != it_end)
    {
        if (*it == &t)
        {
            m_bytes.writeU29((index << 2) | 1);
            return;
        }
        ++index;
        ++it;
    }
    
    traits.push_back(&t);
    
    uint32_t infoBits = 3;
    if (t.externalizable) infoBits |= 4;
    if (t.dynamic) infoBits |= 8;
    infoBits |= (t.count << 4);
    m_bytes.writeU29(infoBits);
    _encodeString(t.className, true);
    
}

void AMFEncoder::_encodeByteArray()
{
    
}

// AMF0
void AMFEncoder::_encodeString0(std::string str)
{
    if (str.empty()){
        m_bytes.writeUShort(0);
		return;
	}
    m_bytes.writeUShort(str.length());
    m_bytes.writeString(str);
}

void ByteBuffer::writeBuf(const char *buf, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        m_bytes.push_back(buf[i]);
    }
}

const char* AMFEncoder::encode(AMF::AMFActionMessage &message)
{
    m_bytes.writeUShort(message.getVersion());
    uint16_t header_count = message.getHeaders().size();
    m_bytes.writeUShort(header_count);
    if (header_count > 0) {
        printf("AMFEncoder:: @todo encode headers");
    }
    
    uint16_t message_count = message.getMessageCount();
    m_bytes.writeUShort(message_count);
    for (uint16_t i = 0; i < message_count; i++)
    {
        AMFMessageBody *obj = message.getBodies()[i];
        string target = obj->targetURI;
        _encodeString0(target);
        string resp = (obj->responseURI);
        _encodeString0(resp);
        
        
        ALBObject* data = obj->data;
        if(message.getVersion() == kAMF3)
        {
            AMFEncoder* amf3Encoder = new AMFEncoder();
            amf3Encoder->getBuf().writeByte(kAMF0AVMPlusObjectType);
            amf3Encoder->_encode(*data);
            ByteBuffer& tmp_buf = amf3Encoder->getBuf();
            m_bytes.writeUint(tmp_buf.getSize());
            m_bytes.writeBuf(tmp_buf.getbuf(), tmp_buf.getSize());
        }
        else
        {
            printf("AMF0 is in progressing......");
        }
    }
    return m_bytes.getbuf();
}