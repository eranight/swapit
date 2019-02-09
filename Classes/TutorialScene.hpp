#pragma once

#include "cocos2d.h"
#include "SwapLayer.h"
#include "LinesLayer.h"
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
	LineSupplier * lineSupplier;
	SwapLayer * swapLayer;
	LinesLayer * linesLayer;
private:
	cocos2d::Menu * menu; //skip button
private:
	void showPrompt();
	void hidePrompt();
	cocos2d::Label * promptLabel;
	float velocity;
	float promptPosition;
	cocos2d::Sequence * showNextPromptAction;
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
	TutorialLineSupplier(const std::initializer_list<LineInfo> & lines);
	LineInfo getNextLine() override;
private:
	std::queue<LineInfo> lineQueue;
};