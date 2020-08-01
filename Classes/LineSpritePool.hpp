#pragma once

#include <map>
#include "cocos2d.h"
#include "SpriteManager.h"

class LineSpritePool {
public:
	LineSpritePool(int initCircleCount, int initSquareCount);
public:
	cocos2d::Sprite * getCircle(LineInfo::Element circleType);
	cocos2d::Sprite * getSquare();
	void freeSprite(cocos2d::Sprite * sprite);
private:
	cocos2d::Sprite * getSprite(SpriteManager::SpriteType spriteType, LineInfo::Element elementType, cocos2d::Vector<cocos2d::Sprite *> &source);
private:
	cocos2d::Vector<cocos2d::Sprite *> circles;
	cocos2d::Vector<cocos2d::Sprite *> squares;
	std::map<cocos2d::Sprite *, cocos2d::Vector<cocos2d::Sprite *> *> tempStorage;
};