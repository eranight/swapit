#include "TutorialScript.h"
#include "GameScene.h"
#include "LineInfo.h"
#include "SwapLayer.h"
#include "LinesLayer.h"
#include "GameScript.h"

USING_NS_CC;

TutorialScript::TutorialScript(GameScene * gameScene) : AbstractScript(gameScene), prompts({ "tap to swap", "allow the square", "tap and hold" }) {
}

void TutorialScript::init() {
	AbstractScript::init();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	promptIsActive = false;
	promptNumber = 0;
	showPromptPosition = origin.y + visibleSize.height * 0.75f;

	promptLabel = Label::create(prompts[promptNumber], "fonts/Marker Felt.ttf", 25);
	promptLabel->setColor(Color3B::BLACK);
	promptLabel->setPosition((origin + visibleSize) * 0.5f);
	promptLabel->setVisible(false);
	gameScene->addChild(promptLabel);

	generateLayer = gameScene->getGenerateLayer();
	//generateLayer->getLineBuilder().setStartConfiguration(LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue));
	//generateLayer->getLineBuilder().setProbabilities(100, 0, 100, 100);

	float velocity = visibleSize.height / 6.0f;

	Vec2 startPosition = Vec2(origin.x, (origin + visibleSize).y);
	generateLayer->setStartPosition(startPosition);
	generateLayer->setFinishPosition(Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize()));
	generateLayer->setVelocity(velocity);
	generateLayer->start();
	showPromptAction = Sequence::createWithTwoActions(DelayTime::create((startPosition - Vec2(0.0f, showPromptPosition)).length() / velocity),
		CallFunc::create(CC_CALLBACK_0(TutorialScript::showPromt, this)));
	showPromptAction->retain();


	velocity = visibleSize.width / (6.0f * 0.75f);
	swapLayer = gameScene->getSwapLayer();
	swapLayer->setVelocity(velocity);
	swapLayer->block();
	prevBallOrder = swapLayer->getBallOrder();
	
	gameScene->runAction(showPromptAction->clone());
}

void TutorialScript::release() {
	gameScene->removeChild(promptLabel);
	showPromptAction->release();
}

void TutorialScript::update(float dt) {
	if (isFinished()) return;
	if (promptIsActive) {
		bool checkCondition = false;
		if (promptNumber < 2) {
			if (swapLayer->getState() == SwapLayer::BallState::StandOnOppositeSides && prevBallOrder != swapLayer->getBallOrder()) {
				checkCondition = true;
				prevBallOrder = swapLayer->getBallOrder();
			}
		}
		else if (promptNumber == 2) {
			if (swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
				checkCondition = true;
			}
		}
		if (checkCondition) {
			promptIsActive = false;
			promptLabel->setVisible(false);
			generateLayer->unblock();
			swapLayer->block();
		}
	}
}

void TutorialScript::showPromt() {
	promptIsActive = true;
	promptLabel->setVisible(true);
	generateLayer->block();
	swapLayer->unblock();
}

bool TutorialScript::collide(LineInfo::Element elemA, LineInfo::Element elemB) {
	if (elemA == elemB) {
		promptNumber++;
		if (promptNumber <= 2) {
			promptLabel->setString(prompts[promptNumber]);
			int prob = promptNumber == 1 ? 100 : 0;
			//generateLayer->getLineBuilder().setProbabilities(prob, prob, prob, prob);
			generateLayer->generateNewLine();
			gameScene->runAction(showPromptAction->clone());
		}
		else {
			swapLayer->unblock();
			setFinished();
		}
		return true;
	}
	return false;
}

void TutorialScript::pause() {
	if (promptIsActive) {
		promptLabel->setVisible(false);
	}
}

void TutorialScript::resume() {
	if (promptIsActive) {
		promptLabel->setVisible(true);
	}
}