#ifndef __ABSTRACT_SCRIPT_H__
#define __ABSTRACT_SCRIPT_H__

#include "cocos2d.h"

class GameScene;

class AbstractScript : public cocos2d::Ref {
public:
	AbstractScript(GameScene * gameScene) { this->gameScene = gameScene; }

public:
	virtual void init() = 0;
	virtual void update(float dt) = 0;

protected:
	GameScene * gameScene;

};

#endif //__ABSTRACT_SCRIPT_H__