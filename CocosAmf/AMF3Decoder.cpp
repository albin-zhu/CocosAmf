//
//  AMF3Decoder.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "AMF3Decoder.h"

USING_NS_CC;
US_ALBIN_AMF;

using namespace std;

AMF3Decoder::AMF3Decoder(std::vector<char> stram, uint32_t pos, AMFVERSION encoding):AMFDecoder(stram, pos, kAMF3)
{
    m_stringTable = CCArray::create();
    m_traitsTable = CCArray::create();
}

AMF3Decoder::~AMF3Decoder()
{
    AMFDecoder::~AMFDecoder();
   
    m_stringTable->release();
    m_traitsTable->release();
    
    CC_SAFE_DELETE(m_traitsTable);
    CC_SAFE_DELETE(m_stringTable);
}

CCString* AMF3Decoder::_stringAt(uint32_t index)
{
    if (m_stringTable->count() <= index)
    {
        char* s = new char[30];
        sprintf(s, "CCString* AMF3Decoder::_stringAt(uint32_t index) index = %d", index);
        throw s;
    }
    return (CCString*)m_stringTable->objectAtIndex(index);
}

AMF3TraitsInfo* AMF3Decoder::_traitsAt(uint32_t index)
{
    if (m_traitsTable->count() <= index)
    {
        char* s = new char[30];
        sprintf(s, "AMF3TraitsInfo* AMF3Decoder::_traitsAt(uint32_t index) index = %d", index);
        throw s;
    }
    return (AMF3TraitsInfo*)m_traitsTable->objectAtIndex(index);
}

CCString* AMF3Decoder::readUTF()
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _stringAt(ref);
    }
    uint32_t length = ref >> 1;
    if (length == 0)
    {
        return new CCString("NULL");
    }
    
    CCString *value = this->readUTF(length);
    m_stringTable->addObject(value);
    return value;
}

CCString* AMF3Decoder::readUTF(uint32_t len)
{
    return AMFStream::readUTF(len);
}

CCObject* AMF3Decoder::beginDecode()
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


CCObject* AMF3Decoder::decodeObject()
{
    AMF3Type type;
    readType(type);
    return (CCObject*)_decode(type);
}

void* AMF3Decoder::_decode(AMF3Type type)
{
    printf("AMF3Decoder::_decode(AMF3Type type) type = 0x%2x", type);
    void* value = NULL;
	switch (type){
		case kAMF3StringType:
			value = this->readUTF();
			break;
            
		case kAMF3ObjectType:
			value = this->_decodeAsObject(NULL);
			break;
			
		case kAMF3ArrayType:
			value = this->_decodeArray();
			break;
			
		case kAMF3FalseType:
        {
            bool f = false;
			value = &f;
            break;
        }
			
		case kAMF3TrueType:
        {
            bool f = true;
			value = &f;
            break;
        }

			
		case kAMF3IntegerType:
        {
			int32_t intValue = readUInt29();
			intValue = (intValue << 3) >> 3;
			value = &intValue;
			break;
		}
			
		case kAMF3DoubleType:
        {
            double d = readDouble();
			value = &d;
			break;
        }
			
		case kAMF3UndefinedType:
			return NULL;
			break;
			
		case kAMF3NullType:
			return NULL;
			break;
			
		case kAMF3XMLType:
		case kAMF3XMLDocType:
			value = _decodeXML();
			break;
			
		case kAMF3DateType:
			value = _decodeDate();
			break;
			
//		case kAMF3ByteArrayType:
//			value = _decod;
//			break;
			
		default:
			_cannotDecodeType("Unknown type");
			break;
	}
	//NSLog(@"%@", value);
	return value;
}


CCObject* AMF3Decoder::_decodeXML()
{
    return AMF3Decoder::_decodeXML();
}


CCObject* AMF3Decoder::_decodeAsObject(cocos2d::CCString *clazName)
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _objAt(ref);
    }
    
    AMF3TraitsInfo *info = this->_decodeTraits(ref);
    bool isAs = false;
    CCObject *object;
    
    if(info->className && info-clazName->isEqual(CCString::create("NULL")))
    {
        isAs = true;
        object = new ASObject();
        ((ASObject*)object)->setType(clazName);
        ((ASObject*)object)->externalizable = info->externalizable;
    }
    else
    {
        object = CCDictionary::create();
    }
    m_objectTable->addObject(object);
    
    CCObject *key;
    CCARRAY_FOREACH(info->properties, key)
    {
        if(isAs)
        {
            ((ASObject*)object)->setObject(this->decodeObject(), (CCString*)key);
        }
        else
        {
            ((CCDictionary*)object)->setObject(this->decodeObject(), (CCString*)key);
        }
    }
    
    if (info->dynamic)
    {
        key = this->readUTF();
        while (key != NULL && ((CCString*)key)->length() > 0)
        {
            if(isAs)
            {
                ((ASObject*)object)->setObject(this->decodeObject(), (CCString*)key);
            }
            else
            {
                ((CCDictionary*)object)->setObject(this->decodeObject(), (CCString*)key);
            }
        }
    }
    return object;
}

CCObject* AMF3Decoder::_decodeArray()
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _objAt(ref);
    }
    
    uint32_t length = ref >> 1;
    CCObject* array;
    
    for(;;)
    {
        CCString* name = this->readUTF();
        if(name == NULL || name->length() == 0)
        {
            break;
        }
        
        if(array == NULL)
        {
            array = CCDictionary::create();
            m_objectTable->addObject(array);
        }
        
        ((CCDictionary*)array)->setObject(this->decodeObject(), name);
    }
    
    if(array == NULL)
    {
        array = CCArray::create();
        m_objectTable->addObject(array);
        for (uint32_t i = 0; i < length; i++)
        {
            ((CCArray*)array)->addObject(this->decodeObject());
        }
    }
    else
    {
        for (uint32_t i = 0; i < length; i++)
        {
            ((CCDictionary*)array)->setObject(this->decodeObject(), i);
        }
    }
    
    return array;
}

AMF3TraitsInfo* AMF3Decoder::_decodeTraits(uint32_t infoBits)
{
    if ((infoBits & 3) == 1)
    {
        infoBits = (infoBits >> 2);
        return this->_traitsAt(infoBits);
    }
    
    bool externalizable = (infoBits & 4) == 4;
    bool dynamic = (infoBits & 8) == 8;
    
    uint32_t count = infoBits >> 4;
    CCString *clazName = readUTF();
    
    AMF3TraitsInfo *info = new AMF3TraitsInfo(clazName, externalizable, dynamic, count);
    info->retain();
    
    while (count--)
    {
        info->addProperty(this->readUTF());
    }
    m_objectTable->addObject(info);
    return info;
}

CCObject* AMF3Decoder::_decodeDate()
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _objAt(ref);
    }
    
    double time = this->readDouble();
    CCString* time_str = CCString::createWithFormat("%f", time);
    return time_str;
}