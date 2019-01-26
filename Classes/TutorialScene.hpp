#pragma once

#include "cocos2d.h"

class TutorialScene : public cocos2d::LayerColor {
public:
	CREATE_FUNC(TutorialScene);
	~TutorialScene();
public:
	virtual bool init() override;
	void update(float) override;
	void pause() override;
	void resume() override;
};