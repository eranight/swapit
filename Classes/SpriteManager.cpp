#include "SpriteManager.h"

USING_NS_CC;

static SpriteManager * s_SharedSpriteManager = nullptr;

SpriteManager::SpriteManager()
{

}

SpriteManager::~SpriteManager()
{
	s_SharedSpriteManager = nullptr;
}

SpriteManager * SpriteManager::getInstance()
{
	if (!s_SharedSpriteManager)
	{
		s_SharedSpriteManager = new (std::nothrow) SpriteManager();
		s_SharedSpriteManager->autorelease();
		s_SharedSpriteManager->init();
	}
	return s_SharedSpriteManager;
}

bool SpriteManager::init()
{
	ballTexture = Director::getInstance()->getTextureCache()->addImage("circle.png");
	squareTexture = Director::getInstance()->getTextureCache()->addImage("square.png");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	realSpriteSize = ballTexture->getContentSize().height;
	collisionSpriteSize = realSpriteSize * Configuration::getInstance()->getValue("collisionCoefficient").asFloat();
	float halfSize = realSpriteSize * 0.5f;
	leftColumn = (origin + visibleSize).x * 0.2f - halfSize;
	centerColumn = (origin + visibleSize).x * 0.5f - halfSize;
	rightColumn = (origin + visibleSize).x * 0.8f - halfSize;

	colorsMap[LineInfo::Element::red] = Color3B(255, 51, 51);
	colorsMap[LineInfo::Element::blue] = Color3B(0, 128, 255);
	colorsMap[LineInfo::Element::violet] = Color3B(178, 102, 255);
	colorsMap[LineInfo::Element::green] = Color3B(0, 153, 76);

	return true;
}

float SpriteManager::getColumn(SpriteManager::SpriteColumn spriteColumn)
{
	return spriteColumn == SpriteManager::SpriteColumn::left ? leftColumn : 
		(spriteColumn == SpriteManager::SpriteColumn::center ? centerColumn : rightColumn);
}

Sprite * SpriteManager::getSprite(LineInfo::Element spriteType)
{
	if (spriteType == LineInfo::Element::none)
		return nullptr;
	Sprite * sprite = Sprite::createWithTexture(spriteType != LineInfo::Element::green ? ballTexture : squareTexture);
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sprite->setColor(colorsMap[spriteType]);
	return sprite;
}

Sprite * SpriteManager::getSprite(SpriteManager::SpriteType spriteType) {
	Sprite * sprite = Sprite::createWithTexture(spriteType == SpriteType::circle ? ballTexture : squareTexture);
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	return sprite;
}

LineInfo::Element SpriteManager::getKeyByColor(const cocos2d::Color3B & color) {
	for (auto pair : colorsMap) {
		if (pair.second == color) {
			return pair.first;
		}
	}
	return LineInfo::Element::none;
}