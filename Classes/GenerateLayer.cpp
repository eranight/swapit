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
	nextGenerationPosition = Vec2(origin.x, origin.y + 2.0f * SPR_MANAGER->getSpriteSize());

	velocity = (startPosition - finishPosition).length() / 5.0f;
	timeToNextGeneration = (startPosition - nextGenerationPosition).length() / velocity;

	generateNewLine();

	return true;
}

void GenerateLayer::generateNewLine()
{
	LineSprites * line = LineSprites::create(LineSprites::LineElement::blue, LineSprites::LineElement::green, LineSprites::LineElement::red);
	line->setPosition(startPosition);
	this->addChild(line);
	line->runAction(
		Sequence::createWithTwoActions(
			MoveTo::create((startPosition - finishPosition).length() / velocity, finishPosition),
			RemoveSelf::create()
		)
	);
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(timeToNextGeneration), CallFunc::create(CC_CALLBACK_0(GenerateLayer::generateNewLine, this))));
}