#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::getScene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
void HelloWorld::addTarget() {
	CCSprite *target = CCSprite::create("Target.png",CCRectMake(0,0,27,40));

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int minY = target->getContentSize().height/2;
	int maxY = winSize.height-target->getContentSize().height/2;

	int rangeY = maxY-minY;
	int actualY = (rand()%rangeY) +minY;

	target->setPosition(ccp(winSize.width+(target->getContentSize().width/2),actualY));

	this->addChild(target);


	//speed
	int minDuration =(int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration-minDuration;

	int actualDuration = (rand()%rangeDuration)+minDuration;

	//create actions
	CCFiniteTimeAction *actionMove = CCMoveTo::create((float)actualDuration,ccp(0-target->getContentSize().width/2,actualY));

	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished));

	target->runAction(CCSequence::create(actionMove,actionMoveDone,NULL));


}


void HelloWorld::gameLogic(float dt) {
	this->addTarget();
}
void HelloWorld::spriteMoveFinished(CCNode* sender) {
	CCSprite *sprite  =(CCSprite *)sender;
	this->removeChild(sprite,true);
}
// on "init" you need to initialize your instance
bool HelloWorld::init() {

	bool bRet = false;
	do {
		//////////////////////////////
		// 1. super init first
		CC_BREAK_IF( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) );
		this->setTouchEnabled(true);
	
		/////////////////////////////
		// 2. add a menu item with "X" image, which is clicked to quit the program
		//    you may modify it.

		// add a "close" icon to exit the progress. it's an autorelease object
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		                                  "CloseNormal.png",
		                                  "CloseSelected.png",
		                                  this,
		                                  menu_selector(HelloWorld::menuCloseCallback) );
		CC_BREAK_IF(!pCloseItem);

		pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

		// create menu, it's an autorelease object
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition( CCPointZero );
		CC_BREAK_IF(!pMenu);

		this->addChild(pMenu, 1);


		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite *player = CCSprite::create("Player.png",CCRectMake(0,0,27,40));

		player->setPosition(ccp(player->getContentSize().width/2,winSize.height/2));
		this->addChild(player);
		bRet = true;

	} while(0);
	this->schedule(schedule_selector(HelloWorld::gameLogic),1.0);
	return bRet;
}

void HelloWorld::ccTouchesEnded(CCSet* touches,CCEvent* event) {

	CCTouch* touch = (CCTouch*)(touches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("Projectile.png",CCRectMake(0,0,20,20));

	projectile->setPosition(ccp(20,winSize.height/2));

	int offX = location.x - projectile->getPosition().x;
	int offY = location.y - projectile->getPosition().y;

	if(offX<=0) {
		return;
	}
	this->addChild(projectile);

	int realX = winSize.width+(projectile->getContentSize().width/2);
	float ratio = (float)offY/(float)offX;

	int realY = (realX*ratio)+projectile->getPosition().y;
	CCPoint realDest = ccp(realX,realY);

	int offRealX = realX -projectile->getPosition().x;
	int offRealY = realY - projectile->getPosition().y;

	float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
	float velocity = 480/1;
	float realMoveDuration = length/velocity;

	projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration,realDest),CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished)),NULL));
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
