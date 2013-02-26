//
//  ALBObject.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-26.
//
//

#include "ALBObject.h"

US_ALBIN_AMF;

ALBObject& ALBObject::operator=(const u_int8_t &v)
{
    m_uData._uint8 = v;
    return *this;
}

ALBObject& ALBObject::operator=(const u_int16_t &v)
{
    m_uData._uint16 = v;
    return *this;
}

ALBObject& ALBObject::operator=(const u_int32_t &v)
{
    m_uData._uint32 = v;
    return *this;
}

ALBObject& ALBObject::operator=(const int32_t &v)
{
    m_uData._uint32 = v;
    return *this;
}

ALBObject& ALBObject::operator=(string &v)
{
    m_uData._str = &v;
    return *this;
}

ALBObject& ALBObject::operator=(const double &v)
{
    m_uData._double = v;
    return *this;
}

ALBObject& ALBObject::operator=(const float &v)
{
    m_uData._float = v;
    return *this;
}

ALBObject& ALBObject::operator=(const bool &v)
{
    m_uData._bool = v;
    return *this;
}

ALBObject& ALBObject::operator=(vector<ALBObject*>& array)
{
    _array = &array;
    return *this;
}

ALBObject& ALBObject::operator=(map<string, ALBObject*>& dict)
{
    _properties = &dict;
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
    return *_array;
}

ALBObject::operator map<string, ALBObject*>()
{
    return *_properties;
}

ALBObject& ALBObject::operator [](const uint32_t &index)
{
    if (!_array)
    {
        _array = new vector<ALBObject*>();
    }    return (*(*_array)[index]);
}

 ALBObject& ALBObject::operator [](const string &key)
{
    if (!_properties)
    {
        _properties = new map<string, ALBObject*>();
    }
    ALBObject *tmp = (*_properties)[key];
    if (tmp == NULL)
    {
        tmp = new ALBObject();
        (*_properties)[key] = tmp;
    }
    return *tmp;
}

void ALBObject::push(ALBObject &o)
{
    if (!_array)
    {
        _array = new vector<ALBObject*>();
    }
    
    _array->push_back(&o);
}

ALBObject& ALBObject::pop()
{
    vector<ALBObject*> &arr = *_array;
    ALBObject *tmp = arr[arr.capacity() - 1];
    _array->pop_back();
    return *tmp;
}

