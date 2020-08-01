#include "LineSpritePool.hpp"
#include "SpriteManager.h"

USING_NS_CC;

LineSpritePool::LineSpritePool(int initCircleCount, int initSquareCount) : circles(initCircleCount), squares(initSquareCount) {
	for (int count = 0; count < initCircleCount; ++count) {
		circles.pushBack(SPR_MANAGER->getSprite(SpriteManager::SpriteType::circle));
	}
	for (int count = 0; count < initSquareCount; ++count) {
		squares.pushBack(SPR_MANAGER->getSprite(SpriteManager::SpriteType::square));
	}
}

Sprite * LineSpritePool::getCircle(LineInfo::Element circleType) {
	CCASSERT(circleType != LineInfo::Element::none && circleType != LineInfo::Element::green, "circleType must be red, blue or violet!");
	return getSprite(SpriteManager::SpriteType::circle, circleType, circles);
}

Sprite * LineSpritePool::getSquare() {
	return getSprite(SpriteManager::SpriteType::circle, LineInfo::Element::green, squares);
}

Sprite * LineSpritePool::getSprite(SpriteManager::SpriteType spriteType, LineInfo::Element elementType, Vector<Sprite *> &source) {
	Sprite * sprite;
	if (source.empty()) {
		sprite = SPR_MANAGER->getSprite(spriteType);
	}
	else {
		sprite = source.at(0);
		sprite->retain();
		source.erase(0);
	}
	sprite->setColor(SPR_MANAGER->getColor(elementType));
	tempStorage[sprite] = &source;
	return sprite;
}

void LineSpritePool::freeSprite(Sprite * sprite) {
	CCASSERT(tempStorage.count(sprite), "tempStorage must contain sprite!");
	tempStorage[sprite]->pushBack(sprite);
}