#include "PlayingLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static const float tocentertime = 0.7f;

PlayingLayer::~PlayingLayer()
{
	moveToCenterAction->release();
}

bool PlayingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
	state = BallState::StandOnOppositeSides;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto spriteBall = Sprite::create("ball.png");
	float ballSize = spriteBall->getContentSize().height * Director::getInstance()->getContentScaleFactor();

	redBall = Sprite::createWithTexture(spriteBall->getTexture());
	redBall->setAnchorPoint(Vec2(0.0f, 0.0f));
	redBall->setColor(Color3B::RED);
	redBall->setPosition(Vec2(ballSize, ballSize));
	this->addChild(redBall);

	blueBall = Sprite::createWithTexture(spriteBall->getTexture());
	blueBall->setAnchorPoint(Vec2(0.0f, 0.0f));
	blueBall->setColor(Color3B::BLUE);
	blueBall->setPosition(Vec2((origin + visibleSize).x - ballSize * 2.0f, ballSize));
	this->addChild(blueBall);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayingLayer::touchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayingLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	moveToCenterAction = MoveBy::create(tocentertime, Vec2((origin + visibleSize).x * 0.5f - 1.5f * ballSize, 0.0f));
	moveToCenterAction->retain();

    return true;
}

bool PlayingLayer::touchBegan(Touch * touch, Event * event)
{
	if (state == BallState::StandOnOppositeSides)
	{
		CCLOG("starts moving to center");
		state = BallState::MovingToCenter;
		redBall->runAction(moveToCenterAction->clone());
		blueBall->runAction(moveToCenterAction->reverse());
	}
	else
	{
		CCLOG("wrong state");
	}
	return true;
}

void PlayingLayer::touchEnded(Touch * touch, Event * event)
{
	if (state == BallState::MovingToCenter || state == BallState::StandInCenter)
	{
		CCLOG("starts moving to opposite sides");
		state = BallState::MovingToOppositeSides;
	}
	else
	{
		CCLOG("wrong state");
	}
}
