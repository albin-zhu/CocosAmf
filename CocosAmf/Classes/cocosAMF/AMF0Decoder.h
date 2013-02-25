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
    
private:
    void *_decode(AMF0Type type);
    cocos2d::CCArray *_decodeArray();
    cocos2d::CCObject *_decodeTypedObject();
    cocos2d::CCObject *_decodeAsOBject(cocos2d::CCString *clazName);
    cocos2d::CCString *_decodeLongString();
    cocos2d::CCObject *_decodeXML();
    cocos2d::CCString *_decodeDate();
    cocos2d::CCDictionary *_decodeECMAArray();
    cocos2d::CCObject *_decodeReference();
};

ALBIN_AMF_END



#endif /* defined(__CocosAmf__AMF0Decoder__) */
