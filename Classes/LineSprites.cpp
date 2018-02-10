#include "LineSprites.h"
#include "SpriteManager.h"

USING_NS_CC;

LineSprites * LineSprites::create(LineElement leftElement, LineElement centerElement, LineElement rightElement)
{
	LineSprites * pRet = new (std::nothrow) LineSprites();
	if (pRet && pRet->init(leftElement, centerElement, rightElement))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool LineSprites::init(LineElement leftElement, LineElement centerElement, LineElement rightElement)
{
	if (!Node::init())
	{
		return false;
	}
	this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	float halfBallSize = SPR_MANAGER->getSpriteSize() * 0.5f;

	leftSprite = initSpriteLike(leftElement, Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::left), halfBallSize));
	centerSprite = initSpriteLike(centerElement, Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::center), halfBallSize));
	rightSprite = initSpriteLike(rightElement, Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::right), halfBallSize));

	return true;
}

SpriteManager::SpriteType convertLineElementToSpriteType(LineSprites::LineElement lineElement)
{
	switch (lineElement)
	{
	case LineSprites::LineElement::none:
		throw std::exception("invalid enums values");
		break;
	case LineSprites::LineElement::red:
		return SpriteManager::SpriteType::red;
	case LineSprites::LineElement::blue:
		return SpriteManager::SpriteType::blue;
	case LineSprites::LineElement::violet:
		return SpriteManager::SpriteType::violet;
	case LineSprites::LineElement::green:
		return SpriteManager::SpriteType::green;
	default:
		throw std::exception("invalid enums values");
		break;
	}
}

Sprite * LineSprites::initSpriteLike(LineElement lineElement, const Vec2 & position)
{
	Sprite * sprite = nullptr;
	if (lineElement != LineElement::none)
	{
		sprite = SPR_MANAGER->getSprite(convertLineElementToSpriteType(lineElement));
		sprite->setPosition(position);
		this->addChild(sprite);
	}
	return sprite;
}