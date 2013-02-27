//
//  ALBObject.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-26.
//
//

#ifndef __CocosAmf__ALBObject__
#define __CocosAmf__ALBObject__

#include "AMF3.h"

ALBIN_AMF

class ALBObject;

typedef union
{
    uint8_t _uint8;
    uint16_t _uint16;
    uint32_t _uint32;
    uint32_t _uint29;
    int32_t _int32;
    int16_t _int16;
    float _float;
    double _double;
    bool _bool;
    std::string *_str;
    std::vector<char> *_byteArray;
    std::vector<ALBObject*> *_array;
    std::map<std::string, ALBObject*> *_properties;
    
}ALBType;

class ALBObject
{
private:
    ALBType m_uData;
    AMF3Type m_dataType;
    
    
public:
    inline AMF3Type getDataType()
    {
        return m_dataType;
    }
    std::string type;
    bool externalizable;
    uint32_t count;
    
// API
public:
    ALBObject& operator=(const u_int8_t& v);
    ALBObject& operator=(const u_int16_t& v);
    ALBObject& operator=(const u_int32_t& v);
    ALBObject& operator=(const int32_t& v);
    ALBObject& operator=(std::string& v);
    ALBObject& operator=(const float& v);
    ALBObject& operator=(const double& v);
    ALBObject& operator=(const bool& v);
    
    operator uint8_t();
    operator uint16_t();
    operator uint32_t();
    operator int32_t();
    operator float();
    operator double();
    operator std::string();
    
    ALBObject& operator=(std::vector<ALBObject*> &array);
    ALBObject& operator=(std::map<std::string, ALBObject*> &dict);
    
    operator std::vector<ALBObject*>();
    operator std::map<std::string, ALBObject*>();
    
    ALBObject& operator [](const uint32_t &index);
    ALBObject& operator [](const std::string &key);
    
    void push(ALBObject& o);
    ALBObject& pop();
    
public:
    ALBObject()
    {
        type = std::string("");
        m_dataType = kAMF3UndefinedType;
    }
    
public:
    int32_t indexOf(const ALBObject& data);
    
    ~ALBObject();
};

ALBIN_AMF_END



#endif /* defined(__CocosAmf__ALBObject__) */
