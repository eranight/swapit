#include "GenerateLayer.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "LineSprites.h"

USING_NS_CC;

GenerateLayer::~GenerateLayer() {}

bool GenerateLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	startPosition = Vec2(origin.x, (origin + visibleSize).y);
	finishPosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());

	LineSprites * line = LineSprites::create(LineSprites::LineElement::blue, LineSprites::LineElement::green, LineSprites::LineElement::red);
	line->setPosition(startPosition);
	this->addChild(line);
	line->runAction(MoveTo::create(5.0f, finishPosition));

	return true;
}