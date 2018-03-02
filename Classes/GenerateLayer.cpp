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

	/*startPosition = Vec2(origin.x, (origin + visibleSize).y);
	finishPosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());
	nextGenerationPosition = Vec2(origin.x, origin.y + 4.0f * SPR_MANAGER->getSpriteSize());*/

	scheduleUpdate();

	return true;
}

void GenerateLayer::update(float dt) {
	Vec2 ds = Vec2(0.0f, dt * velocity);
	for (auto & line = lines.begin(); line != lines.end();) {
		Vec2 currentPosition = (*line)->getPosition();
		Vec2 nextPosition = currentPosition - ds;
		(*line)->setPosition(nextPosition);
		if (nextPosition.y <= finishPosition.y) {
			this->removeChild(*line);
			line = lines.erase(line);
		}
		else {
			++line;
		}
	}
}

void GenerateLayer::generateNewLine()
{
	CCASSERT(velocity > 0, "You must set the velocity field > 0!");
	LineInfo lineInfo = lineBuilder.getNextLine();
	LineSprites * line = LineSprites::create(lineInfo);
	line->setPosition(startPosition);
	this->addChild(line);
	lines.pushBack(line);
	//this->runAction(Sequence::createWithTwoActions(DelayTime::create(timeToNextGeneration), CallFunc::create(CC_CALLBACK_0(GenerateLayer::generateNewLine, this))));
}

LineSprites * GenerateLayer::getFirstHighLine(float y) {
	for (auto & line : lines) {
		if (line->getPosition().y > y) {
			return line;
		}
	}
	return nullptr;
}

void GenerateLayer::start() {
	generateNewLine();
}

void GenerateLayer::stop() {
	this->stopAllActions();
	unscheduleUpdate();
}

void GenerateLayer::setVelocity(float velocity) {
	this->velocity = velocity;
	timeToNextGeneration = (startPosition - nextGenerationPosition).length() / velocity;
}