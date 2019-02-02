#pragma once

#include <functional>

#include"cocos2d.h"
#include "LineInfo.h"

class SwapLayer;
class LinesLayer;
class LineSprites;

using CollideCallback = std::function<bool(const LineInfo::Element &, const LineInfo::Element &)>;

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
	bool checkCollision(LineSprites * line, cocos2d::Node *, cocos2d::Node *);
};