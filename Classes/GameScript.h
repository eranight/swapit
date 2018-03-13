#ifndef __GAME_SCRIPT_H__
#define __GAME_SCRIPT_H__

#include "AbstractScript.h"

class GameScript : public AbstractScript {
public:
	GameScript(GameScene * gameScene);

public:
	void init() override;
	void update(float dt);
	bool collide(LineInfo::Element elemA, LineInfo::Element elemB) override;
	void pause() override;
	void resume() override;

private: //score
	int score;
	cocos2d::Label * scoreLabel;

private: //game over
	bool gameOver;

};

#endif //__GAME_SCRIPT_H__