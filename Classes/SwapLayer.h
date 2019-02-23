#ifndef __SWAP_LAYER_H__
#define __SWAP_LAYER_H__

#include "cocos2d.h"
#include "Blocking.h"

class SwapLayer : public cocos2d::Layer, Blocking
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

	static const std::string MOVE_TO_CENTER;
	static const std::string MOVE_TO_SIDES;
	static const std::string ARRIVED_TO_CENTER;
	static const std::string ARRIVED_TO_SIDES;

public:
	virtual ~SwapLayer();

public:
	BallState getState() { return state; }

	bool init() override;
	CREATE_FUNC(SwapLayer);

	cocos2d::Vec2 getLinePosition() { return centerPosition; }
	BallOrder getBallOrder() { return ballOrder; }

	const cocos2d::Sprite * getRedBallSprite() { return redBall; }
	const cocos2d::Sprite * getBlueBallSprite() { return blueBall; }
	const cocos2d::Sprite * getVioletBallSprite() { return violetBall; }

	void setVelocity(float velocity);

	void stop();
	void pause() override;
	void resume() override;

	void block() override;
	void unblock() override;

private:
	bool touchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void touchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	void finishAction();
	void setStandInCenterState();
	void setStandOnOppositeSidesState();

	void changeVisibility(float violetVisibility);

private:
	float velocity;
	BallState state;
	BallState nextState;
	int finishedCounter;
	std::map<BallState, std::function<void()>> stateFunctor;
	BallOrder ballOrder;
	cocos2d::Vec2 centerPosition;
	cocos2d::Vec2 leftPosition;
	cocos2d::Vec2 rightPosition;

	cocos2d::Sprite * redBall;
	cocos2d::Sprite * blueBall;
	cocos2d::Sprite * violetBall;

	cocos2d::EventListenerTouchOneByOne * touchListener;
};

#endif // __SWAP_LAYER_H__
