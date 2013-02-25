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

AMFDecoder::~AMFDecoder()
{
    AMFStream::~AMFStream();
}

AMF0Decoder::AMF0Decoder(std::vector<char>& stream):AMFDecoder(stream)
{
    
}

void* AMF0Decoder::_decode(AMF0Type type)
{
    void *value = NULL;
	switch (type){
		case kAMF0NumberType:
            double num = this->readDouble();
            value = &num;
			break;
			
		case kAMF0BooleanType:
            bool b = this->readBoolean();
			value = &b;
			break;
			
		case kAMF0StringType:
			value = this->readUTF();
			break;
			
		case kAMF0AVMPlusObjectType:
        {
            AMF3Decoder *amf3Decoder = new AMF3Decoder(m_amfStream, m_position, kAMF3);
        
			value = amf3Decoder->decodeObject();
            m_position = amf3Decoder->getPos();
			amf3Decoder->~AMF3Decoder();
			break;
		}
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
			value = this->_decodeAsOBject(NULL);
			break;
			
		case kAMF0XMLObjectType:
			value = this->_decodeXML();
			break;
			
		case kAMF0NullType:
			value = NULL;
			break;
			
		case kAMF0DateType:
			value = this->_decodeDate();
			break;
			
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



