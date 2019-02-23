#include "LineSprites.h"

USING_NS_CC;

LineSprites * LineSprites::create(LineInfo & line) {
	LineSprites * pRet = new (std::nothrow) LineSprites();
	if (pRet && pRet->init(line)) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool LineSprites::init(LineInfo & line) {
	if (!Node::init()) {
		return false;
	}

	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	leftSprite = initSpriteLike(line.getLeft(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::left), 0));
	centerSprite = initSpriteLike(line.getMiddle(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::center), 0));
	rightSprite = initSpriteLike(line.getRight(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::right), 0));

	return true;
}

Sprite * LineSprites::initSpriteLike(LineInfo::Element element, const Vec2 & position) {
	Sprite * sprite = nullptr;
	if (element != LineInfo::Element::none) {
		sprite = SPR_MANAGER->getSprite(element);
		sprite->setPosition(position);
		this->addChild(sprite);
	}
	return sprite;
}

void LineSprites::destroyLeftSprite() {
	if (leftSprite != nullptr) {
		this->removeChild(leftSprite);
		leftSprite = nullptr;
	}
}

void LineSprites::destroyCenterSprite() {
	if (centerSprite != nullptr) {
		this->removeChild(centerSprite);
		centerSprite = nullptr;
	}
}

void LineSprites::destroyRightSprite() {
	if (rightSprite != nullptr) {
		this->removeChild(rightSprite);
		rightSprite = nullptr;
	}
}
