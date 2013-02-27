//
//  AMFActionMessage.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-27.
//
//

#ifndef __CocosAmf__AMFActionMessage__
#define __CocosAmf__AMFActionMessage__

#include "AMF3.h"
#include "ALBObject.h"

ALBIN_AMF

class AMFMessageHeader
{
public:
    std::string name;
    bool mustUnderstand;
    ALBObject *data;
    
    
    static AMFMessageHeader* getMessage(const std::string& name, const ALBObject& data, bool u);
};

class AMFMessageBody
{
public:
    std::string targetURI;
    std::string responseURI;
    ALBObject* data;
};


class AMFActionMessage
{
private:
    std::vector<AMFMessageHeader*> *m_heades;
    std::vector<AMFMessageBody*> *m_bodies;
    AMFVERSION m_version;
    
    void _initData(std::vector<char>&);
    
public:
    inline AMFVERSION getVersion()
    {
        return m_version;
    }
    
    inline std::vector<AMFMessageHeader*>& getHeaders()
    {
        return *m_heades;
    }
    
    inline uint16_t getMessageCount()
    {
        return m_bodies->capacity();
    }
    
    inline std::vector<AMFMessageBody*>& getBodies()
    {
        return *m_bodies;
    }
    
    void addBody(const std::string& targetURI, const std::string& responseURI, ALBObject& data);
    void addHeader(const std::string& name, bool ud, ALBObject& data );

    void merge(AMFActionMessage& message);
    
    AMFActionMessage();
    AMFActionMessage(std::vector<char>& data);
};


ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFActionMessage__) */
