//
//  AMF3Decoder.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#ifndef __CocosAmf__AMF3Decoder__
#define __CocosAmf__AMF3Decoder__

#include "AMFDecoder.h"
#include "AMF3TraitsInfo.h"

ALBIN_AMF

class AMF3Decoder : public AMFDecoder
{
    
public:
    AMF3Decoder(std::vector<char> stram, uint32_t pos = 0, AMFVERSION encoding = kAMF0);
    virtual ~AMF3Decoder();
    
public:
    // AMFStream::
    virtual string& readUTF();
    virtual string& readUTF(uint32_t len);
    virtual ALBObject& decodeObject();
    virtual ALBObject& beginDecode();
    
protected:
    std::vector<string> *m_stringTable;
    std::vector<AMF3TraitsInfo*> *m_traitsTable;
    
protected:
    virtual ALBObject& _decode(AMF3Type &type);
    virtual ALBObject& _decodeArray();
    virtual ALBObject& _decodeAsObject(cocos2d::CCString *clazName);
    virtual ALBObject& _decodeXML();
    virtual ALBObject& _decodeDate();
//    virtual ALBObject& _decodeReference();
    
    AMF3TraitsInfo *_decodeTraits(uint32_t infoBits);
    std::vector<uint8_t>* decodeByteArry();
    
    string& _stringAt(uint32_t index);
    AMF3TraitsInfo* _traitsAt(uint32_t index);
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMF3Decoder__) */
