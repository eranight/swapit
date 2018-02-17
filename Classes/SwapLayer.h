#ifndef __SWAP_LAYER_H__
#define __SWAP_LAYER_H__

#include "cocos2d.h"

class SwapLayer : public cocos2d::Layer
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
	virtual ~SwapLayer();

public:
	BallState getState() { return state; }

	bool init() override;
	CREATE_FUNC(SwapLayer);

	float getLinePosition() { return centerPosition.y; }

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

#endif // __SWAP_LAYER_H__
