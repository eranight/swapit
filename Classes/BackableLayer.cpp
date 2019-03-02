#include "BackableLayer.hpp"

USING_NS_CC;

Layer * BackableLayer::create(Node * centerNode) {
	BackableLayer * pRef = new (std::nothrow) BackableLayer();
	if (pRef != nullptr && pRef->init(centerNode)) {
		pRef->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRef);
	}
	return pRef;
}

bool BackableLayer::init(Node * centerNode) {
	if (!SwitchableLayer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	centerNode->setPosition(origin + visibleSize * 0.5f);
	this->addChild(centerNode);
	
	auto backItem = MenuItemImage::create("backNormal.png", "backSelected.png", CC_CALLBACK_1(BackableLayer::back, this));
	backItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backItem->setPosition(origin);

	auto menu = Menu::create(backItem, nullptr);
	menu->setPosition(origin);
	this->addChild(menu);

	return true;
}

void BackableLayer::back(Ref * pSender) {
	switchLayer(LayerType::MENU);
}