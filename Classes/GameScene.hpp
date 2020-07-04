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
	bool init() override;
	void onEnter() override;
private:
	void gameOver();
private:
	SwapLayer * swapLayer;
	LinesLayer * linesLayer;

	cocos2d::Layer * gameOverLayer;
	cocos2d::Label * scoreLabel;
	cocos2d::Menu * menu;
};