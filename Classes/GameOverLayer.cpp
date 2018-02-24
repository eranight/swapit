#include "GameOverLayer.h"
#include "GameScene.h"
#include <algorithm>

USING_NS_CC;

GameOverLayer * GameOverLayer::create(int goals, float fadeInTime) {
	auto pRet = new (std::nothrow) GameOverLayer();
	if (pRet != nullptr && pRet->init(goals, fadeInTime)) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool GameOverLayer::init(int goals, float fadeInTime) {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 128)))
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = (origin + visibleSize) * 0.5f;

	auto placeForLabel = Sprite::create("labelForScore.png");
	placeForLabel->setPosition(center);
	this->addChild(placeForLabel);

	auto goalsLabel = Label::create(String::createWithFormat("%d", goals)->getCString(), "fonts/Marker Felt.ttf", 25);
	goalsLabel->setColor(Color3B::BLACK);
	goalsLabel->setPosition(center);
	this->addChild(goalsLabel);

	auto backItem = MenuItemImage::create(
		"backNormal.png",
		"backSelected.png",
		CC_CALLBACK_1(GameOverLayer::back, this));

	backItem->setPosition(Vec2(center.x - (placeForLabel->getContentSize().width * 0.5f - backItem->getContentSize().width * 0.5f),
		center.y - (placeForLabel->getContentSize().height * 0.5f + backItem->getContentSize().height * 0.5f)));

	auto replayItem = MenuItemImage::create(
		"replayNormal.png",
		"replaySelected.png",
		CC_CALLBACK_1(GameOverLayer::replay, this));

	replayItem->setPosition(Vec2(center.x + (placeForLabel->getContentSize().width * 0.5f - replayItem->getContentSize().width * 0.5f),
		center.y - (placeForLabel->getContentSize().height * 0.5f + replayItem->getContentSize().height * 0.5f)));

	auto menu = Menu::create(backItem, replayItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto children = this->getChildren();
	auto fadeInAction = FadeIn::create(fadeInTime);
	std::for_each(children.begin(), children.end(), [&fadeInAction](Node * child) -> void {
		child->setOpacity(0);
		child->runAction(fadeInAction->clone());
	});

	return true;
}

void GameOverLayer::replay(Ref * sender) {
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void GameOverLayer::back(Ref * sender) {
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}