//
//  AMF3TraitsInfo.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-22.
//
//

#ifndef __CocosAmf__AMF3TraitsInfo__
#define __CocosAmf__AMF3TraitsInfo__

#include "AMF3.h"
#include "ALBObject.h"

ALBIN_AMF

class AMF3TraitsInfo : public ALBObject
{
public:
    AMF3TraitsInfo();
    AMF3TraitsInfo(std::string&, bool, bool, int);
    ~AMF3TraitsInfo();
    
    std::string className;
    bool dynamic;
    bool externalizable;
    int count;
    std::vector<std::string> *properties;
    
    void addProperty(std::string& p);
    
    virtual bool isEqual(const AMF3TraitsInfo* pObject);
//    string& toString();
};

ALBIN_AMF_END

#endif /* defined(__CocosAmf__AMF3TraitsInfo__) */
