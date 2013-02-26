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

using namespace std;

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
    string *_str;
    vector<char> *_byteArray;
    
}ALBType;

class ALBObject
{
private:
    ALBType m_uData;
    vector<ALBObject*> *_array;
    map<string, ALBObject*> *_properties;
    
// API
public:
    ALBObject& operator=(const u_int8_t& v);
    ALBObject& operator=(const u_int16_t& v);
    ALBObject& operator=(const u_int32_t& v);
    ALBObject& operator=(const int32_t& v);
    ALBObject& operator=(string& v);
    ALBObject& operator=(const float& v);
    ALBObject& operator=(const double& v);
    ALBObject& operator=(const bool& v);
    
    operator uint8_t();
    operator uint16_t();
    operator uint32_t();
    operator int32_t();
    operator float();
    operator double();
    operator string();
    
    ALBObject& operator=(vector<ALBObject*> &array);
    ALBObject& operator=(map<string, ALBObject*> &dict);
    
    operator vector<ALBObject*>();
    operator map<string, ALBObject*>();
    
    ALBObject& operator [](const uint32_t &index);
    ALBObject& operator [](const string &key);
    
    void push(ALBObject& o);
    ALBObject& pop();
    
public:
    ALBObject()
    {
        _properties = NULL;
        _array = NULL;
        m_uData._uint8 = 0;
    }
};

ALBIN_AMF_END



#endif /* defined(__CocosAmf__ALBObject__) */
