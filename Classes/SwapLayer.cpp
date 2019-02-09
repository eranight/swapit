#include "SwapLayer.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"

USING_NS_CC;

const std::string SwapLayer::MOVE_TO_CENTER = "move to center";
const std::string SwapLayer::MOVE_TO_SIDES = "move to sides";
const std::string SwapLayer::ARRIVED_TO_CENTER = "arrived to center";
const std::string SwapLayer::ARRIVED_TO_SIDES = "arrived to sides";

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

	blocked = false;
	finishedCounter = 0;
	stateFunctor[BallState::StandInCenter] = CC_CALLBACK_0(SwapLayer::setStandInCenterState, this);
	stateFunctor[BallState::StandOnOppositeSides] = CC_CALLBACK_0(SwapLayer::setStandOnOppositeSidesState, this);
	
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

	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SwapLayer::touchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SwapLayer::touchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool SwapLayer::touchBegan(Touch * touch, Event * event)
{
	if (state == BallState::StandOnOppositeSides)
	{
		state = BallState::MovingToCenter;
		float timer = (centerPosition - leftPosition).x / velocity;
		auto sequence = Sequence::createWithTwoActions(MoveTo::create(timer, centerPosition), CallFunc::create(CC_CALLBACK_0(SwapLayer::finishAction, this)));
		redBall->runAction(sequence->clone());
		blueBall->runAction(sequence->clone());
		nextState = BallState::StandInCenter;
		getEventDispatcher()->dispatchCustomEvent(MOVE_TO_CENTER);
	}
	return true;
}

void SwapLayer::touchEnded(Touch * touch, Event * event)
{
	if (state == BallState::MovingToCenter || state == BallState::StandInCenter)
	{
		if (state == BallState::MovingToCenter) {
			redBall->stopAllActions();
			blueBall->stopAllActions();
		}
		else {
			changeVisibility(false);
		}
		state = BallState::MovingToOppositeSides;
		nextState = BallState::StandOnOppositeSides;
		finishedCounter = 0;
		float timer;
		Vec2 redBallTargetPosition;
		Vec2 blueBallTargetPosition;
		if (ballOrder == BallOrder::RedBlue)
		{
			redBallTargetPosition = rightPosition;
			blueBallTargetPosition = leftPosition;
			timer = (rightPosition - redBall->getPosition()).x / velocity;
		}
		else
		{
			redBallTargetPosition = leftPosition;
			blueBallTargetPosition = rightPosition;
			timer = (rightPosition - blueBall->getPosition()).x / velocity;
		}
		redBall->runAction(Sequence::createWithTwoActions(MoveTo::create(timer, redBallTargetPosition), CallFunc::create(CC_CALLBACK_0(SwapLayer::finishAction, this))));
		blueBall->runAction(Sequence::createWithTwoActions(MoveTo::create(timer, blueBallTargetPosition), CallFunc::create(CC_CALLBACK_0(SwapLayer::finishAction, this))));
		getEventDispatcher()->dispatchCustomEvent(MOVE_TO_SIDES);
	}
}

void SwapLayer::finishAction() {
	if (++finishedCounter == 2) {
		stateFunctor[nextState]();
		finishedCounter = 0;
	}
}

void SwapLayer::setStandInCenterState()
{
	state = BallState::StandInCenter;
	changeVisibility(true);
	getEventDispatcher()->dispatchCustomEvent(ARRIVED_TO_CENTER);
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
	getEventDispatcher()->dispatchCustomEvent(ARRIVED_TO_SIDES);
}

void SwapLayer::changeVisibility(float violetVisibility)
{
	violetBall->setVisible(violetVisibility);
	redBall->setVisible(!violetVisibility);
	blueBall->setVisible(!violetVisibility);
}

void SwapLayer::setVelocity(float velocity) {
	this->velocity = velocity;
}

void SwapLayer::stop() {
	redBall->stopAllActions();
	blueBall->stopAllActions();
	this->stopAllActions();
	_eventDispatcher->removeEventListenersForTarget(this);
}

void SwapLayer::block() {
	Blocking::block();
	touchListener->setEnabled(false);
}

void SwapLayer::unblock() {
	Blocking::unblock();
	touchListener->setEnabled(true);
	if (state == BallState::StandInCenter) {
		touchEnded(nullptr, nullptr);
	}
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
}