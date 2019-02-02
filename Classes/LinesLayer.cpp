#include "LinesLayer.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "LineSprites.h"

USING_NS_CC;

LinesLayer * LinesLayer::create(LineSupplier * lineSupplier) {
	auto pRef = new (std::nothrow) LinesLayer();
	if (pRef != nullptr && pRef->init(lineSupplier)) {
		pRef->autorelease();
		return pRef;
	}
	else {
		delete pRef;
		pRef = nullptr;
		return nullptr;
	}
}

LinesLayer::~LinesLayer() {}

bool LinesLayer::init(LineSupplier * lineSupplier)
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->lineSupplier = lineSupplier;

	scheduleUpdate();

	return true;
}

void LinesLayer::update(float dt) {
	if (isBlocking()) return;
}

void LinesLayer::block() {
	Blocking::block();
	for (auto line : lines) {
		line->stopAllActions();
	}
}

void LinesLayer::unblock() {
	Blocking::unblock();
	recreateLineActions();
}

void LinesLayer::generateNewLine()
{
	LineInfo lineInfo = lineSupplier->getNextLine();
	LineSprites * line = LineSprites::create(lineInfo);
	line->setPosition(startPosition);
	this->addChild(line);
	lines.pushBack(line);
	line->runAction(Sequence::create(MoveTo::create(timer, finishPosition), CallFuncN::create(CC_CALLBACK_1(LinesLayer::removeLine, this)), RemoveSelf::create(), nullptr));
}

LineSprites * LinesLayer::getFirstLineAbove(float y) {
	for (auto & line : lines) {
		if (line->getPosition().y > y) {
			return line;
		}
	}
	return nullptr;
}

void LinesLayer::start() {
	generateNewLine();
}

void LinesLayer::stop() {
	this->stopAllActions();
	unscheduleUpdate();
}

void LinesLayer::pause() {
	for (auto line : lines) {
		line->pause();
	}
}

void LinesLayer::resume() {
	for (auto line : lines) {
		line->resume();
	}
}

void LinesLayer::setVelocity(float velocity) {
	this->velocity = velocity;
	recalculateTime();
	for (auto line : lines) {
		line->stopAllActions();
	}
	recreateLineActions();
}

void LinesLayer::recalculateTime() {
	timer = (startPosition - finishPosition).length() / velocity;
}

void LinesLayer::removeLine(Node * line) {
	lines.eraseObject(dynamic_cast<LineSprites *>(line));
}

void LinesLayer::recreateLineActions() {
	for (auto line : lines) {
		float moveTime = (line->getPosition() - finishPosition).length() / velocity;
		line->runAction(Sequence::create(MoveTo::create(moveTime, finishPosition), CallFuncN::create(CC_CALLBACK_1(LinesLayer::removeLine, this)), RemoveSelf::create(), nullptr));
	}
}