#ifndef __LINE_SPRITES_H__
#define __LINE_SPRITES_H__

#include "cocos2d.h"

class LineSprites : public cocos2d::Node
{
public:
	enum class LineElement
	{
		none = 0,
		red,
		blue,
		violet,
		green
	};

public:
	static LineSprites * create(LineSprites::LineElement leftElement, LineSprites::LineElement centerElement, LineSprites::LineElement rightElement);
	bool init(LineSprites::LineElement leftElement, LineSprites::LineElement centerElement, LineSprites::LineElement rightElement);

	//getters
	cocos2d::Sprite * getLeftSprite() { return leftSprite; }
	cocos2d::Sprite * getCenterSprite() { return centerSprite; }
	cocos2d::Sprite * getRightSprite() { return rightSprite; }

private:
	cocos2d::Sprite * leftSprite;
	cocos2d::Sprite * centerSprite;
	cocos2d::Sprite * rightSprite;

	cocos2d::Sprite * initSpriteLike(LineSprites::LineElement lineElement, const cocos2d::Vec2 & position);
};

#endif //__LINE_SPRITES_H__