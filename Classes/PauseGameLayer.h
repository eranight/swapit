#ifndef __PAUSE_GAME_LAYER_H__
#define __PAUSE_GAME_LAYER_H__

#include "cocos2d.h"

class PauseGameLayer : public cocos2d::LayerColor {
public:
	CREATE_FUNC(PauseGameLayer);
	bool init() override;
	void pause() override;
	void resume() override;
	void setVisible(bool) override;

private:
	cocos2d::Menu * menu;

	void menuPauseCallback(cocos2d::Ref *);
	void menuResumeCallback(cocos2d::Ref *);
	void menuBackCallback(cocos2d::Ref *);

	void switchVisibility(bool, bool);
};

#endif //__PAUSE_GAME_LAYER_H__