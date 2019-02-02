#include "GameScript.h"
#include "GameScene.h"
#include "SwapLayer.h"
#include "LinesLayer.h"

USING_NS_CC;

const float START_TIME = 5.0f;
const float TIME_FACTOR = 0.95f;
const float VELOCITY_FACTOR = 1.0f / TIME_FACTOR;
const float SWAP_LAYER_FACTOR = 1.5f;

GameScript::GameScript(GameScene * gameScene) : AbstractScript(gameScene), score(0), gameOver(false), generateNextLineAction(nullptr) {

}

void GameScript::init() {
	AbstractScript::init();
	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	startGeneratedLinePosition = Vec2(origin.x, (origin + visibleSize).y);
	finishGeneratedLinePosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());
	nextGeneratedLinePosition = Vec2(origin.x, origin.y + 2 * SPR_MANAGER->getSpriteSize());

	timerAction = DelayTime::create(START_TIME);
	timerAction->retain();
	callRecalculateAction = CallFunc::create(CC_CALLBACK_0(GameScript::generateNextLine, this));
	callRecalculateAction->retain();

	scoreLabel = Label::create(String::createWithFormat("%d", score)->getCString(), "fonts/Marker Felt.ttf", 25);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(origin + Vec2(visibleSize.width * 0.5f, visibleSize.height - 26.0f));
	gameScene->addChild(scoreLabel);

	needUpdateVelocity = false;

	generateLayerVelocity = visibleSize.height / START_TIME;
	generateLayer = gameScene->getGenerateLayer();
	generateLayer->setVelocity(generateLayerVelocity);
	generateLayer->setStartPosition(startGeneratedLinePosition);
	generateLayer->setFinishPosition(finishGeneratedLinePosition);
	//lineBuilder = &generateLayer->getLineBuilder();
	lineBuilder->setProbabilities(100, 0, 100, 70);

	swapLayerVelocity = SWAP_LAYER_FACTOR * visibleSize.width / START_TIME;
	swapLayer = gameScene->getSwapLayer();
	swapLayer->setVelocity(swapLayerVelocity);

	generateNextLine();
}

void GameScript::release() {
	timerAction->release();
	callRecalculateAction->release();
}

void GameScript::update(float dt) {
	if (gameOver) {
		gameScene->invokeGameOver(score);
	}
}

void GameScript::pause() {
	
}

void GameScript::resume() {

}

bool GameScript::collide(LineInfo::Element elemA, LineInfo::Element elemB) {
	if (elemA == elemB) {
		++score;
		scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
		if (score % 10 == 0) {
			setVelocity(generateLayerVelocity * VELOCITY_FACTOR, swapLayerVelocity * VELOCITY_FACTOR);
		}
		return true;
	}
	else {
		if (elemA != LineInfo::Element::violet) {
			gameOver = true;
		}
		return false;
	}
}

void GameScript::setVelocity(float generateLayerVelocity, float swapLayerVelocity) {
	this->generateLayerVelocity = generateLayerVelocity;
	this->swapLayerVelocity = swapLayerVelocity;
	needUpdateVelocity = true;
}

void GameScript::generateNextLine() {
	if (needUpdateVelocity) {
		generateLayer->setVelocity(generateLayerVelocity);
		swapLayer->setVelocity(swapLayerVelocity);
		float timer = timerAction->getDuration();
		timerAction->release();
		timerAction = DelayTime::create(timer * TIME_FACTOR);
		timerAction->retain();
		needUpdateVelocity = false;
		CCLOG("%.3f %.3f", generateLayerVelocity, swapLayerVelocity);
	}
	generateLayer->generateNewLine();
	generateNextLineAction = Sequence::createWithTwoActions(timerAction->clone(), callRecalculateAction->clone());
	gameScene->runAction(generateNextLineAction);
}