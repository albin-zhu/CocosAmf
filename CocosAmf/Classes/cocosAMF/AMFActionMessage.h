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

class AMFActionMessage
{
private:
    ALBObject *m_heades;
    ALBObject *m_bodies;
    AMFVERSION m_version;
    
public:
    inline AMFVERSION getVersion()
    {
        return m_version;
    }
    
    inline ALBObject& getHeaders()
    {
        return *m_heades;
    }
    
    inline ALBObject& getBodies()
    {
        return *m_bodies;
    }
    
    void addBody(const std::string& targetURI, const std::string& responseURI, const ALBObject& data);
    void addHeader(const std::string& );
};


ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMFActionMessage__) */
