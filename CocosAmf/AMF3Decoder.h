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
    
protected:
    cocos2d::CCArray *m_stringTable;
    cocos2d::CCArray *m_traitsTable;
    
private:
    cocos2d::CCObject *_decode(AMF0Type type);
    cocos2d::CCObject *_decodeArray();
    cocos2d::CCObject *_decodeAsOBject(cocos2d::CCString *clazName);
    cocos2d::CCObject *_decodeXML();
    cocos2d::CCString *_decodeDate();
    cocos2d::CCObject *_decodeReference();
    
    AMF3TraitsInfo *_decodeTraits(uint32_t infoBits);
    std::vector<uint8_t>* decodeByteArry();
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMF3Decoder__) */
