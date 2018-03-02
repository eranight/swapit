#ifndef __ABSTRACT_SCRIPT_H__
#define __ABSTRACT_SCRIPT_H__

#include "cocos2d.h"
#include "LineInfo.h"

class GameScene;
class GenerateLayer;
class SwapLayer;

class AbstractScript : public cocos2d::Ref {
public:
	AbstractScript(GameScene * gameScene) : gameOver(false) { this->gameScene = gameScene; }

public:
	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual bool collide(LineInfo::Element elemA, LineInfo::Element elemB) = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

protected:
	GameScene * gameScene;
	GenerateLayer * generateLayer;
	SwapLayer * swapLayer;

	bool gameOver;
};

#endif //__ABSTRACT_SCRIPT_H__