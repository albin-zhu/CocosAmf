//
//  AMFStream.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#ifndef __CocosAmf__AMFStream__
#define __CocosAmf__AMFStream__

#include "AMF3.h"
#include <vector>

ALBIN_AMF

class AMFStream {
    
public:
    AMFStream(std::vector<char>& stream, uint32_t pos = 0, AMF::AMFVERSION encoding = kAMF0);
    ~AMFStream();
    
public:
    int readInt();
    u_int32_t readUInt();
    int8_t readShort();
    u_int16_t readUShort();
    u_int8_t readUChar();
    uint8_t readType(AMF0Type &type);
    uint8_t readType(AMF3Type &type);
    double readDouble();
    float readFloat();
    bool readBoolean();
    char* readString();
    u_int32_t readUInt29();
    virtual std::string readUTF();
    std::string readUTF(uint32_t len);
    
    inline uint32_t getBytesAvailable()
    {
        return m_amfStream.capacity() - m_position;
    }
    
    inline uint32_t getPos()
    {
        return m_position;
    }
    
protected:
    bool ensureLength(uint32_t len);
    AMF::AMFVERSION m_encoding;
    uint32_t m_position;
    std::vector<char> m_amfStream;
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFStream__) */
