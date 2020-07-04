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
	Vec2 center = (origin + visibleSize) * 0.5f;

	swapLayer = SwapLayer::create();
	swapLayer->setVelocity(visibleSize.width * 0.3f);
	this->addChild(swapLayer);

	linesLayer = LinesLayer::create({ Vec2::ZERO, Vec2::ZERO, 0.0f }, nullptr);
	this->addChild(linesLayer);

	gameOverLayer = LayerColor::create(Color4B(Color3B::WHITE, 50));
	this->addChild(gameOverLayer);
	
	auto scoreCover = Sprite::create("labelForScore.png");
	scoreCover->setPosition(center);
	gameOverLayer->addChild(scoreCover);
	
	scoreLabel = Label::create("kek", Configuration::getInstance()->getValue("font").asString(), Configuration::getInstance()->getValue("fontSize").asFloat());
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(center);
	gameOverLayer->addChild(scoreLabel);
	auto backItem = MenuItemImage::create("backNormal.png", "backSelected.png", [](Ref * ref) {
		Director::getInstance()->replaceScene(SceneFactory::createMenuScene());
	});
	backItem->setPosition(Vec2(center.x - (scoreCover->getContentSize().width * 0.5f - backItem->getContentSize().width * 0.5f),
		center.y - (scoreCover->getContentSize().height * 0.5f + backItem->getContentSize().height * 0.5f)));

	auto replayItem = MenuItemImage::create("replayNormal.png", "replaySelected.png", [](Ref * ref) {
		Director::getInstance()->replaceScene(SceneFactory::createGameScene());
	});
	replayItem->setPosition(Vec2(center.x + (scoreCover->getContentSize().width * 0.5f - replayItem->getContentSize().width * 0.5f),
		center.y - (scoreCover->getContentSize().height * 0.5f + replayItem->getContentSize().height * 0.5f)));

	menu = Menu::create(backItem, replayItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	gameOverLayer->addChild(menu, 1);

	return true;
}

void GameScene::onEnter() {
	Scene::onEnter();
	menu->setEnabled(false);
	gameOverLayer->setVisible(false);
	auto children = gameOverLayer->getChildren();
	std::for_each(children.begin(), children.end(), [](Node * child) { child->setOpacity(0); });
}

void GameScene::gameOver() {
	swapLayer->block();
	linesLayer->block();
	gameOverLayer->setVisible(true);
	auto fadeInAction = FadeIn::create(Configuration::getInstance()->getValue("fadeInTime").asFloat());
	auto children = gameOverLayer->getChildren();
	std::for_each(children.begin(), children.end(), [fadeInAction](Node * child) { child->runAction(fadeInAction->clone()); });
	this->runAction(Sequence::createWithTwoActions(fadeInAction, CallFunc::create([this]() { menu->setEnabled(true); })));
}