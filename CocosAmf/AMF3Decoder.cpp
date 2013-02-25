//
//  AMF3Decoder.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-25.
//
//

#include "AMF3Decoder.h"

US_ALBIN_AMF;
USING_NS_CC;

AMF3Decoder::AMF3Decoder(std::vector<char> stram, uint32_t pos, AMFVERSION encoding):AMFDecoder(stram, pos, encoding)
{
    m_stringTable = CCArray::create();
    m_traitsTable = CCArray::create();
}

AMF3Decoder::~AMF3Decoder()
{
    AMFDecoder::~AMFDecoder();
   
    m_stringTable->release();
    m_traitsTable->release();
    
    CC_SAFE_DELETE(m_traitsTable);
    CC_SAFE_DELETE(m_stringTable);
}