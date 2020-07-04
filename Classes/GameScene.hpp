#pragma once

#include "cocos2d.h"
#include "SwapLayer.h"
#include "LinesLayer.h"
#include "LineSupplier.hpp"

class GameScene : public cocos2d::Scene {
public:
	CREATE_FUNC(GameScene);
	GameScene();
	~GameScene();
public:
	virtual bool init() override;
private:
	SwapLayer * swapLayer;
	LinesLayer * linesLayer;
};