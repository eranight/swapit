#ifndef __TUTORIAL_SCRIPT_H__
#define __TUTORIAL_SCRIPT_H__

#include "AbstractScript.h"

class LineSprites;

class TutorialScript : public AbstractScript {
public:
	TutorialScript(GameScene * gameScene);
	~TutorialScript();

public:
	void init() override;
	void update(float dt) override;
	bool collide(LineInfo::Element elemA, LineInfo::Element elemB) override;
	void pause() override;
	void resume() override;

private:
	GenerateLayer * generateLayer;
	SwapLayer * swapLayer;

private:
	bool promtIsActive;
	int promtNumber;
	float showPromtPosition;
	const std::vector<std::string> promts;
	cocos2d::Label * promtLabel;
	LineSprites * line;
	SwapLayer::BallOrder prevBallOrder;
};

#endif //__TUTORIAL_SCRIPT_H__