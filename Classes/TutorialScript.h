#ifndef __TUTORIAL_SCRIPT_H__
#define __TUTORIAL_SCRIPT_H__

#include "AbstractScript.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"

class LineSprites;

class TutorialScript : public AbstractScript {
public:
	TutorialScript(GameScene * gameScene);
	~TutorialScript() {}

public:
	void init() override;
	void release() override;
	void update(float dt) override;
	bool collide(LineInfo::Element elemA, LineInfo::Element elemB) override;
	void pause() override;
	void resume() override;

private:
	void showPromt();

private:
	bool promptIsActive;
	int promptNumber;
	float showPromptPosition;
	const std::vector<std::string> prompts;
	cocos2d::Label * promptLabel;
	cocos2d::Action * showPromptAction;
	SwapLayer::BallOrder prevBallOrder;
};

#endif //__TUTORIAL_SCRIPT_H__