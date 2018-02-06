#include "PlayingLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static const enum class BallZOrder
{
	right = 0,
	left,
	center
};

PlayingLayer::~PlayingLayer()
{
}

bool PlayingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
	state = BallState::StandOnOppositeSides;
	ballOrder = BallOrder::RedBlue;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto spriteBall = Sprite::create("ball.png");
	float ballSize = spriteBall->getContentSize().height * Director::getInstance()->getContentScaleFactor();

	centerPosition = Vec2((origin + visibleSize).x * 0.5f, ballSize);
	leftPosition = Vec2(origin.x + ballSize * 1.5f, ballSize);
	rightPosition = Vec2((origin + visibleSize).x - ballSize * 1.5f, ballSize);

	velocity = (centerPosition - leftPosition).x / 0.7f;

	redBall = Sprite::createWithTexture(spriteBall->getTexture());
	redBall->setColor(Color3B::RED);
	redBall->setPosition(leftPosition);
	this->addChild(redBall, static_cast<int>(BallZOrder::left));

	blueBall = Sprite::createWithTexture(spriteBall->getTexture());
	blueBall->setColor(Color3B::BLUE);
	blueBall->setPosition(rightPosition);
	this->addChild(blueBall, static_cast<int>(BallZOrder::right));

	violetBall = Sprite::createWithTexture(spriteBall->getTexture());
	violetBall->setColor(Color3B::MAGENTA);
	violetBall->setPosition(centerPosition);
	violetBall->setVisible(false);
	this->addChild(violetBall, static_cast<int>(BallZOrder::center));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayingLayer::touchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayingLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool PlayingLayer::touchBegan(Touch * touch, Event * event)
{
	if (state == BallState::StandOnOppositeSides)
	{
		CCLOG("starts moving to center");
		state = BallState::MovingToCenter;
		float timer = (centerPosition - leftPosition).x / velocity;
		redBall->runAction(MoveTo::create(timer, centerPosition));
		blueBall->runAction(MoveTo::create(timer, centerPosition));
		auto sequence = Sequence::createWithTwoActions(DelayTime::create(timer), CallFunc::create(CC_CALLBACK_0(PlayingLayer::setStandInCenterState, this)));
		this->runAction(sequence);
	}
	return true;
}

void PlayingLayer::touchEnded(Touch * touch, Event * event)
{
	if (state == BallState::MovingToCenter || state == BallState::StandInCenter)
	{
		CCLOG("starts moving to opposite sides");
		if (state == BallState::MovingToCenter)
		{
			this->stopAllActions();
		}
		else
		{
			changeVisibility(false);
		}
		state = BallState::MovingToOppositeSides;
		redBall->stopAllActions();
		blueBall->stopAllActions();
		float timer = 0.0f;
		if (ballOrder == BallOrder::RedBlue)
		{
			float t1 = (rightPosition - redBall->getPosition()).x / velocity;
			redBall->runAction(MoveTo::create(t1, rightPosition));
			float t2 = (blueBall->getPosition() - leftPosition).x / velocity;
			blueBall->runAction(MoveTo::create(t2, leftPosition));
			timer = MAX(t1, t2);
		}
		else
		{
			float t1 = (redBall->getPosition() - leftPosition).x / velocity;
			redBall->runAction(MoveTo::create(t1, leftPosition));
			float t2 = (rightPosition - blueBall->getPosition()).x / velocity;
			blueBall->runAction(MoveTo::create(t2, rightPosition));
			timer = MAX(t1, t2);
		}
		auto sequence = Sequence::createWithTwoActions(DelayTime::create(timer), CallFunc::create(CC_CALLBACK_0(PlayingLayer::setStandOnOppositeSidesState, this)));
		this->runAction(sequence);
	}
}

void PlayingLayer::setStandInCenterState()
{
	state = BallState::StandInCenter;
	changeVisibility(true);
}

void PlayingLayer::setStandOnOppositeSidesState()
{
	state = BallState::StandOnOppositeSides;
	ballOrder = ballOrder == BallOrder::RedBlue ?
		(redBall->setZOrder(static_cast<int>(BallZOrder::right)),
		blueBall->setZOrder(static_cast<int>(BallZOrder::left)),
		BallOrder::BlueRed) :
		(redBall->setZOrder(static_cast<int>(BallZOrder::left)),
		blueBall->setZOrder(static_cast<int>(BallZOrder::right)),
		BallOrder::RedBlue);
}

void PlayingLayer::changeVisibility(float violetVisibility)
{
	violetBall->setVisible(violetVisibility);
	redBall->setVisible(!violetVisibility);
	blueBall->setVisible(!violetVisibility);
}