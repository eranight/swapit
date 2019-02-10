#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::LayerColor
{
public:
	~GameScene();

public:
	static cocos2d::Scene* createScene();

	CREATE_FUNC(GameScene);
	virtual bool init() override;
	void update(float) override;
	void pause() override;
	void resume() override;
};


#endif // __GAME_SCENE_H__
