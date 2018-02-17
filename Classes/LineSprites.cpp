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
	float halfBallSize = SPR_MANAGER->getSpriteSize() * 0.5f;

	leftSprite = initSpriteLike(line.getLeft(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::left), halfBallSize));
	centerSprite = initSpriteLike(line.getMiddle(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::center), halfBallSize));
	rightSprite = initSpriteLike(line.getRight(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::right), halfBallSize));

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
