#pragma once

#include "cocos2d.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"

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
private:
	cocos2d::Menu * menu; //skip button
private:
	const std::vector<std::string> prompts;
	cocos2d::Label * promptLabel;
};