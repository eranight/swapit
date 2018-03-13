#ifndef __ABSTRACT_SCRIPT_H__
#define __ABSTRACT_SCRIPT_H__

#include "cocos2d.h"
#include "LineInfo.h"

class GameScene;
class GenerateLayer;
class SwapLayer;

class AbstractScript : public cocos2d::Ref {
public:
	AbstractScript(GameScene * gameScene) { this->gameScene = gameScene; }
	virtual ~AbstractScript() {}

public:
	virtual void init() = 0;
	virtual void update(float dt) = 0;
	//elemA always belongs to the LineSprites instance from the GenerateLayer instance
	//elemB always belongs to the SwapLayer instance
	virtual bool collide(LineInfo::Element elemA, LineInfo::Element elemB) = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

protected:
	GameScene * gameScene;
};

#endif //__ABSTRACT_SCRIPT_H__