#include "PauseGameLayer.h"
#include <algorithm>

USING_NS_CC;

bool PauseGameLayer::init() {
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto pauseItem = MenuItemImage::create(
		"pauseNormal.png",
		"pauseSelected.png",
		CC_CALLBACK_1(PauseGameLayer::menuPauseCallback, this));
	pauseItem->setTag(static_cast<int>(UiButtonName::PAUSE_TAG));
	pauseItem->setPosition(Vec2(origin.x + visibleSize.width - pauseItem->getContentSize().width / 2,
		origin.y + pauseItem->getContentSize().height / 2));

	auto resumeItem = MenuItemImage::create(
		"playNormal.png",
		"playSelected.png",
		CC_CALLBACK_1(PauseGameLayer::menuResumeCallback, this));
	resumeItem->setTag(static_cast<int>(UiButtonName::RESUME_TAG));
	resumeItem->setPosition((origin + visibleSize) * 0.5f);

	auto backItem = MenuItemImage::create(
		"backNormal.png",
		"backSelected.png",
		CC_CALLBACK_1(PauseGameLayer::menuBackCallback, this));
	backItem->setTag(static_cast<int>(UiButtonName::BACK_TAG));
	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width / 2,
		origin.y + backItem->getContentSize().height / 2));

	
	auto skipItem = MenuItemLabel::create(
		Label::create("skip tutorial", "fonts/Marker Felt.ttf", 25),
		CC_CALLBACK_1(PauseGameLayer::menuSkipCallback, this));
	skipItem->setColor(Color3B::BLACK);
	skipItem->setTag(static_cast<int>(UiButtonName::SKIP_TAG));
	skipItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + skipItem->getContentSize().height / 2));

	menu = Menu::create(pauseItem, resumeItem, backItem, skipItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	switchVisibility(true, false);

	return true;
}

void PauseGameLayer::pause() {
	setOpacity(0);
	switchVisibility(true, false);
}

void PauseGameLayer::resume() {
	setOpacity(128);
	switchVisibility(false, true);
}

void PauseGameLayer::setVisible(bool visible) {
	LayerColor::setVisible(visible);
	switchVisibility(false, false);
}

void PauseGameLayer::switchVisibility(bool isVisiblePause, bool isVisiblePlayAndBack) {
	menu->getChildByTag(static_cast<int>(UiButtonName::PAUSE_TAG))->setVisible(isVisiblePause);
	if (menu->getChildByTag(static_cast<int>(UiButtonName::SKIP_TAG)) != nullptr) {
		menu->getChildByTag(static_cast<int>(UiButtonName::SKIP_TAG))->setVisible(isVisiblePause);
	}
	menu->getChildByTag(static_cast<int>(UiButtonName::RESUME_TAG))->setVisible(isVisiblePlayAndBack);
	menu->getChildByTag(static_cast<int>(UiButtonName::BACK_TAG))->setVisible(isVisiblePlayAndBack);
}

void PauseGameLayer::killSkipMenuItem() {
	if (menu->getChildByTag(static_cast<int>(UiButtonName::SKIP_TAG)) != nullptr) {
		menu->removeChildByTag(static_cast<int>(UiButtonName::SKIP_TAG));
	}
}

void PauseGameLayer::menuPauseCallback(Ref * pSender) {
	getParent()->pause();
}

void PauseGameLayer::menuResumeCallback(cocos2d::Ref * pSender) {
	getParent()->resume();
}

void PauseGameLayer::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void PauseGameLayer::menuSkipCallback(Ref * pSender) {
	getParent()->getEventDispatcher()->dispatchCustomEvent("skip");
}