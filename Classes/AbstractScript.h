#ifndef __ABSTRACT_SCRIPT_H__
#define __ABSTRACT_SCRIPT_H__

#include "cocos2d.h"
#include "LineInfo.h"

class GameScene;
class GenerateLayer;
class SwapLayer;

class AbstractScript {
public:
	AbstractScript(GameScene * gameScene) : gameScene(gameScene), finished(false), initialized(false) { }
	virtual ~AbstractScript() {}

public:
	virtual void init() { initialized = true; }
	virtual void release() = 0;
	bool isInit() { return initialized; }
	virtual void update(float dt) = 0;
	//elemA always belongs to the LineSprites instance from the GenerateLayer instance
	//elemB always belongs to the SwapLayer instance
	virtual bool collide(LineInfo::Element elemA, LineInfo::Element elemB) = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	bool isFinished() { return finished; }

protected:
	GameScene * gameScene;
	GenerateLayer * generateLayer;
	SwapLayer * swapLayer;
	void setFinished() { finished = true; }
	bool initialized;

private:
	bool finished;

};

#endif //__ABSTRACT_SCRIPT_H__