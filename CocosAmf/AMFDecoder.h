//
//  AMFDecoder.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#ifndef __CocosAmf__AMFDecoder__
#define __CocosAmf__AMFDecoder__

#include "AMFStream.h"
#include "ASObject.h"

ALBIN_AMF

class AMFDecoder : public AMFStream
{
    
public:
    AMFDecoder(std::vector<char> stream, uint32_t pos = 0, AMFVERSION encoding = kAMF0);
    static AMFDecoder* getDecoder(std::vector<char> stream);
    virtual ~AMFDecoder();
    
public:
    inline AMFVERSION getEncoding()
    {
        return m_encoding;
    }
    inline bool isAtEnd()
    {
        return m_position >= m_amfStream.capacity();
    }
    
protected:
    void _cannotDecodeType(const char* type);
    cocos2d::CCObject *_objAt(uint32_t index);
    double _decodeNumberForKey(cocos2d::CCString* key);
    
protected:
    // DecodeForKey
    bool decodeBoolForKey(cocos2d::CCString* key);
    double decodeDoubleForKey(cocos2d::CCString* key);
    float decodeFloatForKey(cocos2d::CCString* key);
    int32_t decodeInt32ForKey(cocos2d::CCString* key);
    int64_t decodeInt64ForKey(cocos2d::CCString* key);
    int decodeIntForKey(cocos2d::CCString* key);
    cocos2d::CCObject* decodeObjectForKey(cocos2d::CCString* key);

// uncompress
public:
    cocos2d::CCString* decodeMultiBytesString(uint32_t len, AMFVERSION encoding);
    virtual cocos2d::CCObject *decodeObject();
    virtual cocos2d::CCObject* beginDecode();
    
    
protected:
    cocos2d::CCArray* m_objectTable;
    ASObject *m_currentDeserializedObject;
    
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFDecoder__) */
