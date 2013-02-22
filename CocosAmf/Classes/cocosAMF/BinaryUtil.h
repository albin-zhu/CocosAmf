//
//  BinaryUtil.h
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-21.
//
//

#ifndef __CocosAmf__BinaryUtil__
#define __CocosAmf__BinaryUtil__

#include <ostream>
#include <vector>
#include "cocos2d.h"
#include "AMFObject.h"
#include "AMFMessage.h"

enum AMFVERSION
{
    kAMF0 = 0x00,
    kAMF3 = 0x03
};

class BinaryUtil
{
public:
    BinaryUtil(std::vector<char> &stream);
    BinaryUtil(std::vector<char> &stream, u_int32_t pos, AMFVERSION ver);
    ~BinaryUtil();
    
    AMF::AMFMessage* decodeAmf();
    
    int readInt();
    u_int32_t readUInt();
    int8_t readShort();
    u_int16_t readUShort();
    u_int8_t readUChar();
    double readDouble();
    float readFloat();
    bool readBoolean();
    char* readString();
    u_int32_t readUInt29();
    cocos2d::CCString* readUTF8();
    cocos2d::CCString* readUTF8WithLen(u_int32_t);
    
    cocos2d::CCObject* readObject(AMF0Type type);
    cocos2d::CCObject* readObject(AMF3Type type);
    
    cocos2d::CCArray* readArray();
    u_int8_t* readByteArry();
    
    inline int getPostion()
    {
        return m_position;
    }
    
    inline void setPosition(int p)
    {
        m_position = p;
    }
    
    inline void addPosition(int p)
    {
        m_position += p;
    }
    
    AMF::AMFMessage* decodeAmf0();
    AMF::AMFMessage* decodeAmf3();
    
    cocos2d::CCObject* _decodeAmf0();
    cocos2d::CCObject* _decodeAmf3();
    cocos2d::CCObject* _findRef(u_int32_t);
    


private:
    cocos2d::CCArray* m_objectTabel;
    bool ensureLength(int);
    int m_position;
    std::vector<char> m_amf3Stream;
    AMFVERSION amfVersion;
};

#endif /* defined(__CocosAmf__BinaryUtil__) */
