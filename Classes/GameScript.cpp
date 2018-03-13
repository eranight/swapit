#include "GameScript.h"
#include "GameScene.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"

USING_NS_CC;

const float START_TIME = 6.0f;
const float START_TIME_HORIZONTAL_FACTOR = 0.75f;
const float VELOCITY_FACTOR = 1.09f;

GameScript::GameScript(GameScene * gameScene) : AbstractScript(gameScene), score(0), gameOver(false) {

}

void GameScript::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	scoreLabel = Label::create(String::createWithFormat("%d", score)->getCString(), "fonts/Marker Felt.ttf", 25);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(origin + Vec2(visibleSize.width * 0.5f, visibleSize.height - 26.0f));
	gameScene->addChild(scoreLabel);

	float velocity = visibleSize.height / START_TIME;
	gameScene->getGenerateLayer()->setVelocity(velocity);

	velocity = visibleSize.width / (START_TIME * START_TIME_HORIZONTAL_FACTOR);
	gameScene->getSwapLayer()->setVelocity(velocity);
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