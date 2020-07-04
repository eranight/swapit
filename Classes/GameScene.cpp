#include "GameScene.hpp"
#include "SceneFactory.hpp"
#include "SpriteManager.h"

USING_NS_CC;

GameScene::GameScene() {

}

GameScene::~GameScene() {
	SPR_MANAGER->release();
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	SPR_MANAGER->retain();

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	swapLayer = SwapLayer::create();
	swapLayer->setVelocity(visibleSize.width * 0.3f);
	this->addChild(swapLayer);

	linesLayer = LinesLayer::create({ Vec2::ZERO, Vec2::ZERO, 0.0f }, nullptr);
	this->addChild(linesLayer);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
			Director::getInstance()->replaceScene(SceneFactory::createMenuScene());
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}