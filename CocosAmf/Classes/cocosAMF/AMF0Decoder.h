//
//  AMF0Decoder.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#ifndef __CocosAmf__AMF0Decoder__
#define __CocosAmf__AMF0Decoder__

#include "AMFDecoder.h"

ALBIN_AMF

class AMF0Decoder : public AMFDecoder
{
public:
    AMF0Decoder(std::vector<char> &stream);
    virtual ALBObject& decodeObject();
    virtual ALBObject&  beginDecode();
//    virtual cocos2d::CCString* readUTF();
    
protected:
    ALBObject& _decode(AMF0Type& type);
    virtual vector<ALBObject*>& _decodeArray();
    virtual ALBObject& _decodeTypedObject();
    virtual ALBObject& _decodeAsOBject(string& clazName);
    string& _decodeLongString();
    virtual ALBObject& _decodeXML();
//    cocos2d::CCString *_decodeDate();
    virtual ALBObject& _decodeDate();
    ALBObject& _decodeECMAArray();
    virtual ALBObject& _decodeReference();
};

ALBIN_AMF_END



#endif /* defined(__CocosAmf__AMF0Decoder__) */
