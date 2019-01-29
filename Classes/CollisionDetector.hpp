#pragma once

#include <functional>

#include"cocos2d.h"
#include "LineInfo.h"

class SwapLayer;
class GenerateLayer;
class LineSprites;

using CollideCallback = std::function<bool(const LineInfo::Element &, const LineInfo::Element &)>;

class CollisionDetector : public cocos2d::Node {
public:
	static CollisionDetector * create(SwapLayer *, GenerateLayer *, CollideCallback);
	CollisionDetector() {}
	~CollisionDetector();
public:
	bool init(SwapLayer *, GenerateLayer *, CollideCallback);
	void update(float);
private:
	SwapLayer * swapLayer;
	GenerateLayer * generateLayer;
	CollideCallback collideCallback;
	bool checkCollision(LineSprites * line, cocos2d::Node *, cocos2d::Node *);
};