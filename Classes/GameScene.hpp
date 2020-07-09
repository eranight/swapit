#pragma once

#include "cocos2d.h"
#include "SwapLayer.h"
#include "LinesLayer.h"
#include "LineSupplier.hpp"
#include "LevelProbabilities.hpp"
#include "CollisionDetector.hpp"

class GameScene : public cocos2d::Scene {
public:
	CREATE_FUNC(GameScene);
	GameScene();
	~GameScene();
public:
	bool init() override;
	void onEnter() override;
private:
	void run();
	void gameOver();
	std::vector<LevelProbabilities> convert();
	bool collide(const LineInfo::Element & first, const LineInfo::Element & second);
private:
	SwapLayer * swapLayer;
	LinesLayer * linesLayer;

	cocos2d::Layer * gameOverLayer;
	cocos2d::Label * scoreLabel;
	cocos2d::Menu * menu;

	LineSupplier * lineSupplier;
	CollisionDetector * detector;

	int score;
	cocos2d::Vec2 center;
};