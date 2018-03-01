#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GenerateLayer;
class SwapLayer;
class PauseGameLayer;
class LineSprites;

class GameScene;
class SwapItSctipt {
public:
	SwapItSctipt(GameScene * gs) { gameScene = gs; }
public:
	virtual void init() = 0;
	virtual void update(float dt) = 0;

protected:
	GameScene * gameScene;
};
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

	SwapItSctipt * script;

friend class TutorialScript;
friend class GameScript;
};

class TutorialScript : public SwapItSctipt {
public:
	TutorialScript(GameScene * gs);

public:
	void init() override;
	void update(float dt) override;

private:
	bool promtIsActive;
	int promtNumber;
	float showPromtPosition;
	const std::vector<std::string> promts;
	cocos2d::Label * promtLabel;
	LineSprites * line;

};

class GameScript : public SwapItSctipt {
public:
	GameScript(GameScene * gs);

public:
	void init() override;
	void update(float dt) override;

};

#endif // __GAME_SCENE_H__
