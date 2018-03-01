#include "TutorialScript.h"
#include "GameScene.h"
#include "LineInfo.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"
#include "GameScript.h"

USING_NS_CC;


TutorialScript::TutorialScript(GameScene * gameScene) : AbstractScript(gameScene), promts({ "tap to swap", "allow the square", "tap and hold" }) {

}

void TutorialScript::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	gameScene->scoreCountOn = false;
	promtIsActive = false;
	promtNumber = 0;
	showPromtPosition = origin.y + visibleSize.height * 0.75f;

	promtLabel = Label::create(promts[promtNumber], "fonts/Marker Felt.ttf", 25);
	promtLabel->setColor(Color3B::BLACK);
	promtLabel->setPosition((origin + visibleSize) * 0.5f);
	promtLabel->setVisible(false);
	gameScene->addChild(promtLabel);

	gameScene->generateLayer->getLineBuilder().setStartConfiguration(LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue));
	gameScene->generateLayer->getLineBuilder().setProbabilities(100, 0, 100, 100);
	gameScene->velocity = visibleSize.height / 6.0f;
	gameScene->generateLayer->setStartPosition(Vec2(origin.x, (origin + visibleSize).y));
	gameScene->generateLayer->setNextGenerationPosition(Vec2(origin.x, origin.y + SPR_MANAGER->getSpriteSize()));
	gameScene->generateLayer->setFinishPosition(Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize()));
	gameScene->generateLayer->setVelocity(gameScene->velocity);
	gameScene->generateLayer->start();

	gameScene->swapLayer->setVelocity(gameScene->velocity);
	gameScene->swapLayer->getEventDispatcher()->pauseEventListenersForTarget(gameScene->swapLayer);

	line = nullptr;
}

void TutorialScript::update(float dt) {
	if (line == nullptr) {
		line = gameScene->generateLayer->getFirstHighLine(showPromtPosition);
	}
	else if (line != nullptr && gameScene->convertToNodeSpace(gameScene->generateLayer->convertToWorldSpace(line->getPosition())).y < showPromtPosition) {
		promtIsActive = true;
		promtLabel->setVisible(true);
		gameScene->generateLayer->pause();
		gameScene->swapLayer->getEventDispatcher()->resumeEventListenersForTarget(gameScene->swapLayer);
	}
	if (promtLabel->isVisible()) {
		bool checkCondition = false;
		if (promtNumber == 0 || promtNumber == 1) {
			const Sprite * sprite = line->getLeftSprite() != nullptr ? line->getLeftSprite() : line->getRightSprite();
			const Sprite * compareSprite = sprite->getColor() == SPR_MANAGER->getColor(LineInfo::Element::red) ? gameScene->swapLayer->getRedBallSprite() : gameScene->swapLayer->getBlueBallSprite();
			float x1 = gameScene->convertToNodeSpace(line->convertToWorldSpace(sprite->getPosition())).x;
			float x2 = gameScene->convertToNodeSpace(gameScene->swapLayer->convertToWorldSpace(compareSprite->getPosition())).x;
			if (gameScene->swapLayer->getState() == SwapLayer::BallState::StandOnOppositeSides && abs(x1 - x2) <= 0.1f) {
				checkCondition = true;
			}
		}
		else if (promtNumber == 2) {
			if (gameScene->swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
				checkCondition = true;
			}
		}
		if (checkCondition) {
			promtLabel->setVisible(false);
			gameScene->generateLayer->resume();
			gameScene->swapLayer->getEventDispatcher()->pauseEventListenersForTarget(gameScene->swapLayer);
			if (++promtNumber < promts.size()) {
				promtLabel->setString(promts[promtNumber]);
				if (promtNumber == 1) {
					gameScene->generateLayer->getLineBuilder().setProbabilities(100, 100, 100, 100);
				}
				else {
					gameScene->generateLayer->getLineBuilder().setProbabilities(0, 0, 0, 0);
				}
				promtIsActive = false;
				line = nullptr;
			}
			else {
				gameScene->removeChild(promtLabel);
				promtLabel = nullptr;
				gameScene->script = new GameScript(gameScene);
				gameScene->script->init();
			}
		}
	}
}