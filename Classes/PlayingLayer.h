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

	enum class BallOrder
	{
		RedBlue,
		BlueRed
	};

public:
	virtual ~PlayingLayer();

public:
	bool init(float leftColumn, float centerColumn, float rightColumn);
	static PlayingLayer * create(float leftColumn, float centerColumn, float rightColumn);
	BallState getState() { return state; }

private:
	bool touchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void touchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	void setStandInCenterState();
	void setStandOnOppositeSidesState();

	void changeVisibility(float violetVisibility);

private:
	float velocity;
	BallState state;
	BallOrder ballOrder;
	cocos2d::Vec2 centerPosition;
	cocos2d::Vec2 leftPosition;
	cocos2d::Vec2 rightPosition;

	cocos2d::Sprite * redBall;
	cocos2d::Sprite * blueBall;
	cocos2d::Sprite * violetBall;
};

#endif // __PLAYING_LAYER_H__
