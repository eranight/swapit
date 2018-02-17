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
	ballTexture = Director::getInstance()->getTextureCache()->addImage("ball.png");
	squareTexture = Director::getInstance()->getTextureCache()->addImage("square.png");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	leftColumn = (origin + visibleSize).x * 0.2f;
	centerColumn = (origin + visibleSize).x * 0.5f;
	rightColumn = (origin + visibleSize).x * 0.8f;

	colorsMap[LineInfo::Element::red] = Color3B::RED;
	colorsMap[LineInfo::Element::blue] = Color3B::BLUE;
	colorsMap[LineInfo::Element::violet] = Color3B::MAGENTA;
	colorsMap[LineInfo::Element::green] = Color3B::GREEN;

	return true;
}

float SpriteManager::getSpriteSize()
{
	return ballTexture->getContentSize().height * Director::getInstance()->getContentScaleFactor();
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
	sprite->setColor(colorsMap[spriteType]);
	return sprite;
}