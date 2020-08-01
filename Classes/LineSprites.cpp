#include "LineSprites.h"

USING_NS_CC;

LineSprites * LineSprites::create(LineInfo & line, LineSpritePool * factory) {
	LineSprites * pRet = new (std::nothrow) LineSprites();
	if (pRet && pRet->init(line, factory)) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool LineSprites::init(LineInfo & line, LineSpritePool * factory) {
	if (!Node::init()) {
		return false;
	}

	this->lineInfo = line;

	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	this->factory = factory;

	leftSprite = initSpriteLike(line.getLeft(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::left), 0));
	centerSprite = initSpriteLike(line.getMiddle(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::center), 0));
	rightSprite = initSpriteLike(line.getRight(), Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::right), 0));

	return true;
}

Sprite * LineSprites::initSpriteLike(LineInfo::Element element, const Vec2 & position) {
	Sprite * sprite = nullptr;
	if (element != LineInfo::Element::none) {
		sprite = element != LineInfo::Element::green ? factory->getCircle(element) : factory->getSquare();
		sprite->setPosition(position);
		this->addChild(sprite);
	}
	return sprite;
}

void LineSprites::destroyLeftSprite() {
	if (leftSprite != nullptr) {
		this->removeChild(leftSprite);
		factory->freeSprite(leftSprite);
		leftSprite = nullptr;
		lineInfo = LineInfo(LineInfo::Element::none, lineInfo.getMiddle(), lineInfo.getRight());
	}
}

void LineSprites::destroyCenterSprite() {
	if (centerSprite != nullptr) {
		this->removeChild(centerSprite);
		factory->freeSprite(centerSprite);
		centerSprite = nullptr;
		lineInfo = LineInfo(lineInfo.getLeft(), LineInfo::Element::none, lineInfo.getRight());
	}
}

void LineSprites::destroyRightSprite() {
	if (rightSprite != nullptr) {
		this->removeChild(rightSprite);
		factory->freeSprite(rightSprite);
		rightSprite = nullptr;
		lineInfo = LineInfo(lineInfo.getLeft(), lineInfo.getMiddle(), LineInfo::Element::none);
	}
}
