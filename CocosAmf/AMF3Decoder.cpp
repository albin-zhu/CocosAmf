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
    m_stringTable = new vector<string>();
    m_traitsTable = new vector<AMF3TraitsInfo*>();
}

AMF3Decoder::~AMF3Decoder()
{
    AMFDecoder::~AMFDecoder();
   
    delete m_stringTable;
    delete m_stringTable;
}

string& AMF3Decoder::_stringAt(uint32_t index)
{
    if (m_stringTable->capacity() <= index)
    {
        char* s = new char[30];
        sprintf(s, "string& AMF3Decoder::_stringAt(uint32_t index) index = %d", index);
        printf("%s",s);
        throw s;
    }
    return (*m_stringTable)[index];
}

AMF3TraitsInfo* AMF3Decoder::_traitsAt(uint32_t index)
{
    if (m_traitsTable->capacity() <= index)
    {
        char* s = new char[30];
        sprintf(s, "AMF3TraitsInfo* AMF3Decoder::_traitsAt(uint32_t index) index = %d", index);
        printf("\n\nERROR: %s\n",s);
        throw s;
    }
    return (*m_traitsTable)[index];
}

string& AMF3Decoder::readUTF()
{
    string *res = new string("");
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _stringAt(ref);
    }
    uint32_t length = ref >> 1;
    if (length == 0)
    {
        return *res;
    }
    
    *res = this->readUTF(length);
    m_stringTable->push_back(*res);
    return *res;
}

string& AMF3Decoder::readUTF(uint32_t len)
{
    return AMFStream::readUTF(len);
}

ALBObject&  AMF3Decoder::beginDecode()
{
    readShort();
    cout<<"AMFEncoding:"<<m_encoding<<endl;
    short headCount = this->readShort();
    cout<<"Heades count:"<<headCount<<endl;
    short bodyCount = this->readShort();
    cout<<"Bodys count:"<<bodyCount<<endl;
    
    string& target = this->readUTF();
    cout<<"Target:"<<target<<endl;
    string& resp = this->readUTF();
    cout<<"Response:"<<resp<<endl;
    
    cout<<int(this->readUInt())<<endl;
    
    return decodeObject();
}


ALBObject&  AMF3Decoder::decodeObject()
{
    AMF3Type type;
    readType(type);
    return (ALBObject& )_decode(type);
}

ALBObject& AMF3Decoder::_decode(AMF3Type &type)
{
    printf("AMF3Decoder::_decode(AMF3Type type) type = 0x%02x\n", type);
    ALBObject *tmp = new ALBObject();
    ALBObject &value = *tmp;
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
            value = false;
            break;
        }
			
		case kAMF3TrueType:
        {
            value = true;
            break;
        }

			
		case kAMF3IntegerType:
        {
			int32_t intValue = readUInt29();
			value = (intValue << 3) >> 3;
			break;
		}
			
		case kAMF3DoubleType:
        {
            value = readDouble();
			break;
        }
			
		case kAMF3UndefinedType:
        {
            ALBObject *a = new ALBObject();
			return *a;
			break;
        }
			
		case kAMF3NullType:
        {
            ALBObject *a = new ALBObject();
			return *a;
			break;
        }
			
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


ALBObject&  AMF3Decoder::_decodeXML()
{
    return AMF3Decoder::_decodeXML();
}


ALBObject&  AMF3Decoder::_decodeAsObject(cocos2d::CCString *clazName)
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _objAt(ref);
    }
    
    AMF3TraitsInfo *info = this->_decodeTraits(ref);
  
    ALBObject *tmp = new ALBObject();
    ALBObject &object = *tmp;
    
    if(info->className.length() > 0)
    {
        object = new ALBObject();
        object.type = info->className;
        object.externalizable = info->externalizable;
    }
    
    m_objectTable->push_back(tmp);
    
    
    vector<string>::iterator it;
    for (it = info->properties->begin(); it != info->properties->end(); it++)
    {
        string key = (string)*it;
        int k = info->properties->capacity();
        if(key.length() > 0)
        {
            long len = key.length();
            printf("%ld", len);
            object[key] = decodeObject();
        }
       
    }
    
    
    if (info->dynamic)
    {
        string &key = this->readUTF();
        while (key.length() > 0)
        {
            object[key] = decodeObject();
            key = readUTF();
        }
    }
    return object;
}

ALBObject&  AMF3Decoder::_decodeArray()
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _objAt(ref);
    }
    
    uint32_t length = ref >> 1;
    ALBObject*  array = NULL;
    
    for(;;)
    {
        string &name = this->readUTF();
        if(name.length() == 0)
        {
            break;
        }
        
        if(array == NULL)
        {
            array = new ALBObject();
            m_objectTable->push_back(array);
        }
        
        (*array)[name] = decodeObject();
    }
    
    if(array == NULL)
    {
        array = new ALBObject();
        m_objectTable->push_back(array);
        for (uint32_t i = 0; i < length; i++)
        {
            array->push(decodeObject());
        }
    }
    else
    {
        for (uint32_t i = 0; i < length; i++)
        {
            array->push(decodeObject());
        }
    }
    
    return *array;
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
    string &clazName = readUTF();
    
    AMF3TraitsInfo *info = new AMF3TraitsInfo(clazName, externalizable, dynamic, count);
    
    while (count--)
    {
        info->addProperty(this->readUTF());
    }
    m_objectTable->push_back(info);
    return info;
}

ALBObject&  AMF3Decoder::_decodeDate()
{
    uint32_t ref = this->readUInt29();
    if((ref & 1) == 0)
    {
        ref = (ref >> 1);
        return _objAt(ref);
    }
    
    double time = this->readDouble();
    ALBObject *t = new ALBObject();
    *t = time;
    return *t;
}