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

	gameOver = false;
	goals = 0;

	SpriteManager::getInstance()->retain(); //initialization of the SpriteManager instance

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	velocity = visibleSize.height / 7.0f;

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
	generateLayer->start(velocity);
	this->addChild(generateLayer);

	swapLayer = SwapLayer::create();
	swapLayer->setVelocity(velocity);
	this->addChild(swapLayer);

	goalsLabel = Label::create(String::createWithFormat("%d", goals)->getCString(), "fonts/Marker Felt.ttf", 25);
	goalsLabel->setColor(Color3B::BLACK);
	goalsLabel->setPosition(origin + Vec2(visibleSize.width * 0.5f, visibleSize.height - 26.0f));
	this->addChild(goalsLabel);

	lineForCollisionDetect = nullptr;

	scheduleUpdate();

    return true;
}

void GameScene::update(float dt) {
	int prevGoals = goals;
	if (lineForCollisionDetect == nullptr) {
		lineForCollisionDetect = generateLayer->getFirstHighLine(swapLayer->getLinePosition());
	}
	if (lineForCollisionDetect != nullptr) {
		if (lineForCollisionDetect->getPosition().y < swapLayer->getLinePosition() + SPR_MANAGER->getSpriteSize()) {
			if (lineForCollisionDetect->getPosition().y < swapLayer->getLinePosition() - SPR_MANAGER->getSpriteSize()) {
				lineForCollisionDetect = nullptr;
			}
			else {
				collisionUpdate();
				if (lineForCollisionDetect->getLeftSprite() == nullptr &&
					lineForCollisionDetect->getCenterSprite() == nullptr &&
					lineForCollisionDetect->getRightSprite() == nullptr) {
					lineForCollisionDetect = nullptr;
				}
			}
		}
	}
	if (prevGoals != goals) {
		goalsLabel->setString(String::createWithFormat("%d", goals)->getCString());
		if (goals % 5 == 0) {
			velocity *= 1.1;
			generateLayer->setVelocity(velocity);
			swapLayer->setVelocity(velocity);
			CCLOG("new velocity: %.3f", velocity);
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

void GameScene::collisionUpdate() {
	int prevGoals = goals;
	const Sprite * sprite = lineForCollisionDetect->getLeftSprite();
	if (sprite != nullptr) {
		checkCollision(sprite, swapLayer->getRedBallSprite());
		checkCollision(sprite, swapLayer->getBlueBallSprite());
		if (!gameOver && goals != prevGoals) {
			lineForCollisionDetect->destroyLeftSprite();
		}
	}
	prevGoals = goals;
	sprite = lineForCollisionDetect->getCenterSprite();
	if (sprite != nullptr) {
		if (swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
			checkCollision(sprite, swapLayer->getVioletBallSprite());
			if (!gameOver && goals != prevGoals) {
				lineForCollisionDetect->destroyCenterSprite();
			}
		}
		else {
			checkCollision(sprite, swapLayer->getRedBallSprite());
			checkCollision(sprite, swapLayer->getBlueBallSprite());
		}
	}
	prevGoals = goals;
	sprite = lineForCollisionDetect->getRightSprite();
	if (sprite != nullptr) {
		checkCollision(sprite, swapLayer->getRedBallSprite());
		checkCollision(sprite, swapLayer->getBlueBallSprite());
		if (!gameOver && goals != prevGoals) {
			lineForCollisionDetect->destroyRightSprite();
		}
	}
}

static const float PERCENT = 0.8f;

//spriteB is always a ball from SwapLayer, but spriteA is might be a square
void GameScene::checkCollision(const Sprite * spriteA, const cocos2d::Sprite * spriteB) {
	Rect bbA = spriteA->getBoundingBox();
	Vec2 centerA = convertToNodeSpace(lineForCollisionDetect->convertToWorldSpace(Vec2(bbA.getMidX(), bbA.getMidY())));
	Rect bbB = spriteB->getBoundingBox();
	Vec2 centerB = convertToNodeSpace(swapLayer->convertToWorldSpace(Vec2(bbB.getMidX(), bbB.getMidY())));
	float diameter = SPR_MANAGER->getSpriteSize() * PERCENT;
	float radius = diameter * 0.5f;
	if (spriteA->getColor() != SPR_MANAGER->getColor(LineInfo::Element::green)) {
		if ((centerA - centerB).length() <= diameter) {
			if (spriteA->getColor() == spriteB->getColor()) {
				++goals;
			}
			else {
				gameOver = true;
			}
		}
	}
	else {
		Rect collisionRect = Rect(centerA.x - radius, centerA.y - radius, diameter, diameter);
		if (collisionRect.intersectsCircle(centerB, radius)) {
			gameOver = true;
		}
	}
}