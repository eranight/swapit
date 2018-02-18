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
	nextGenerationPosition = Vec2(origin.x, origin.y + 4.0f * SPR_MANAGER->getSpriteSize());

	return true;
}

void GenerateLayer::generateNewLine()
{
	LineInfo lineInfo = lineBuilder.getNextLine();
	LineSprites * line = LineSprites::create(lineInfo);
	line->setPosition(startPosition);
	this->addChild(line);
	line->runAction(
		Sequence::create(
			MoveTo::create((startPosition - finishPosition).length() / velocity, finishPosition),
			CallFunc::create(CC_CALLBACK_0(GenerateLayer::eraseLine, this, line)),
			RemoveSelf::create(),
			nullptr
		)
	);
	lines.pushBack(line);
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(timeToNextGeneration), CallFunc::create(CC_CALLBACK_0(GenerateLayer::generateNewLine, this))));
}

void GenerateLayer::eraseLine(LineSprites * line) {
	lines.eraseObject(line);
}

LineSprites * GenerateLayer::getFirstHighLine(float y) {
	for (auto & line : lines) {
		if (line->getPosition().y > y) {
			return line;
		}
	}
	return nullptr;
}

void GenerateLayer::start(float velocity) {
	setVelocity(velocity);
	generateNewLine();
}

void GenerateLayer::setVelocity(float velocity) {
	this->velocity = velocity;
	timeToNextGeneration = (startPosition - nextGenerationPosition).length() / velocity;
}