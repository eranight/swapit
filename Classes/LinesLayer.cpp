#include "LinesLayer.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "LineSprites.h"

USING_NS_CC;

const std::string LinesLayer::GENERATE_NEW_LINE_EVENT = "generate new line event";
const std::string LinesLayer::SET_VELOCITY_EVENT = "set velocity event";

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

LinesLayer::~LinesLayer() {}

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

	scheduleUpdate();

	return true;
}

void LinesLayer::onEnter() {
	CCLOG("creates listeners");
	Layer::onEnter();
	getEventDispatcher()->addCustomEventListener(GENERATE_NEW_LINE_EVENT, CC_CALLBACK_0(LinesLayer::generateNewLine, this));
	getEventDispatcher()->addCustomEventListener(SET_VELOCITY_EVENT, CC_CALLBACK_1(LinesLayer::setVelocity, this));
}

void LinesLayer::onExit() {
	CCLOG("removes listeners");
	Layer::onExit();
	getEventDispatcher()->removeCustomEventListeners(GENERATE_NEW_LINE_EVENT);
	getEventDispatcher()->removeCustomEventListeners(SET_VELOCITY_EVENT);
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
	CCLOG("generate new line");
}

void LinesLayer::setVelocity(cocos2d::EventCustom * event) {
	this->velocity = *(reinterpret_cast<float *>(event->getUserData()));
	recalculateTime();
	for (auto line : lines) {
		line->stopAllActions();
	}
	recreateLineActions();
}

LineSprites * LinesLayer::getFirstLineAbove(float y) {
	for (auto & line : lines) {
		if (line->getPosition().y > y) {
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