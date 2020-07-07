#pragma once

#include <functional>

#include"cocos2d.h"
#include "LineInfo.h"

class SwapLayer;
class LinesLayer;
class LineSprites;

using CollideCallback = std::function<bool(const LineInfo::Element &, const LineInfo::Element &)>;

class Circle {
public:
	Circle(cocos2d::Vec2 center, float radius) : center(center), radius(radius) {}
	bool intersectsCircle(const Circle & circle) {
		return (center - circle.center).length() <= radius + circle.radius;
	}
	bool intersectsRect(const cocos2d::Rect & rect) {
		return rect.intersectsCircle(center, radius);
	}
	cocos2d::Vec2 getCenter() const { return center; }
	float getRadius() const { return radius; }
private:
	cocos2d::Vec2 center;
	float radius;
};

class Shape {
public:
	Shape() : sprite(nullptr) {}
	Shape(const cocos2d::Sprite * sprite, LineInfo::Element element, cocos2d::Vec2 position) : sprite(sprite), element(element), position(position) {}
	bool isEmpty() const { return sprite == nullptr; }
	const cocos2d::Sprite * getSprite() const { return sprite; }
	LineInfo::Element getElement() const { return element; }
	cocos2d::Vec2 getPosition() const { return position; }
	bool isCircle() const { return element != LineInfo::Element::green; }
	bool isRect() const { return element == LineInfo::Element::green; }
	cocos2d::Rect asRect() const;
	Circle asCircle() const;
	bool intersects(const Shape & other);
private:
	const cocos2d::Sprite * sprite;
	LineInfo::Element element;
	cocos2d::Vec2 position;
};

class CollisionDetector : public cocos2d::Node {
public:
	static CollisionDetector * create(SwapLayer *, LinesLayer *, CollideCallback);
	CollisionDetector() {}
	~CollisionDetector();
public:
	bool init(SwapLayer *, LinesLayer *, CollideCallback);
	void update(float);
private:
	SwapLayer * swapLayer;
	LinesLayer * generateLayer;
	CollideCallback collideCallback;

	Shape toShape(const cocos2d::Sprite * sprite, LineInfo::Element element);

	class Collision {
	public:
		Shape firstShape;
		Shape secondShape;
	};
	std::vector<Collision> collide(std::vector<Shape> firstShapes, std::vector<Shape> secondShapes);

	float higherTestCollisionLine;
	float lowerTestCollisionLine;
};