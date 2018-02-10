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

	enum class LinePosition
	{
		left = 0,
		center,
		right
	};

public:
	LineSprites();

	static LineSprites * create(const std::vector<LineSprites::LineElement> &);
	bool init(const std::vector<LineSprites::LineElement> &);
	
	//getters
	cocos2d::Sprite * getSprite(const LineSprites::LinePosition &);

private:
	cocos2d::Vector<cocos2d::Sprite *> sprites;

	cocos2d::Sprite * createSprite(const LineSprites::LineElement & element);
};

#endif //__LINE_SPRITES_H__