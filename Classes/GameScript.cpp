#include "GameScript.h"
#include "GameScene.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"

USING_NS_CC;

GameScript::GameScript(GameScene * gameScene) : AbstractScript(gameScene) {

}

void GameScript::init() {
	gameScene->scoreCountOn = true;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	gameScene->generateLayer->setStartPosition(Vec2(origin.x, (origin + visibleSize).y));
	gameScene->generateLayer->setNextGenerationPosition(Vec2(origin.x, origin.y + 4.0f * SPR_MANAGER->getSpriteSize()));
	gameScene->generateLayer->setFinishPosition(Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize()));

	gameScene->swapLayer->getEventDispatcher()->resumeEventListenersForTarget(gameScene->swapLayer);
}

void GameScript::update(float dt) {

}