//
//  ALBObject.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-26.
//
//

#include "ALBObject.h"

US_ALBIN_AMF;
using namespace std;

ALBObject& ALBObject::operator=(const u_int8_t &v)
{
    m_uData._uint8 = v;
    m_dataType = kAMF3IntegerType;
    return *this;
}

ALBObject& ALBObject::operator=(const u_int16_t &v)
{
    m_dataType = kAMF3IntegerType;
    m_uData._uint16 = v;
    return *this;
}

ALBObject& ALBObject::operator=(const u_int32_t &v)
{
    m_dataType = kAMF3IntegerType;
    m_uData._uint32 = v;
    return *this;
}

ALBObject& ALBObject::operator=(const int32_t &v)
{
    m_dataType = kAMF3IntegerType;
    m_uData._uint32 = v;
    return *this;
}

ALBObject& ALBObject::operator=(string &v)
{
    m_dataType = kAMF3StringType;
    string *value = new string(v);
    m_uData._str = value;
    return *this;
}

ALBObject& ALBObject::operator=(const double &v)
{
    m_dataType = kAMF3DoubleType;
    m_uData._double = v;
    return *this;
}

ALBObject& ALBObject::operator=(const float &v)
{
    m_dataType = kAMF3DoubleType;
    m_uData._float = v;
    return *this;
}

ALBObject& ALBObject::operator=(const bool &v)
{
    m_dataType = kAMF3TrueType;
    m_uData._bool = v;
    return *this;
}

ALBObject& ALBObject::operator=(vector<ALBObject*>& array)
{
    m_dataType = kAMF3ArrayType;
    m_uData._array = &array;
    return *this;
}

ALBObject& ALBObject::operator=(map<string, ALBObject*>& dict)
{
    m_dataType = kAMF3ObjectType;
    m_uData._properties = &dict;
    return *this;
}

ALBObject::operator uint8_t()
{
    return m_uData._uint8;
}

ALBObject::operator uint16_t()
{
    return m_uData._uint16;
}

ALBObject::operator uint32_t()
{
    return m_uData._uint32;
}

ALBObject::operator int32_t()
{
    return m_uData._int32;
}

ALBObject::operator double()
{
    return m_uData._double;
}

ALBObject::operator float()
{
    return m_uData._float;
}

ALBObject::operator string()
{
    return *m_uData._str;
}

ALBObject::operator vector<ALBObject*>()
{
    return *m_uData._array;
}

ALBObject::operator map<string, ALBObject*>()
{
    return *m_uData._properties;
}

ALBObject& ALBObject::operator [](const uint32_t &index)
{
    if (m_dataType != kAMF3ArrayType || !m_uData._array)
    {
        m_dataType = kAMF3ArrayType;
        m_uData._array = new vector<ALBObject*>();
    }    return (*(*m_uData._array)[index]);
}

 ALBObject& ALBObject::operator [](const string &key)
{
    if (m_dataType != kAMF3ObjectType || !m_uData._properties)
    {
        m_dataType = kAMF3ObjectType;
        m_uData._properties = new map<string, ALBObject*>();
    }
    ALBObject *tmp = (*m_uData._properties)[key];
    if (tmp == NULL)
    {
        tmp = new ALBObject();
        (*m_uData._properties)[key] = tmp;
    }
    return *tmp;
}

void ALBObject::push(ALBObject &o)
{
    if (m_dataType != kAMF3ArrayType || !m_uData._array)
    {
        m_dataType = kAMF3ArrayType;
        m_uData._array = new vector<ALBObject*>();
    }
    
    m_uData._array->push_back(&o);
}

ALBObject& ALBObject::pop()
{
    vector<ALBObject*> &arr = *m_uData._array;
    ALBObject *tmp = arr[arr.capacity() - 1];
    m_uData._array->pop_back();
    return *tmp;
}

ALBObject::~ALBObject()
{
    switch (m_dataType) {
        case kAMF3StringType:
        {
            m_uData._str->clear();
            delete m_uData._str;
            break;
        }
            
        case kAMF3ArrayType:
        {
            m_uData._array->clear();
            delete m_uData._array;
            break;
        }
            
        case kAMF3ObjectType:
        {
            m_uData._properties->clear();
            delete m_uData._properties;
            break;
        }
            
        default:
            break;
    }
    
    type.clear();
}

int32_t ALBObject::indexOf(const AMF::ALBObject &data)
{
    if(m_dataType != kAMF3ArrayType)
    {
        return -1;
    }
    else
    {
        int i = 0;
        vector<ALBObject*>::iterator it;
        for (it = m_uData._array->begin(); it != m_uData._array->end(); it++)
        {
            if(*it == &data)
            {
                return i;
            }
            i++;
        }
    }
    return -1;
}