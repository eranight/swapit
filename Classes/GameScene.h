#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GenerateLayer;
class SwapLayer;
class UserInterfaceGameLayer;
class LineSprites;
class AbstractScript;
class TutorialScript;
class GameSctipt;

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

public:
	GenerateLayer * getGenerateLayer() { return generateLayer; }
	SwapLayer * getSwapLayer() { return swapLayer; }
	void invokeGameOver(int score);

private:
	GenerateLayer * generateLayer;
	SwapLayer * swapLayer;
	UserInterfaceGameLayer * userInterfaceGameLayer;
	LineSprites * lineForCollisionDetect;

	void collisionUpdate();
	void checkCollision(const cocos2d::Sprite *, const cocos2d::Sprite *, bool & needToDestroy);

	void skipScriptEvent(cocos2d::EventCustom *);
	void toNextScript();
	std::vector<AbstractScript *> scripts;
	std::vector<AbstractScript *>::iterator currentScriptPointer;
};


#endif // __GAME_SCENE_H__
