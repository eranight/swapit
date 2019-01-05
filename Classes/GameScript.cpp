#include "GameScript.h"
#include "GameScene.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"

USING_NS_CC;

const float START_TIME = 6.0f;
const float START_TIME_HORIZONTAL_FACTOR = 0.75f;
const float VELOCITY_FACTOR = 1.09f;

GameScript::GameScript(GameScene * gameScene) : AbstractScript(gameScene), score(0), gameOver(false), generateNextLineAction(nullptr) {

}

void GameScript::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	startGeneratedLinePosition = Vec2(origin.x, (origin + visibleSize).y);
	finishGeneratedLinePosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());
	nextGeneratedLinePosition = Vec2(origin.x, origin.y + SPR_MANAGER->getSpriteSize());

	scoreLabel = Label::create(String::createWithFormat("%d", score)->getCString(), "fonts/Marker Felt.ttf", 25);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(origin + Vec2(visibleSize.width * 0.5f, visibleSize.height - 26.0f));
	gameScene->addChild(scoreLabel);

	velocity = visibleSize.height / START_TIME;
	generateLayer = gameScene->getGenerateLayer();
	generateLayer->setVelocity(velocity);
	generateLayer->setStartPosition(startGeneratedLinePosition);
	generateLayer->setFinishPosition(finishGeneratedLinePosition);
	lineBuilder = &generateLayer->getLineBuilder();
	lineBuilder->setProbabilities(100, 0, 50, 70);

	swapLayer = gameScene->getSwapLayer();
	swapLayer->setVelocity(visibleSize.width / (START_TIME * START_TIME_HORIZONTAL_FACTOR));

	recalculateNextLineAction();
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
		return true;
	}
	else {
		if (elemA != LineInfo::Element::violet) {
			gameOver = true;
		}
		return false;
	}
}

void GameScript::recalculateNextLineAction() {
	gameScene->stopAction(generateNextLineAction);
	auto line = generateLayer->getFirstLineAbove(nextGeneratedLinePosition.y);
	float startY = line != nullptr ? line->getPosition().y : startGeneratedLinePosition.y;
	float nextLineTimer = abs(startY - nextGeneratedLinePosition.y) / velocity;
	generateNextLineAction = Sequence::create(DelayTime::create(nextLineTimer), CallFunc::create(CC_CALLBACK_0(GenerateLayer::generateNewLine, generateLayer)),
		CallFunc::create(CC_CALLBACK_0(GameScript::recalculateNextLineAction, this)), nullptr);
	gameScene->runAction(generateNextLineAction);
}