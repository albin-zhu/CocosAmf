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
    
    
    AMFActionMessage *m = new AMFActionMessage(*data->getResponseData());
 
    AMFMessageBody* body = m->getBodies()[0];
    string timesatmap = (*body->data)[string("data")][0][string("metadata")][string("DailyTask")][0][string("name")];
    CCLog("Content.data[0].data.timestamp = %s", timesatmap.c_str());
    

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
    
    CCHttpClient *client = CCHttpClient::getInstance();
    CCHttpRequest *req = new CCHttpRequest();
    req->setUrl("http://127.0.0.1/message");
    req->setRequestData("armyId=17024", 12);
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
