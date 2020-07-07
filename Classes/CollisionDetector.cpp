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
	unscheduleUpdate();
}

bool CollisionDetector::init(SwapLayer * swapLayer, LinesLayer * generateLayer, CollideCallback callback) {
	this->swapLayer = swapLayer;
	this->swapLayer->retain();
	this->generateLayer = generateLayer;
	this->generateLayer->retain();
	this->collideCallback = callback;

	float collisionSpriteSize = SPR_MANAGER->getCollisionSpriteSize();
	float swapLayerLine = swapLayer->convertToWorldSpace(swapLayer->getLinePosition()).y;
	higherTestCollisionLine = swapLayerLine + collisionSpriteSize;
	lowerTestCollisionLine = swapLayerLine - collisionSpriteSize;

	scheduleUpdate();
	return true;
}

void CollisionDetector::update(float deltaTime) {
	auto spritesLine = generateLayer->getLineBetween(lowerTestCollisionLine, higherTestCollisionLine);
	if (spritesLine != nullptr) {
		LineInfo lineInfo = spritesLine->getLineInfo();
		auto lineShapes = { toShape(spritesLine->getLeftSprite(), spritesLine->getLineInfo().getLeft()),
			toShape(spritesLine->getCenterSprite(), spritesLine->getLineInfo().getMiddle()),
			toShape(spritesLine->getRightSprite(), spritesLine->getLineInfo().getRight()) };
		std::vector<Shape> swapShapes;
		if (swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
			swapShapes = { toShape(swapLayer->getVioletBallSprite(), LineInfo::Element::violet) };
		}
		else {
			swapShapes = { toShape(swapLayer->getRedBallSprite(), LineInfo::Element::red), toShape(swapLayer->getBlueBallSprite(), LineInfo::Element::blue) };
		}
		auto collisions = collide(lineShapes, swapShapes);
		for (auto & collision : collisions) {
			if (collideCallback(collision.firstShape.getElement(), collision.secondShape.getElement())) {
				if (collision.firstShape.getSprite() == spritesLine->getLeftSprite()) {
					spritesLine->destroyLeftSprite();
				}
				else if (collision.firstShape.getSprite() == spritesLine->getCenterSprite()) {
					spritesLine->destroyCenterSprite();
				}
				else if (collision.firstShape.getSprite() == spritesLine->getRightSprite()) {
					spritesLine->destroyRightSprite();
				}
			}
		}
	}
	else {
		return;
	}
}

Shape CollisionDetector::toShape(const Sprite * sprite, LineInfo::Element element) {
	if (element == LineInfo::Element::none) {
		return Shape();
	}
	Vec2 worldPosition = sprite->getParent()->convertToWorldSpace(sprite->getPosition());
	return Shape({ sprite, element, worldPosition });
}

std::vector<CollisionDetector::Collision> CollisionDetector::collide(std::vector<Shape> firstShapes, std::vector<Shape> secondShapes) {
	std::vector<Collision> collisions;
	for (auto & firstShape : firstShapes) {
		if (!firstShape.isEmpty()) {
			for (auto & secondShape : secondShapes) {
				if (!secondShape.isEmpty()) {
					if (firstShape.intersects(secondShape)) {
						collisions.push_back(Collision({ firstShape, secondShape }));
					}
				}
			}
		}
	}
	return collisions;
}

Rect Shape::asRect() const {
	float ss = SpriteManager::getInstance()->getSpriteSize();
	float cs = SpriteManager::getInstance()->getCollisionSpriteSize();
	float m = abs(ss - cs) * 0.5f;
	return Rect(position.x + m, position.y + m, cs, cs);
}

Circle Shape::asCircle() const {
	float half_ss = SPR_MANAGER->getSpriteSize() * 0.5f;
	float half_cs = SPR_MANAGER->getCollisionSpriteSize() * 0.5f;
	Vec2 center = Vec2(position.x + half_ss, position.y + half_ss);
	return Circle(center, half_cs);
}

bool Shape::intersects(const Shape & other) {
	if (isCircle()) {
		Circle circle = asCircle();
		if (other.isCircle()) {
			return circle.intersectsCircle(other.asCircle());
		}
		else {
			return circle.intersectsRect(other.asRect());
		}
	}
	else {
		Rect rect = asRect();
		if (other.isCircle()) {
			Circle circle = other.asCircle();
			return rect.intersectsCircle(circle.getCenter(), circle.getRadius());
		}
		else {
			return rect.intersectsRect(other.asRect());
		}
	}
}
