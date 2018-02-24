#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GenerateLayer;
class SwapLayer;
class PauseGameLayer;
class LineSprites;

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

private:
	bool gameOver;
	int goals;
	cocos2d::Label * goalsLabel;
	int goalLevel;
	float velocity;

	GenerateLayer * generateLayer;
	SwapLayer * swapLayer;
	PauseGameLayer * pauseGameLayer;

	LineSprites * lineForCollisionDetect;

	void collisionUpdate();
	void checkCollision(const cocos2d::Sprite *, const cocos2d::Sprite *);
};

#endif // __GAME_SCENE_H__
