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
	if (isBlocking()) return;
	/*Vec2 ds = Vec2(0.0f, dt * velocity);
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
	}*/
}

void GenerateLayer::block() {
	Blocking::block();
	for (auto line : lines) {
		line->stopAllActions();
	}
}

void GenerateLayer::unblock() {
	Blocking::unblock();
	recreateLineActions();
}

void GenerateLayer::generateNewLine()
{
	LineInfo lineInfo = lineBuilder.getNextLine();
	LineSprites * line = LineSprites::create(lineInfo);
	line->setPosition(startPosition);
	this->addChild(line);
	lines.pushBack(line);
	line->runAction(Sequence::create(MoveTo::create(timer, finishPosition), CallFuncN::create(CC_CALLBACK_1(GenerateLayer::removeLine, this)), RemoveSelf::create(), nullptr));
}

LineSprites * GenerateLayer::getFirstLineAbove(float y) {
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

void GenerateLayer::pause() {
	for (auto line : lines) {
		line->pause();
	}
}

void GenerateLayer::resume() {
	for (auto line : lines) {
		line->resume();
	}
}

void GenerateLayer::setVelocity(float velocity) {
	this->velocity = velocity;
	timeToNextGeneration = (startPosition - nextGenerationPosition).length() / velocity;
	recalculateTime();
	for (auto line : lines) {
		line->stopAllActions();
	}
	recreateLineActions();
}

void GenerateLayer::recalculateTime() {
	timer = (startPosition - finishPosition).length() / velocity;
}

void GenerateLayer::removeLine(Node * line) {
	lines.eraseObject(dynamic_cast<LineSprites *>(line));
}

void GenerateLayer::recreateLineActions() {
	for (auto line : lines) {
		float moveTime = (line->getPosition() - finishPosition).length() / velocity;
		line->runAction(Sequence::create(MoveTo::create(moveTime, finishPosition), CallFuncN::create(CC_CALLBACK_1(GenerateLayer::removeLine, this)), RemoveSelf::create(), nullptr));
	}
}