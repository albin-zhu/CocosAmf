//
//  AMF0Decoder.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "AMF0Decoder.h"
#include "AMF3Decoder.h"

US_ALBIN_AMF;
USING_NS_CC;
using namespace std;

AMF0Decoder::AMF0Decoder(std::vector<char>& stream):AMFDecoder(stream)
{
    
}

CCObject* AMF0Decoder::beginDecode()
{
    readShort();
    cout<<"AMFEncoding:"<<m_encoding<<endl;
    short headCount = this->readShort();
    cout<<"Heades count:"<<headCount<<endl;
    short bodyCount = this->readShort();
    cout<<"Bodys count:"<<bodyCount<<endl;
    
    CCString* target = this->readUTF();
    cout<<"Target:"<<target->getCString()<<endl;
    CCString* resp = this->readUTF();
    cout<<"Response:"<<resp->getCString()<<endl;
    
    cout<<int(this->readUInt())<<endl;
    
    return decodeObject();
}

CCObject* AMF0Decoder::decodeObject()
{
    AMF0Type type;
    readType(type);
    return (CCObject*)_decode(type);
}

void* AMF0Decoder::_decode(AMF0Type type)
{
    void *value = NULL;
	switch (type){
		case kAMF0NumberType:
        {
            double num = this->readDouble();
            value = &num;
        }break;
			
		case kAMF0BooleanType:
        {
            bool b = this->readBoolean();
			value = &b;
        }break;
			
		case kAMF0StringType:
			value = this->readUTF();
			break;
			
		case kAMF0AVMPlusObjectType:
        {
            AMF3Decoder *amf3Decoder = new AMF3Decoder(m_amfStream, m_position, kAMF3);
        
			value = amf3Decoder->decodeObject();
            m_position = amf3Decoder->getPos();
			amf3Decoder->~AMF3Decoder();
		}break;
		case kAMF0StrictArrayType:
			value = this->_decodeArray();
			break;
			
		case kAMF0TypedObjectType:
			value = this->_decodeTypedObject();
			break;
			
		case kAMF0LongStringType:
			value = this->_decodeLongString();
			break;
			
		case kAMF0ObjectType:
        {
			value = this->_decodeAsOBject(NULL);
        }break;
            
        case kAMF0XMLObjectType:
            value = this->_decodeXML();
            break;
        			
		case kAMF0NullType:
			value = NULL;
			break;
			
		case kAMF0DateType:
        {
			cc_timeval date = this->_decodeDate();
            value = &date;
			break;
        }
			
		case kAMF0ECMAArrayType:
			value = this->_decodeECMAArray();
			break;
			
		case kAMF0ReferenceType:
			value = this->_decodeReference();
			break;
			
		case kAMF0UndefinedType:
			value = NULL;
			break;
			
		case kAMF0UnsupportedType:
			_cannotDecodeType("Unsupported type");
			break;
			
		case kAMF0ObjectEndType:
			_cannotDecodeType("Unexpected object end");
			break;
			
		case kAMF0RecordsetType:
			_cannotDecodeType("Unexpected recordset");
			break;
			
		default:
			_cannotDecodeType("Unknown type");
	}
	return value;

}

CCArray* AMF0Decoder::_decodeArray()
{
    uint32_t size = this->readUInt();
    CCArray *array = CCArray::createWithCapacity(size);
    m_objectTable->addObject(array);
    for (uint32_t i = 0; i < size; i++)
    {
        CCObject *obj = this->decodeObject();
        if(obj != NULL)
        {
            array->addObject(obj);
        }
    }
    
    return array;
}

CCObject* AMF0Decoder::_decodeTypedObject()
{
    CCString *clazName = this->readUTF();
    return this->_decodeAsOBject(clazName);
}

CCObject* AMF0Decoder::_decodeAsOBject(cocos2d::CCString *clazName)
{
    ASObject *object = new ASObject();
    object->type = clazName;
    object->type->retain();
    m_objectTable->addObject(object);
    
    CCString *propertyName = this->readUTF();
    AMF0Type type;
    readType(type);
    while (type != kAMF0ObjectEndType)
    {
        object->setObjectForKey(propertyName, (CCObject*)this->_decode(type));
        propertyName = this->readUTF();
        this->readType(type);
    }
    
    return object;
}

CCString* AMF0Decoder::_decodeLongString()
{
    uint32_t length = this->readUInt();
    if(length == 0)
    {
        return NULL;
    }
    
    return this->readUTF(length);
}

CCObject* AMF0Decoder::_decodeXML()
{
    CCString *xmlString = this->_decodeLongString();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS // 呆会再做不同平台的 xml解析。。。算了，还是留给大侠们做吧
    return xmlString;
#else
    return xmlString;
#endif
    
}


cc_timeval AMF0Decoder::_decodeDate()
{
    double time = this->readDouble();
    // 时区
//    u_int16_t timeZonge = this->readUShort();
    cc_timeval t = {time / 1000, time};
    return t;
}

CCDictionary* AMF0Decoder::_decodeECMAArray()
{
    this->readUInt();
    CCDictionary *dict = CCDictionary::create();
    m_objectTable->addObject(dict);
    
    CCString *propertyName = this->readUTF();
    AMF0Type type;
    readType(type);
    while (type != kAMF0ObjectEndType)
    {
        dict->setObject((CCObject*)this->_decode(type), propertyName);
        propertyName = this->readUTF();
        readType(type);
    }
    
    return dict;
}

CCObject* AMF0Decoder::_decodeReference()
{
    uint16_t index = this->readUShort();
    return _objAt(index);
}

