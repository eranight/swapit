#include "CollisionDetector.hpp"
#include "SwapLayer.h"
#include "GenerateLayer.h"

USING_NS_CC;

CollisionDetector * CollisionDetector::create(SwapLayer * swapLayer, GenerateLayer * generateLayer, CollideCallback callback) {
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

bool CollisionDetector::init(SwapLayer * swapLayer, GenerateLayer * generateLayer, CollideCallback callback) {
	this->swapLayer = swapLayer;
	this->swapLayer->retain();
	this->generateLayer = generateLayer;
	this->generateLayer->retain();
	this->collideCallback = callback;
	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
	return true;
}

void CollisionDetector::update(float deltaTime) {

}