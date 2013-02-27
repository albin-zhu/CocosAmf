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

ALBIN_AMF

class AMFEncoder
{
protected:
    std::vector<char> m_bytes;
    uint32_t m_position;
    std::vector<ALBObject*> *m_objectTable;
    std::vector<ALBObject*> *m_writeStack;
    
public:
    AMFEncoder();

protected:
    void operator+(bool v);
    void operator+(double v);
    void operator+(int8_t v);
    void operator+(float v);
    void operator+(int32_t v);
    void operator+(uint8_t v);
    void operator+(uint32_t v);
    void operator+(uint16_t v);
    void operator*(int32_t v);
    void operator+(ALBObject &v);
    void operator+(std::string &v);
    
// virtual protected:
protected:
    virtual void _encode(ALBObject&);
    virtual void _encodeArray(std::vector<ALBObject*> data);
    virtual void _encodeDictionary(std::map<std::string, ALBObject*> data);
    virtual void _encodeNumber(double data);
    virtual void _encodeString(std::string data);
    virtual void _encodeNull();
    virtual void _encodeCustomObject(ALBObject& data);
       
private:
    void _apply(const void* bytes, uint32_t len);
    void _before(const void* bytes, uint32_t len);
    void _apb(uint8_t v, uint32_t p);
};


ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFEncoder__) */
