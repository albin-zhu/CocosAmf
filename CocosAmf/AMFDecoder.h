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
#include "ALBObject.h"


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
    ALBObject& _objAt(uint32_t index);
    ALBObject& _decodeNumberForKey(std::string& key);
    
protected:
    // DecodeForKey
    bool decodeBoolForKey(std::string& key);
    double decodeDoubleForKey(std::string& key);
    float decodeFloatForKey(std::string& key);
    int32_t decodeInt32ForKey(std::string& key);
    int64_t decodeInt64ForKey(std::string& key);
    int decodeIntForKey(std::string& key);
    ALBObject& decodeObjectForKey(std::string& key);

// uncompress
public:
    std::string& decodeMultiBytesString(uint32_t len, AMFVERSION encoding);
    virtual ALBObject& decodeObject();
    virtual ALBObject& beginDecode();
    
    
protected:
    std::vector<ALBObject*>* m_objectTable;
    ALBObject *m_currentDeserializedObject;
    
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFDecoder__) */
