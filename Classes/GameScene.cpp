#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"
#include "GameOverLayer.h"
#include "PauseGameLayer.h"

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

	if (script != nullptr) {
		script->update(dt);
	}

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

TutorialScript::TutorialScript(GameScene * gs) : SwapItSctipt(gs), promts({ "tap to swap", "allow the square", "tap and hold" }) {}

void TutorialScript::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	promtIsActive = false;
	promtNumber = 0;
	showPromtPosition = origin.y + visibleSize.height * 0.75f;

	promtLabel = Label::create(promts[promtNumber], "fonts/Marker Felt.ttf", 25);
	promtLabel->setColor(Color3B::BLACK);
	promtLabel->setPosition((origin + visibleSize) * 0.5f);
	promtLabel->setVisible(false);
	gameScene->addChild(promtLabel);

	gameScene->generateLayer->getLineBuilder().setStartConfiguration(LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue));
	gameScene->generateLayer->getLineBuilder().setProbabilities(100, 0, 100, 100);
	gameScene->velocity = visibleSize.height / 6.0f;
	gameScene->generateLayer->setStartPosition(Vec2(origin.x, (origin + visibleSize).y));
	gameScene->generateLayer->setNextGenerationPosition(Vec2(origin.x, origin.y + SPR_MANAGER->getSpriteSize()));
	gameScene->generateLayer->setFinishPosition(Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize()));
	gameScene->generateLayer->setVelocity(gameScene->velocity);
	gameScene->generateLayer->start();

	gameScene->swapLayer->setVelocity(gameScene->velocity);
	gameScene->swapLayer->getEventDispatcher()->pauseEventListenersForTarget(gameScene->swapLayer);

	line = nullptr;
}

void TutorialScript::update(float dt) {
	if (line == nullptr) {
		line = gameScene->generateLayer->getFirstHighLine(showPromtPosition);
	}
	else if (line != nullptr && gameScene->convertToNodeSpace(gameScene->generateLayer->convertToWorldSpace(line->getPosition())).y < showPromtPosition) {
		promtIsActive = true;
		promtLabel->setVisible(true);
		gameScene->generateLayer->pause();
		gameScene->swapLayer->getEventDispatcher()->resumeEventListenersForTarget(gameScene->swapLayer);
	}
	if (promtLabel->isVisible()) {
		bool checkCondition = false;
		if (promtNumber == 0 || promtNumber == 1) {
			const Sprite * sprite = line->getLeftSprite() != nullptr ? line->getLeftSprite() : line->getRightSprite();
			const Sprite * compareSprite = sprite->getColor() == SPR_MANAGER->getColor(LineInfo::Element::red) ? gameScene->swapLayer->getRedBallSprite() : gameScene->swapLayer->getBlueBallSprite();
			float x1 = gameScene->convertToNodeSpace(line->convertToWorldSpace(sprite->getPosition())).x;
			float x2 = gameScene->convertToNodeSpace(gameScene->swapLayer->convertToWorldSpace(compareSprite->getPosition())).x;
			if (gameScene->swapLayer->getState() == SwapLayer::BallState::StandOnOppositeSides && abs(x1 - x2) <= 0.1f) {
				checkCondition = true;
			}
		}
		else if (promtNumber == 2) {
			if (gameScene->swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
				checkCondition = true;
			}
		}
		if (checkCondition) {
			promtLabel->setVisible(false);
			gameScene->generateLayer->resume();
			gameScene->swapLayer->getEventDispatcher()->pauseEventListenersForTarget(gameScene->swapLayer);
			if (++promtNumber < promts.size()) {
				promtLabel->setString(promts[promtNumber]);
				if (promtNumber == 1) {
					gameScene->generateLayer->getLineBuilder().setProbabilities(100, 100, 100, 100);
				}
				else {
					gameScene->generateLayer->getLineBuilder().setProbabilities(0, 0, 0, 0);
				}
				promtIsActive = false;
				line = nullptr;
			}
			else {
				gameScene->removeChild(promtLabel);
				promtLabel = nullptr;
				auto script = new GameScript(gameScene);
				script->init();
				gameScene->script = script;
			}
		}
	}
}

GameScript::GameScript(GameScene * gc) : SwapItSctipt(gc) {}

void GameScript::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	gameScene->generateLayer->setStartPosition(Vec2(origin.x, (origin + visibleSize).y));
	gameScene->generateLayer->setNextGenerationPosition(Vec2(origin.x, origin.y + 4.0f * SPR_MANAGER->getSpriteSize()));
	gameScene->generateLayer->setFinishPosition(Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize()));

	gameScene->swapLayer->getEventDispatcher()->resumeEventListenersForTarget(gameScene->swapLayer);
}

void GameScript::update(float dt) {

}