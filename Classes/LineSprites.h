#ifndef __LINE_SPRITES_H__
#define __LINE_SPRITES_H__

#include "cocos2d.h"
#include "LineInfo.h"
#include "SpriteManager.h"

class LineSprites : public cocos2d::Node
{
public:

	static LineSprites * create(LineInfo &);
	bool init(LineInfo &);

	//getters
	cocos2d::Sprite * getLeftSprite() { return leftSprite; }
	cocos2d::Sprite * getCenterSprite() { return centerSprite; }
	cocos2d::Sprite * getRightSprite() { return rightSprite; }

private:
	cocos2d::Sprite * leftSprite;
	cocos2d::Sprite * centerSprite;
	cocos2d::Sprite * rightSprite;

	cocos2d::Sprite * initSpriteLike(LineInfo::Element, const cocos2d::Vec2 &);
};

#endif //__LINE_SPRITES_H__