//
//  AMFEncoder.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-27.
//
//

#ifndef __CocosAmf__AMFEncoder__
#define __CocosAmf__AMFEncoder__

#include "AMF3.h"
#include "ALBObject.h"
#include "AMF3TraitsInfo.h"
#include "AMFActionMessage.h"

ALBIN_AMF

class ByteBuffer
{
public:
    ByteBuffer()
    {
        m_bytes.clear();
        m_position = 0;
    }
    
public:
    inline uint32_t getSize()
    {
        return m_bytes.size();
    }
    
    inline const std::vector<char>& getBytes()
    {
        return m_bytes;
    }
    
    inline const char* getbuf()
    {
        uint32_t size = getSize();
        char* res = new char(size);
        for (uint32_t i = 0; i < size; i++)
        {
            res[i] = m_bytes[i];
        }
        return res;
    }
private:
    std::vector<char> m_bytes;
    uint32_t m_position;
    
private:
    void _apply(const void* bytes, uint32_t len);
    void _before(const void* bytes, uint32_t len);
    void _apb(char v, uint32_t p);
    
public:
    
    void writeBoll(bool v);
    void writeDouble(double v);
    void writeChar(int8_t v);
    void writeFloat(float v);
    void writeInt(int32_t v);
    void writeByte(uint8_t v);
    void writeUint(uint32_t v);
    void writeUShort(uint16_t v);
    void writeU29(int32_t v);
    //    void write(ALBObject &v);
    void writeString(std::string v);
    
    void writeBuf(const char* buf, uint32_t size);
};

class AMFEncoder
{
protected:
    ByteBuffer m_bytes;
//    uint32_t m_position;
    std::vector<std::string> stringArr;
    ALBObject* objects;
    std::vector<AMF3TraitsInfo*> traits;
    std::vector<ALBObject*> byte_arrays;
    
    
public:
    AMFEncoder();

public:
    const char* encode(AMFActionMessage& message);

//protected:
//    void operator+(bool v);
//    void operator+(double v);
//    void operator+(int8_t v);
//    void operator+(float v);
//    void operator+(int32_t v);
//    void operator+(uint8_t v);
//    void operator+(uint32_t v);
//    void operator+(uint16_t v);
//    bool _encodeU29(uint32_t v);
////    void operator+(ALBObject &v);
//    void operator+(std::string &v);
    
//    void _setContext(const ALBObject& context);
//    void _restoreContext();
    
// virtual protected:
public:
    inline ByteBuffer& getBuf()
    {
        return m_bytes;
    }
    
    inline uint32_t getSize()
    {
        return m_bytes.getSize();
    }
    
    void _encodeTraits(AMF3TraitsInfo& traits);

    
    virtual void _encode(ALBObject& source);
    virtual void _encodeArray(ALBObject& source);
    virtual void _encodeObject(ALBObject& source);
    virtual void _encodeString(std::string& source, bool writeType = true);
    virtual void _encodeNull();
    virtual void _encodeTrue();
    virtual void _encodeFalse();
    virtual void _encodeDouble(ALBObject& source);
    virtual void _encodeInteger(ALBObject& source);
    virtual void _encodeUndefined();
    virtual void _encodeByteArray();
    
    virtual void _encodeString0(std::string);
};


ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFEncoder__) */
