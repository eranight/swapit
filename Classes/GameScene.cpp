#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"

USING_NS_CC;

GameScene::~GameScene()
{
	SpriteManager::getInstance()->release();
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
	auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
	if (!LayerColor::initWithColor(Color4B::WHITE))
	{
		return false;
	}

	SpriteManager::getInstance()->retain(); //initialization of the SpriteManager instance

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	generateLayer = GenerateLayer::create();
	this->addChild(generateLayer);

	swapLayer = SwapLayer::create();
	this->addChild(swapLayer);

	lineForCollisionDetect = nullptr;

	//scheduleUpdate();

    return true;
}

void GameScene::update(float dt) {
	/*if (lineForCollisionDetect == nullptr) {
		LineSprites * lineSprites = generateLayer->getFirstHighLine();
		if (std::abs(lineSprites->getPosition().y - swapLayer->getLinePosition()) < SPR_MANAGER->getSpriteSize()) {
			lineForCollisionDetect = lineSprites;
			CCLOG("");
		}
	}*/
	if (lineForCollisionDetect != nullptr) {

		if (swapLayer->getLinePosition() - lineForCollisionDetect->getPosition().y > SPR_MANAGER->getSpriteSize()) {
			lineForCollisionDetect = nullptr;
			CCLOG("");
		}
	}
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
