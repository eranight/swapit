#include "LineSprites.h"

USING_NS_CC;

LineSprites::LineSprites() : sprites({ nullptr, nullptr, nullptr }) {}

LineSprites * LineSprites::create(const std::vector<LineSprites::LineElement> & elements)
{
	LineSprites * pRet = new (std::nothrow) LineSprites();
	if (pRet && pRet->init(elements))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool LineSprites::init(const std::vector<LineSprites::LineElement> & elements)
{
	if (!Node::init())
	{
		return false;
	}

	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	auto & lineSprite = sprites.begin();
	for (auto & element = elements.begin(); element != elements.end(); ++element, ++lineSprite)
	{
		Sprite * sprite = nullptr;
		if (*element != LineElement::none)
		{
			sprite = createSprite(*element);
		}
		*lineSprite = sprite;
	}

	return true;
}

Sprite * LineSprites::getSprite(const LinePosition &position)
{
	return sprites.at(static_cast<int>(position));
}

Sprite * LineSprites::createSprite(const LineElement & element)
{
	Sprite * sprite = Sprite::createWithTexture(
		element != LineElement::green ?
			Director::getInstance()->getTextureCache()->getTextureForKey(FileUtils::getInstance()->fullPathForFilename("ball.png")) :
				Director::getInstance()->getTextureCache()->getTextureForKey(FileUtils::getInstance()->fullPathForFilename("square.png"))
	);

	switch (element)
	{
	case LineElement::red:
		sprite->setColor(Color3B::RED);
		break;
	case LineElement::blue:
		sprite->setColor(Color3B::BLUE);
		break;
	case LineElement::violet:
		sprite->setColor(Color3B::MAGENTA);
		break;
	case LineElement::green:
		sprite->setColor(Color3B::GREEN);
		break;
	default:
		break;
	}
	return sprite;
}