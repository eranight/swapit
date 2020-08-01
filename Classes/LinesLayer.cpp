#include "LinesLayer.h"
#include "SimpleAudioEngine.h"
#include "LineSprites.h"

USING_NS_CC;

const std::string LinesLayer::GENERATE_NEW_LINE_EVENT = "generate new line event";

LinesLayer * LinesLayer::create(const LinesLayerConfiguration & configuration, LineSupplier * lineSupplier) {
	auto pRef = new (std::nothrow) LinesLayer();
	if (pRef != nullptr && pRef->init(configuration, lineSupplier)) {
		pRef->autorelease();
		return pRef;
	}
	else {
		delete pRef;
		pRef = nullptr;
		return nullptr;
	}
}

LinesLayer::~LinesLayer() {
	CC_SAFE_DELETE(factory);
}

bool LinesLayer::init(const LinesLayerConfiguration & configuration, LineSupplier * lineSupplier)
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->startPosition = configuration.getStartPosition();
	this->finishPosition = configuration.getFinishPosition();
	this->velocity = configuration.getVelocity();
	recalculateTime();
	this->lineSupplier = lineSupplier;

	this->factory = new (std::nothrow) LineSpritePool(6, 6);

	scheduleUpdate();

	return true;
}

void LinesLayer::onEnter() {
	Layer::onEnter();
	isBlocked = false;
	getEventDispatcher()->addCustomEventListener(GENERATE_NEW_LINE_EVENT, CC_CALLBACK_0(LinesLayer::generateNewLine, this));
}

void LinesLayer::onExit() {
	Layer::onExit();
	getEventDispatcher()->removeCustomEventListeners(GENERATE_NEW_LINE_EVENT);
}

void LinesLayer::update(float dt) {
	if (isBlocked) return;
}

void LinesLayer::block() {
	isBlocked = true;
	for (auto line : lines) {
		line->stopAllActions();
	}
}

void LinesLayer::unblock() {
	isBlocked = false;
	recreateLineActions();
}

void LinesLayer::generateNewLine()
{
	LineInfo lineInfo = lineSupplier->getNextLine();
	LineSprites * line = LineSprites::create(lineInfo, factory);
	line->setPosition(startPosition);
	this->addChild(line);
	lines.pushBack(line);
	line->runAction(Sequence::create(MoveTo::create(timer, finishPosition), CallFuncN::create(CC_CALLBACK_1(LinesLayer::removeLine, this)), RemoveSelf::create(), nullptr));
	CCLOG("generate new line");
}

void LinesLayer::setVelocity(float velocity) {
	this->velocity = velocity;
	recalculateTime();
	for (auto line : lines) {
		line->stopAllActions();
	}
	recreateLineActions();
}

LineSprites * LinesLayer::getLineBetween(float lowY, float highY) {
	for (auto & line : lines) {
		float y = this->convertToWorldSpace(line->getPosition()).y;
		if (lowY <= y && y <= highY) {
			return line;
		}
	}
	return nullptr;
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

void LinesLayer::recalculateTime() {
	timer = (startPosition - finishPosition).length() / velocity;
}

void LinesLayer::removeLine(Node * line) {
	lines.eraseObject(dynamic_cast<LineSprites *>(line));
	CCLOG("delete line");
}

void LinesLayer::recreateLineActions() {
	for (auto line : lines) {
		float moveTime = (line->getPosition() - finishPosition).length() / velocity;
		line->runAction(Sequence::create(MoveTo::create(moveTime, finishPosition), CallFuncN::create(CC_CALLBACK_1(LinesLayer::removeLine, this)), RemoveSelf::create(), nullptr));
	}
}