#pragma once

#include "cocos2d.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"
#include "CollisionDetector.hpp"
#include "LineSupplier.hpp"

class TutorialScene : public cocos2d::LayerColor {
public:
	CREATE_FUNC(TutorialScene);
	TutorialScene();
	~TutorialScene();
public:
	virtual bool init() override;
	void update(float) override;
private:
	void menuSkipCallback(cocos2d::Ref *);
private:
	SwapLayer * swapLayer;
	GenerateLayer * generateLayer;
	CollisionDetector * collisionDetector;
	bool collide(const LineInfo::Element &, const LineInfo::Element &);
private:
	cocos2d::Menu * menu; //skip button
private:
	void showPrompt();
	void hidePrompt();
	cocos2d::Label * promptLabel;
	class Prompt {
	public:
		Prompt(const std::string &, const std::string &);
		std::string text;
		std::string awaitedEvent;
	};
	std::queue<Prompt> prompts;
};

class TutorialLineSupplier : public LineSupplier {
public:
	TutorialLineSupplier(std::queue<LineInfo> lineQueue) : lineQueue(lineQueue) {}
	LineSprites * getNextLine() override;
private:
	std::queue<LineInfo> lineQueue;
};