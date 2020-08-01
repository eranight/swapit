#ifndef __LINE_SPRITES_H__
#define __LINE_SPRITES_H__

#include "cocos2d.h"
#include "LineInfo.h"
#include "SpriteManager.h"
#include "LineSpritePool.hpp"

class LineSprites : public cocos2d::Node
{
public:

	static LineSprites * create(LineInfo &, LineSpritePool *);
	bool init(LineInfo &, LineSpritePool *);

	//getters
	const cocos2d::Sprite * getLeftSprite() { return leftSprite; }
	const cocos2d::Sprite * getCenterSprite() { return centerSprite; }
	const cocos2d::Sprite * getRightSprite() { return rightSprite; }

	const LineInfo getLineInfo() { return lineInfo; }

	void destroyLeftSprite();
	void destroyCenterSprite();
	void destroyRightSprite();

private:
	cocos2d::Sprite * leftSprite;
	cocos2d::Sprite * centerSprite;
	cocos2d::Sprite * rightSprite;

	LineInfo lineInfo;
	LineSpritePool * factory;

	cocos2d::Sprite * initSpriteLike(LineInfo::Element, const cocos2d::Vec2 &);
};

#endif //__LINE_SPRITES_H__