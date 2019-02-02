#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "SwapLayer.h"
#include "LinesLayer.h"
#include "GameOverLayer.h"
#include "UserInterfaceGameLayer.h"
#include "GameScript.h"
#include "TutorialScript.h"

USING_NS_CC;

GameScene::~GameScene()
{
	SpriteManager::getInstance()->release();
	for (auto script : scripts) {
		if (script->isInit()) {
			script->release();
		}
		delete script;
	}
	scripts.clear();
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

	lineForCollisionDetect = nullptr;

	generateLayer = LinesLayer::create();
	this->addChild(generateLayer);

	swapLayer = SwapLayer::create();
	this->addChild(swapLayer);

	/*goalsLabel = Label::create(String::createWithFormat("%d", goals)->getCString(), "fonts/Marker Felt.ttf", 25);
	goalsLabel->setColor(Color3B::BLACK);
	goalsLabel->setPosition(origin + Vec2(visibleSize.width * 0.5f, visibleSize.height - 26.0f));
	this->addChild(goalsLabel);*/

	scripts.push_back(new TutorialScript(this));
	scripts.push_back(new GameScript(this));
	scripts[0]->init();
	currentScriptPointer = scripts.begin();

	userInterfaceGameLayer = UserInterfaceGameLayer::create();
	this->addChild(userInterfaceGameLayer);

	getEventDispatcher()->addCustomEventListener("skip", CC_CALLBACK_1(GameScene::skipScriptEvent, this));

	scheduleUpdate();

	return true;
}

void GameScene::update(float dt) {
	lineForCollisionDetect = generateLayer->getFirstLineAbove(swapLayer->getLinePosition());
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

	if ((*currentScriptPointer)->isFinished()) {
		toNextScript();
	}

	(*currentScriptPointer)->update(dt);

	/*if (scoreCountOn && prevGoals != goals) {
	goalsLabel->setString(String::createWithFormat("%d", goals)->getCString());
	if (goals >= goalLevel) {
	velocity *= 1.1;
	generateLayer->setVelocity(velocity);
	swapLayer->setVelocity(velocity);
	goalLevel *= 1.5;
	}
	}*/
}

void GameScene::pause() {
	Layer::pause();
	swapLayer->pause();
	generateLayer->pause();
	userInterfaceGameLayer->resume();
	(*currentScriptPointer)->pause();
}

void GameScene::resume() {
	Layer::resume();
	swapLayer->resume();
	generateLayer->resume();
	userInterfaceGameLayer->pause();
	(*currentScriptPointer)->resume();
}

void GameScene::collisionUpdate() {
	bool needToDestroy = false;
	const Sprite * sprite = lineForCollisionDetect->getLeftSprite();
	if (sprite != nullptr) {
		checkCollision(sprite, swapLayer->getRedBallSprite(), needToDestroy);
		if (needToDestroy) {
			lineForCollisionDetect->destroyLeftSprite();
		}
		else {
			checkCollision(sprite, swapLayer->getBlueBallSprite(), needToDestroy);
			if (needToDestroy) {
				lineForCollisionDetect->destroyLeftSprite();
			}
		}
	}
	sprite = lineForCollisionDetect->getCenterSprite();
	if (sprite != nullptr) {
		if (sprite->getColor() == SPR_MANAGER->getColor(LineInfo::Element::violet)) {
			if (swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
				checkCollision(sprite, swapLayer->getVioletBallSprite(), needToDestroy);
				if (needToDestroy) {
					lineForCollisionDetect->destroyCenterSprite();
				}
			}
		}
		else {
			checkCollision(sprite, swapLayer->getRedBallSprite(), needToDestroy);
			checkCollision(sprite, swapLayer->getBlueBallSprite(), needToDestroy);
		}
	}
	sprite = lineForCollisionDetect->getRightSprite();
	if (sprite != nullptr) {
		checkCollision(sprite, swapLayer->getRedBallSprite(), needToDestroy);
		if (needToDestroy) {
			lineForCollisionDetect->destroyRightSprite();
		}
		else {
			checkCollision(sprite, swapLayer->getBlueBallSprite(), needToDestroy);
			if (needToDestroy) {
				lineForCollisionDetect->destroyRightSprite();
			}
		}
	}
}

static const float PERCENT = 0.8f;

//spriteB is always a ball from SwapLayer, but spriteA is might be a square
void GameScene::checkCollision(const Sprite * spriteA, const cocos2d::Sprite * spriteB, bool & needToDestroy) {
	Rect bbA = spriteA->getBoundingBox();
	Vec2 centerA = convertToNodeSpace(lineForCollisionDetect->convertToWorldSpace(Vec2(bbA.getMidX(), bbA.getMidY())));
	Rect bbB = spriteB->getBoundingBox();
	Vec2 centerB = convertToNodeSpace(swapLayer->convertToWorldSpace(Vec2(bbB.getMidX(), bbB.getMidY())));
	float diameter = SPR_MANAGER->getSpriteSize() * PERCENT;
	float radius = diameter * 0.5f;
	if (spriteA->getColor() != SPR_MANAGER->getColor(LineInfo::Element::green)) {
		if ((centerA - centerB).length() <= diameter) {
			needToDestroy = (*currentScriptPointer)->collide(SPR_MANAGER->getKeyByColor(spriteA->getColor()), SPR_MANAGER->getKeyByColor(spriteB->getColor()));
		}
	}
	else {
		Rect collisionRect = Rect(centerA.x - radius, centerA.y - radius, diameter, diameter);
		if (collisionRect.intersectsCircle(centerB, radius)) {
			needToDestroy = (*currentScriptPointer)->collide(SPR_MANAGER->getKeyByColor(spriteA->getColor()), SPR_MANAGER->getKeyByColor(spriteB->getColor()));
		}
	}
}

void GameScene::toNextScript() {
	(*currentScriptPointer)->release();
	currentScriptPointer = scripts.erase(currentScriptPointer);
	(*currentScriptPointer)->init();
}

void GameScene::skipScriptEvent(EventCustom * event) {
	CCLOG("skip event");
	toNextScript();
	userInterfaceGameLayer->killSkipMenuItem();
}

void GameScene::invokeGameOver(int score) {
	swapLayer->stop();
	generateLayer->stop();
	userInterfaceGameLayer->setVisible(false);
	unscheduleUpdate();
	auto gameOverLayer = GameOverLayer::create(score, 2.0f);
	this->addChild(gameOverLayer);
}