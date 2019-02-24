#include "MainMenuLayer.hpp"
#include "MainMenuScene.hpp"
#include "SpriteManager.h"
#include "SceneFactory.hpp"

USING_NS_CC;

bool MainMenuLayer::init() {
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = (origin + visibleSize) * 0.5f;

	auto playItem = MenuItemImage::create("playNormal.png", "playSelected.png", [this](Ref * ref) -> void {
		Director::getInstance()->replaceScene(SceneFactory::createTutorialScene());
	});
	playItem->setPosition(Vec2(center.x, center.y + SPR_MANAGER->getSpriteSize() * 1.5f));

	auto recordItem = MenuItemImage::create("recordNormal.png", "recordSelected.png", [this](Ref * ref) -> void { this->switchLayer(LayerType::RECORD); });
	recordItem->setPosition(Vec2(center.x - SPR_MANAGER->getSpriteSize(), center.y));

	auto authorItem = MenuItemImage::create("authorsNormal.png", "authorsSelected.png", [this](Ref * ref) -> void { this->switchLayer(LayerType::AUTHORS); });
	authorItem->setPosition(Vec2(center.x + SPR_MANAGER->getSpriteSize(), center.y));

	auto closeItem = MenuItemImage::create("closeNormal.png", "closeSelected.png", [this](Ref * ref) -> void { this->switchLayer(LayerType::AUTHORS); });
	closeItem->setPosition(Vec2(center.x, center.y - SPR_MANAGER->getSpriteSize()));

	menu = Menu::create(playItem, recordItem, authorItem, closeItem, nullptr);
	menu->setPosition(origin);
	this->addChild(menu);

	return true;
}

void MainMenuLayer::switchLayer(const LayerType & layerType) {
	dynamic_cast<MainMenuScene *>(Director::getInstance()->getRunningScene())->switchLayer(layerType);
}