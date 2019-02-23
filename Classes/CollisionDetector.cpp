#include "CollisionDetector.hpp"
#include "SwapLayer.h"
#include "LinesLayer.h"

USING_NS_CC;

CollisionDetector * CollisionDetector::create(SwapLayer * swapLayer, LinesLayer * generateLayer, CollideCallback callback) {
	auto ref = new (std::nothrow) CollisionDetector();
	if (ref != nullptr && ref->init(swapLayer, generateLayer, callback)) {
		ref->autorelease();
		return ref;
	}
	else {
		delete ref;
		ref = nullptr;
		return nullptr;
	}
}

CollisionDetector::~CollisionDetector() {
	swapLayer->release();
	generateLayer->release();
	Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

bool CollisionDetector::init(SwapLayer * swapLayer, LinesLayer * generateLayer, CollideCallback callback) {
	this->swapLayer = swapLayer;
	this->swapLayer->retain();
	this->generateLayer = generateLayer;
	this->generateLayer->retain();
	this->collideCallback = callback;
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
	return true;
}

void CollisionDetector::update(float deltaTime) {
	float swapLinePos = swapLayer->getLinePosition().y - SpriteManager::getInstance()->getSpriteSize() * 0.5f;
	auto line = generateLayer->getFirstLineAbove(swapLinePos);
	if (line != nullptr) {
		
	}
}

//first is always a ball from SwapLayer, but second is might be a square
bool CollisionDetector::checkCollision(LineSprites * line, Node * first, Node * second) {
	Rect bbA = first->getBoundingBox();
	Vec2 centerA = line->convertToWorldSpace(Vec2(bbA.getMidX(), bbA.getMidY()));
	Rect bbB = second->getBoundingBox();
	Vec2 centerB = swapLayer->convertToWorldSpace(Vec2(bbB.getMidX(), bbB.getMidY()));
	float diameter = SPR_MANAGER->getSpriteSize();
	float radius = diameter * 0.5f;
	if (second->getColor() != SPR_MANAGER->getColor(LineInfo::Element::green)) {
		return (centerA - centerB).length() <= diameter;
	}
	else {
		Rect collisionRect = Rect(centerA.x - radius, centerA.y - radius, diameter, diameter);
		return collisionRect.intersectsCircle(centerB, radius);
	}
}