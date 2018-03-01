#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"
#include "GameOverLayer.h"
#include "PauseGameLayer.h"
#include "GameScript.h"
#include "TutorialScript.h"

USING_NS_CC;

GameScene::~GameScene()
{
	SpriteManager::getInstance()->release();
	if (script != nullptr) {
		delete script;
	}
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

	scoreCountOn = false;
	gameOver = false;
	goals = 0;

	SpriteManager::getInstance()->retain(); //initialization of the SpriteManager instance

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	goalLevel = 5;
	lineForCollisionDetect = nullptr;

	generateLayer = GenerateLayer::create();
	this->addChild(generateLayer);

	swapLayer = SwapLayer::create();
	this->addChild(swapLayer);

	pauseGameLayer = PauseGameLayer::create();
	this->addChild(pauseGameLayer);

	goalsLabel = Label::create(String::createWithFormat("%d", goals)->getCString(), "fonts/Marker Felt.ttf", 25);
	goalsLabel->setColor(Color3B::BLACK);
	goalsLabel->setPosition(origin + Vec2(visibleSize.width * 0.5f, visibleSize.height - 26.0f));
	this->addChild(goalsLabel);

	script = new TutorialScript(this);
	script->init();

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

	if (script != nullptr) {
		script->update(dt);
	}

	if (scoreCountOn && prevGoals != goals) {
		goalsLabel->setString(String::createWithFormat("%d", goals)->getCString());
		if (goals >= goalLevel) {
			velocity *= 1.1;
			generateLayer->setVelocity(velocity);
			swapLayer->setVelocity(velocity);
			goalLevel *= 1.5;
		}
	}

	if (gameOver) {
		swapLayer->stop();
		generateLayer->stop();
		pauseGameLayer->setVisible(false);
		unscheduleUpdate();
		auto gameOverLayer = GameOverLayer::create(goals, 2.0f);
		this->addChild(gameOverLayer);
	}
}

void GameScene::pause() {
	Layer::pause();
	swapLayer->pause();
	generateLayer->pause();
	pauseGameLayer->resume();
}

void GameScene::resume() {
	Layer::resume();
	swapLayer->resume();
	generateLayer->resume();
	pauseGameLayer->pause();
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
		if (sprite->getColor() == SPR_MANAGER->getColor(LineInfo::Element::violet)) {
			if (swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
				checkCollision(sprite, swapLayer->getVioletBallSprite());
				if (!gameOver && goals != prevGoals) {
					lineForCollisionDetect->destroyCenterSprite();
				}
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