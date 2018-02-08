#include "SwapLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static const enum class BallZOrder
{
	right = 0,
	left,
	center
};

SwapLayer::~SwapLayer()
{
}

SwapLayer * SwapLayer::create(float leftColumn, float centerColumn, float rightColumn)
{
	SwapLayer * pRet = new (std::nothrow) SwapLayer();
	if (pRet && pRet->init(leftColumn, centerColumn, rightColumn))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool SwapLayer::init(float leftColumn, float centerColumn, float rightColumn)
{
	if (!Layer::init())
	{
		return false;
	}
    
	state = BallState::StandOnOppositeSides;
	ballOrder = BallOrder::RedBlue;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Texture2D * tx = Director::getInstance()->getTextureCache()->addImage("ball.png");
	float ballSize = tx->getContentSize().height * Director::getInstance()->getContentScaleFactor();

	centerPosition = Vec2(centerColumn, ballSize);
	leftPosition = Vec2(leftColumn, ballSize);
	rightPosition = Vec2(rightColumn, ballSize);

	velocity = (centerPosition - leftPosition).x / 0.7f;

	redBall = Sprite::createWithTexture(tx);
	redBall->setColor(Color3B::RED);
	redBall->setPosition(leftPosition);
	this->addChild(redBall, static_cast<int>(BallZOrder::left));

	blueBall = Sprite::createWithTexture(tx);
	blueBall->setColor(Color3B::BLUE);
	blueBall->setPosition(rightPosition);
	this->addChild(blueBall, static_cast<int>(BallZOrder::right));

	violetBall = Sprite::createWithTexture(tx);
	violetBall->setColor(Color3B::MAGENTA);
	violetBall->setPosition(centerPosition);
	violetBall->setVisible(false);
	this->addChild(violetBall, static_cast<int>(BallZOrder::center));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SwapLayer::touchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SwapLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool SwapLayer::touchBegan(Touch * touch, Event * event)
{
	if (state == BallState::StandOnOppositeSides)
	{
		CCLOG("starts moving to center");
		state = BallState::MovingToCenter;
		float timer = (centerPosition - leftPosition).x / velocity;
		redBall->runAction(MoveTo::create(timer, centerPosition));
		blueBall->runAction(MoveTo::create(timer, centerPosition));
		auto sequence = Sequence::createWithTwoActions(DelayTime::create(timer), CallFunc::create(CC_CALLBACK_0(SwapLayer::setStandInCenterState, this)));
		this->runAction(sequence);
	}
	return true;
}

void SwapLayer::touchEnded(Touch * touch, Event * event)
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
		auto sequence = Sequence::createWithTwoActions(DelayTime::create(timer), CallFunc::create(CC_CALLBACK_0(SwapLayer::setStandOnOppositeSidesState, this)));
		this->runAction(sequence);
	}
}

void SwapLayer::setStandInCenterState()
{
	state = BallState::StandInCenter;
	changeVisibility(true);
}

void SwapLayer::setStandOnOppositeSidesState()
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

void SwapLayer::changeVisibility(float violetVisibility)
{
	violetBall->setVisible(violetVisibility);
	redBall->setVisible(!violetVisibility);
	blueBall->setVisible(!violetVisibility);
}