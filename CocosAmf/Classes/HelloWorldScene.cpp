#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AMF3Decoder.h"
#include "ALBObject.h"
#include "AMFActionMessage.h"
#include "AMFEncoder.h"


using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocos2d::extension;
using namespace AMF;
using namespace std;


CCScene* HelloWorld::scene()
{
//    BinaryUtil* t = new BinaryUtil();

    
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::onRecieved(CCObject *sender, CCHttpResponse* data)
{
//    AMFDecoder *decoder = AMFDecoder::getDecoder(*data->getResponseData());
//    ALBObject &obj = decoder->beginDecode();
//
//    string timesatmap = obj[string("data")][0][string("metadata")][string("DailyTask")][0][string("name")];
//    CCLog("Content.data[0].data.timestamp = %s", timesatmap.c_str());
//    CCLog("size of ALBObject = %ld", sizeof(ALBObject));
//    
//    delete &obj;
//    string url(data->getHttpRequest()->getUrl());
//    if(url.compare("http://127.0.0.1/Examples/Php/") != 0)
//    {
//        CCHttpClient *client = CCHttpClient::getInstance();
//        CCHttpRequest *req = new CCHttpRequest();
//        req->setUrl("http://127.0.0.1/Examples/Php/");
//        
//        vector<char> &d = *data->getResponseData();
//        char *buf = new char(d.size());
//        
//        for (uint32_t i = 0; i < d.size() ; i++) {
//            buf[i] = d[i];
//            printf("%d",buf[i]);
//            if(i % 10 == 0)
//            {
//                printf("\n");
//            }
//        }
//        
//        CCLog("%s", buf);
//        
//        req->setRequestData(buf, data->getResponseData()->size());
//        req->setContenType(AMF3_HEADER_CONTENT);
//        req->setRequestType(CCHttpRequest::kHttpPost);
//        req->setResponseCallback(this, callfuncND_selector(HelloWorld::onRecieved));
//        client->send(req);
//    }
//    else
//    {
    vector<char> d = *data->getResponseData();
    char *err = new char[d.size()];
    for (uint32_t i = 0; i < d.size(); i++)
    {
        err[i] = d[i];
    }
    printf("%s", err);

    
        AMFActionMessage *m = new AMFActionMessage(*data->getResponseData());
 
        AMFMessageBody* body = m->getBodies()[0];
        body->data->toString();
//    }
    

}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    AMFEncoder *encoder = new AMFEncoder();
    
    AMFActionMessage* message = new AMFActionMessage();
   
    ALBObject *data = new ALBObject();
    
    int x = 33;
    (*data)[string("age")] = x;
    (*data)[string("status")] = "nothing yet";
    (*data)[string("name")] = "Areil";
   
    
    ALBObject* content = new ALBObject();
    content->push(*data);
    message->addBody("VoService/receiveAndReturnUserVo2", "/1", *content);
    const char* buf = encoder->encode(*message);
    uint32_t len = encoder->getSize();
  
    CCHttpClient *client = CCHttpClient::getInstance();
    CCHttpRequest *req = new CCHttpRequest();
    req->setUrl("http://127.0.0.1/Examples/Php/");
    
    
    req->setRequestData(buf, len);
    req->setContenType(AMF3_HEADER_CONTENT);
    req->setRequestType(CCHttpRequest::kHttpPost);
    req->setResponseCallback(this, callfuncND_selector(HelloWorld::onRecieved));
    client->send(req);


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
