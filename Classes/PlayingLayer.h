#ifndef __PLAYING_LAYER_H__
#define __PLAYING_LAYER_H__

#include "cocos2d.h"

class PlayingLayer : public cocos2d::Layer
{
public:
	enum class BallState
	{
		MovingToCenter,
		MovingToOppositeSides,
		StandInCenter,
		StandOnOppositeSides
	};

public:
	virtual ~PlayingLayer();

public:
    virtual bool init();
	CREATE_FUNC(PlayingLayer);
	BallState getState() { return state; }

private:
	bool touchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void touchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

private:
	BallState state;
	cocos2d::MoveBy * moveToCenterAction;

	cocos2d::Sprite * redBall;
	cocos2d::Sprite * blueBall;
	cocos2d::Sprite * violetBall;
};

#endif // __PLAYING_LAYER_H__
