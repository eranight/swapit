#pragma once

#include "cocos2d.h"
#include "SwapLayer.h"
#include "LinesLayer.h"
#include "LineSupplier.hpp"

class TutorialScene : public cocos2d::Scene {
public:
	CREATE_FUNC(TutorialScene);
	TutorialScene();
	~TutorialScene();
public:
	virtual bool init() override;
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
	void emitCollision();
	cocos2d::Label * promptLabel;
	float velocity;
	float promptPosition;
	cocos2d::Sequence * showNextPromptAction;
	cocos2d::Sequence * collisionHappenedAction;
	class Prompt {
	public:
		Prompt(const std::string &, const std::string &, const std::function<void(LinesLayer *)> &);
		std::string text;
		std::string awaitedEvent;
		std::function<void(LinesLayer *)> destroyLinesObjectAction;

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