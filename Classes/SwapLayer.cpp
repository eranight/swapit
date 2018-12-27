#include "SwapLayer.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"

USING_NS_CC;

static const enum class BallZOrder
{
	right = 0,
	left,
	center
};

SwapLayer::~SwapLayer() {}

bool SwapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
	state = BallState::StandOnOppositeSides;
	ballOrder = BallOrder::RedBlue;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
	float ballSize = SPR_MANAGER->getSpriteSize();

	centerPosition = Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::center), ballSize);
	leftPosition = Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::left), ballSize);
	rightPosition = Vec2(SPR_MANAGER->getColumn(SpriteManager::SpriteColumn::right), ballSize);

	redBall = SPR_MANAGER->getSprite(LineInfo::Element::red);
	redBall->setPosition(leftPosition);
	this->addChild(redBall, static_cast<int>(BallZOrder::left));

	blueBall = SPR_MANAGER->getSprite(LineInfo::Element::blue);
	blueBall->setPosition(rightPosition);
	this->addChild(blueBall, static_cast<int>(BallZOrder::right));

	violetBall = SPR_MANAGER->getSprite(LineInfo::Element::violet);
	violetBall->setPosition(centerPosition);
	violetBall->setVisible(false);
	this->addChild(violetBall, static_cast<int>(BallZOrder::center));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SwapLayer::touchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SwapLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void SwapLayer::stop() {
	redBall->stopAllActions();
	blueBall->stopAllActions();
	this->stopAllActions();
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool SwapLayer::touchBegan(Touch * touch, Event * event)
{
	if (state == BallState::StandOnOppositeSides)
	{
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

void SwapLayer::setVelocity(float velocity) {
	this->velocity = velocity * 1.3f;
}

void SwapLayer::pause() {
	Layer::pause();
	redBall->pause();
	blueBall->pause();
}

void SwapLayer::resume() {
	Layer::resume();
	redBall->resume();
	blueBall->resume();
	if (state == BallState::StandInCenter) {
		touchEnded(nullptr, nullptr);
	}
}