#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "cocos2d.h"
#include "LineInfo.h"

class SpriteManager : public cocos2d::Ref
{
public: //enums
	enum class SpriteColumn
	{
		left = 0,
		center,
		right
	};

private:
	SpriteManager();
	~SpriteManager();

public:
	static SpriteManager * getInstance();

public: //interface
	float getSpriteSize();
	float getColumn(SpriteManager::SpriteColumn);
	cocos2d::Sprite * getSprite(LineInfo::Element);
	cocos2d::Color3B getColor(LineInfo::Element el) { return colorsMap[el]; }
	LineInfo::Element getKeyByColor(const cocos2d::Color3B & color);

private:
	float leftColumn, centerColumn, rightColumn;
	cocos2d::Texture2D * ballTexture;
	cocos2d::Texture2D * squareTexture;
	std::map<LineInfo::Element, cocos2d::Color3B> colorsMap;

	bool init();
};

#define SPR_MANAGER SpriteManager::getInstance()

#endif //__SPRITE_MANAGER_H__