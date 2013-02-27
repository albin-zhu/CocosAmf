//
//  AMFActionMessage.cpp
//  CocosAmf
//
//  Created by Albin Zhu on 13-2-27.
//
//

#include "AMFActionMessage.h"
#include "AMFDecoder.h"

using namespace std;
US_ALBIN_AMF;

AMFActionMessage::AMFActionMessage()
{
    m_heades = new vector<AMFMessageHeader*>();
    m_bodies = new vector<AMFMessageBody*>();
    m_version = kAMF3;
}

AMFActionMessage::AMFActionMessage(vector<char>& data)
{
    _initData(data);
}

void AMFActionMessage::addBody(const std::string &targetURI, const std::string &responseURI, AMF::ALBObject &data)
{
    AMFMessageBody *body = new AMFMessageBody();
    body->targetURI = targetURI;
    body->responseURI = responseURI;
    body->data = &data;
    m_bodies->push_back(body);
}

void AMFActionMessage::addHeader(const std::string &name, bool ud, AMF::ALBObject &data)
{
    AMFMessageHeader *header = new AMFMessageHeader();
    header->name = name;
    header->mustUnderstand = ud;
    header->data = &data;
    m_heades->push_back(header);
}

// Binary to Object
void AMFActionMessage::_initData(vector<char> &data)
{
    AMFDecoder *ba = AMFDecoder::getDecoder(data);
    m_version = (AMFVERSION)ba->readShort();
    uint16_t numHeaders = ba->readShort();
    if(numHeaders > 0)
    {
        m_heades = new vector<AMFMessageHeader*>();
    }
    for (uint16_t i = 0; i < numHeaders; i++)
    {
        AMFMessageHeader *header = new AMFMessageHeader();
        header->name = ba->readUTF();
        header->mustUnderstand = ba->readBoolean();
        ba->readUInt();
        header->data = &ba->decodeObject();
    }
    
    
    uint16_t numBodies = ba->readShort();
    if(numBodies > 0)
    {
        m_bodies = new vector<AMFMessageBody*>();
    }
    for (uint16_t i = 0; i < numBodies; i++)
    {
        AMFMessageBody *body = new AMFMessageBody();
        body->targetURI = ba->readUTF();
        body->responseURI = ba->readUTF();
        ba->readUInt();
        body->data = &ba->decodeObject();
        m_bodies->push_back(body);
    }
}