#ifndef __GAME_SCRIPT_H__
#define __GAME_SCRIPT_H__

#include "AbstractScript.h"
#include "LineBuilder.h"

class GameScript : public AbstractScript {
public:
	GameScript(GameScene * gameScene);
	~GameScript() {}

public:
	void init() override;
	void release() override;
	void update(float dt);
	bool collide(LineInfo::Element elemA, LineInfo::Element elemB) override;
	void pause() override;
	void resume() override;

private:
	void setVelocity(float generateLayerVelocity, float swapLayerVelocity);
	void generateNextLine();

private:
	cocos2d::Action * generateNextLineAction;
	cocos2d::FiniteTimeAction * timerAction;
	cocos2d::FiniteTimeAction * callRecalculateAction;
	cocos2d::Vec2 startGeneratedLinePosition;
	cocos2d::Vec2 finishGeneratedLinePosition;
	cocos2d::Vec2 nextGeneratedLinePosition;

private: //score
	int score;
	cocos2d::Label * scoreLabel;
	LineBuilder * lineBuilder;

private:
	cocos2d::Size visibleSize;
	float generateLayerVelocity;
	float swapLayerVelocity;
	bool needUpdateVelocity;

private: //game over
	bool gameOver;

};

#endif //__GAME_SCRIPT_H__