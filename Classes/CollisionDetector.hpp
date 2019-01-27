#pragma once

#include <functional>

#include"cocos2d.h"
#include "LineInfo.h"

class SwapLayer;
class GenerateLayer;

using CollideCallback = std::function<bool(const LineInfo::Element &, const LineInfo::Element &)>;

class CollisionDetector : public cocos2d::Ref {
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
};