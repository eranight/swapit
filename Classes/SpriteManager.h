#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "cocos2d.h"

class SpriteManager : public cocos2d::Ref
{
public: //enums
	enum class SpriteType
	{
		red = 0,
		blue,
		violet,
		green  //square
	};
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
	cocos2d::Sprite * getSprite(SpriteManager::SpriteType);

private:
	float leftColumn, centerColumn, rightColumn;
	cocos2d::Texture2D * ballTexture;
	cocos2d::Texture2D * squareTexture;
	std::map<SpriteManager::SpriteType, cocos2d::Color3B> colorsMap;

	bool init();
};

#define SPR_MANAGER SpriteManager::getInstance()

#endif //__SPRITE_MANAGER_H__