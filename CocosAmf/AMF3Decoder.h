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
    virtual cocos2d::CCString* readUTF();
    virtual cocos2d::CCString* readUTF(uint32_t len);
    virtual cocos2d::CCObject* decodeObject();
    virtual cocos2d::CCObject* beginDecode();
    
protected:
    cocos2d::CCArray *m_stringTable;
    cocos2d::CCArray *m_traitsTable;
    
protected:
    virtual void *_decode(AMF3Type type);
    virtual cocos2d::CCObject *_decodeArray();
    virtual cocos2d::CCObject *_decodeAsObject(cocos2d::CCString *clazName);
    virtual cocos2d::CCObject *_decodeXML();
    virtual cocos2d::CCObject* _decodeDate();
//    virtual cocos2d::CCObject *_decodeReference();
    
    AMF3TraitsInfo *_decodeTraits(uint32_t infoBits);
    std::vector<uint8_t>* decodeByteArry();
    
    cocos2d::CCString* _stringAt(uint32_t index);
    AMF3TraitsInfo* _traitsAt(uint32_t index);
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMF3Decoder__) */
