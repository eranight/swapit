#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

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

	auto spriteBall = Sprite::create("ball.png");
	float ballSize = spriteBall->getContentSize().height * Director::getInstance()->getContentScaleFactor();

	auto redball = Sprite::createWithTexture(spriteBall->getTexture());
	redball->setAnchorPoint(Vec2(0.0f, 0.0f));
	redball->setColor(Color3B::RED);
	redball->setPosition(Vec2(ballSize, ballSize));
	this->addChild(redball);

	auto blueball = Sprite::createWithTexture(spriteBall->getTexture());
	blueball->setAnchorPoint(Vec2(0.0f, 0.0f));
	blueball->setColor(Color3B::BLUE);
	blueball->setPosition(Vec2((origin + visibleSize).x - ballSize * 2.0f, ballSize));
	this->addChild(blueball);

    return true;
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
