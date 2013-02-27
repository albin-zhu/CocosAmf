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

ALBObject& AMF0Decoder::beginDecode()
{
    readShort();
    cout<<"AMFEncoding:"<<m_encoding<<endl;
    short headCount = this->readShort();
    cout<<"Heades count:"<<headCount<<endl;
    short bodyCount = this->readShort();
    cout<<"Bodys count:"<<bodyCount<<endl;
    
    string target = this->readUTF();
    cout<<"Target:"<<target<<endl;
    string resp = this->readUTF();
    cout<<"Response:"<<resp<<endl;
    
    cout<<int(this->readUInt())<<endl;
    
    return decodeObject();
}

ALBObject& AMF0Decoder::decodeObject()
{
    AMF0Type type;
    readType(type);
    return _decode(type);
}

ALBObject& AMF0Decoder::_decode(AMF0Type& type)
{
    printf("AMF0Decoder::_decode(AMF0Type type) type = 0x%02x\n", type);
    ALBObject *tmp = new ALBObject();
    ALBObject &value = *tmp;
	switch (type){
		case kAMF0NumberType:
        {
            value = this->readDouble();
        }break;
			
		case kAMF0BooleanType:
        {
            value = this->readBoolean();
        }break;
			
		case kAMF0StringType:
        {
            string a = readUTF();
			value = a;
			break;
        }
			
		case kAMF0AVMPlusObjectType:
        {
            AMF3Decoder *amf3Decoder = new AMF3Decoder(m_amfStream, m_position, kAMF3);
        
			value = amf3Decoder->decodeObject();
            m_position = amf3Decoder->getPos();
//			amf3Decoder->~AMF3Decoder();
		}break;
		case kAMF0StrictArrayType:
			value = this->_decodeArray();
			break;
			
		case kAMF0TypedObjectType:
			value = this->_decodeTypedObject();
			break;
			
		case kAMF0LongStringType:
//			value = this->_decodeLongString();
			break;
			
		case kAMF0ObjectType:
        {
            string cls("");
			value = this->_decodeAsOBject(cls);
        }break;
            
        case kAMF0XMLObjectType:
            value = this->_decodeXML();
            break;
        			
		case kAMF0NullType:
			value = NULL;
			break;
			
		case kAMF0DateType:
        {
			value = this->_decodeDate();
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

vector<ALBObject*>& AMF0Decoder::_decodeArray()
{
    uint32_t size = this->readUInt();
    vector<ALBObject*> *array = new vector<ALBObject*>();
    ALBObject *obj = new ALBObject();
    *obj = *array;
    m_objectTable->push_back(obj);
    for (uint32_t i = 0; i < size; i++)
    {
        ALBObject &t = this->decodeObject();
        if(&t != NULL)
        {
            array->push_back(&t);
        }
    }
    
    return *array;
}

ALBObject& AMF0Decoder::_decodeTypedObject()
{
    string clazName = this->readUTF();
    return this->_decodeAsOBject(clazName);
}

ALBObject& AMF0Decoder::_decodeAsOBject(string &clazName)
{
    ALBObject *tmp = new ALBObject();
    ALBObject &object = *tmp;
    object.type = clazName;

    m_objectTable->push_back(tmp);
    
    string propertyName = this->readUTF();
    AMF0Type type;
    readType(type);
    while (type != kAMF0ObjectEndType)
    {
        object[propertyName] = this->_decode(type);
        propertyName = this->readUTF();
        this->readType(type);
    }
    
    return object;
}

string AMF0Decoder::_decodeLongString()
{
    string *res = new string("");
    uint32_t length = this->readUInt();
    if(length == 0)
    {
        return *res;
    }
    
    return this->readUTF(length);
}

ALBObject& AMF0Decoder::_decodeXML()
{
//    ALBObject *xmlString = new ALBObject();
//    xmlString = this->_decodeLongString();
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS // 呆会再做不同平台的 xml解析。。。算了，还是留给大侠们做吧
//    return *xmlString;
//#else
//    return *xmlString;
//#endif
    
}


ALBObject& AMF0Decoder::_decodeDate()
{
    double time = this->readDouble();
    // 时区
//    u_int16_t timeZonge = this->readUShort();
//    cc_timeval t = {time / 1000, time};
    ALBObject *t = new ALBObject();
    *t = time;
    return *t;
}

ALBObject& AMF0Decoder::_decodeECMAArray()
{
    this->readUInt();
    ALBObject *dict = new ALBObject();
    m_objectTable->push_back(dict);
    
    string propertyName = this->readUTF();
    AMF0Type type;
    readType(type);
    while (type != kAMF0ObjectEndType)
    {
        (*dict)[propertyName] = this->_decode(type);
        propertyName = this->readUTF();
        readType(type);
    }
    
    return *dict;
}

ALBObject& AMF0Decoder::_decodeReference()
{
    uint16_t index = this->readUShort();
    return _objAt(index);
}

