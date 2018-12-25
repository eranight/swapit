#include "TutorialScript.h"
#include "GameScene.h"
#include "LineInfo.h"
#include "SwapLayer.h"
#include "GenerateLayer.h"
#include "GameScript.h"

USING_NS_CC;

TutorialScript::TutorialScript(GameScene * gameScene) : AbstractScript(gameScene), promts({ "tap to swap", "allow the square", "tap and hold" }) {
}

TutorialScript::~TutorialScript() {
	gameScene->removeChild(promtLabel);
}

void TutorialScript::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	promtIsActive = false;
	promtNumber = 0;
	showPromtPosition = origin.y + visibleSize.height * 0.75f;

	promtLabel = Label::create(promts[promtNumber], "fonts/Marker Felt.ttf", 25);
	promtLabel->setColor(Color3B::BLACK);
	promtLabel->setPosition((origin + visibleSize) * 0.5f);
	promtLabel->setVisible(false);
	gameScene->addChild(promtLabel);

	generateLayer = gameScene->getGenerateLayer();
	generateLayer->getLineBuilder().setStartConfiguration(LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue));
	generateLayer->getLineBuilder().setProbabilities(100, 0, 100, 100);

	float velocity = visibleSize.height / 6.0f;

	generateLayer->setStartPosition(Vec2(origin.x, (origin + visibleSize).y));
	generateLayer->setFinishPosition(Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize()));
	generateLayer->setVelocity(velocity);
	generateLayer->start();

	velocity = visibleSize.width / (6.0f * 0.75f);
	swapLayer = gameScene->getSwapLayer();
	swapLayer->setVelocity(velocity);

	line = nullptr;
}

void TutorialScript::update(float dt) {
	if (isFinished()) return;
	if (line == nullptr) {
		line = generateLayer->getFirstHighLine(showPromtPosition);
		swapLayer->pause();
	}
	else if (!promtIsActive && gameScene->convertToNodeSpace(generateLayer->convertToWorldSpace(line->getPosition())).y < showPromtPosition) {
		promtIsActive = true;
		promtLabel->setVisible(true);
		generateLayer->pause();
		swapLayer->resume();
	}
	if (promtIsActive) {
		bool checkCondition = false;
		if (promtNumber == 0 || promtNumber == 1) {
			const Sprite * sprite = line->getLeftSprite() != nullptr ? line->getLeftSprite() : line->getRightSprite();
			const Sprite * compareSprite = sprite->getColor() == SPR_MANAGER->getColor(LineInfo::Element::red) ? swapLayer->getRedBallSprite() : swapLayer->getBlueBallSprite();
			float x1 = gameScene->convertToNodeSpace(line->convertToWorldSpace(sprite->getPosition())).x;
			float x2 = gameScene->convertToNodeSpace(swapLayer->convertToWorldSpace(compareSprite->getPosition())).x;
			if (swapLayer->getState() == SwapLayer::BallState::StandOnOppositeSides && abs(x1 - x2) <= 0.1f) {
				checkCondition = true;
				swapLayer->pause();
			}
		}
		else if (promtNumber == 2) {
			if (swapLayer->getState() == SwapLayer::BallState::StandInCenter) {
				checkCondition = true;
				swapLayer->pause();
			}
		}
		if (checkCondition) {
			promtLabel->setVisible(false);
			generateLayer->resume();
		}
	}
}

bool TutorialScript::collide(LineInfo::Element elemA, LineInfo::Element elemB) {
	if (elemA == elemB) {
		promtIsActive = false;
		line = nullptr;
		if (++promtNumber > 2) {
			setFinished();
		}
		else {
			promtLabel->setString(promts[promtNumber]);
			if (promtNumber == 1) {
				generateLayer->getLineBuilder().setProbabilities(100, 100, 100, 100);
			}
			else if (promtNumber == 2) {
				generateLayer->getLineBuilder().setProbabilities(0, 0, 0, 0);
			}
			else {
				setFinished();
			}
			generateLayer->generateNewLine();
		}
		return true;
	}
	return false;
}

void TutorialScript::pause() {
	if (promtIsActive) {
		promtLabel->setVisible(false);
	}
}

void TutorialScript::resume() {
	if (promtIsActive) {
		promtLabel->setVisible(true);
	}
}