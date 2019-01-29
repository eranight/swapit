#include "TutorialScene.hpp"
#include "SceneFactory.hpp"

USING_NS_CC;

TutorialScene::Prompt::Prompt(const std::string & text, const std::string & awaitedEvent) : text(text), awaitedEvent(awaitedEvent) {

}

TutorialScene::TutorialScene() {
	prompts.push(Prompt("tap to swap", SwapLayer::ARRIVED_TO_SIDES));
	prompts.push(Prompt("allow the square", SwapLayer::ARRIVED_TO_SIDES));
	prompts.push(Prompt("tap and hold", SwapLayer::ARRIVED_TO_CENTER));
}

TutorialScene::~TutorialScene() {
	if (collisionDetector != nullptr) {
		collisionDetector->release();
	}
}

bool TutorialScene::init() {
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	swapLayer = SwapLayer::create();
	swapLayer->pause();
	swapLayer->setVelocity(visibleSize.width * 0.25);
	this->addChild(swapLayer);

	generateLayer = GenerateLayer::create();
	this->addChild(generateLayer);

	collisionDetector =  CollisionDetector::create(swapLayer, generateLayer, nullptr);
	if (collisionDetector != nullptr) {
		collisionDetector->retain();
	}
	else {
		return false;
	}

	promptLabel = Label::create("", "fonts/Marker Felt.ttf", 25);
	promptLabel->setColor(Color3B::BLACK);
	promptLabel->setPosition((origin + visibleSize) * 0.5f);
	promptLabel->setVisible(false);
	this->addChild(promptLabel);

	auto skipLabel = Label::create("skip tutorial", "fonts/Marker Felt.ttf", 25);
	auto skipItem = MenuItemLabel::create(skipLabel, CC_CALLBACK_1(TutorialScene::menuSkipCallback, this));
	skipItem->setColor(Color3B::BLACK);
	skipItem->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + skipItem->getContentSize().height * 0.5f));
	menu = Menu::createWithItem(skipItem);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void TutorialScene::update(float deltaTime) {

}

void TutorialScene::menuSkipCallback(Ref * sender) {
	Director::getInstance()->replaceScene(SceneFactory::createGameScene());
}

bool TutorialScene::collide(const LineInfo::Element & elemA, const LineInfo::Element & elemB) {

	return true;
}

void TutorialScene::showPrompt() {
	generateLayer->pause();
	swapLayer->resume();
	promptLabel->setString(prompts.front().text);
	promptLabel->setVisible(true);
	getEventDispatcher()->addCustomEventListener(prompts.front().awaitedEvent, CC_CALLBACK_0(TutorialScene::hidePrompt, this));
}

void TutorialScene::hidePrompt() {
	generateLayer->resume();
	swapLayer->pause();
	promptLabel->setVisible(false);
	getEventDispatcher()->removeCustomEventListeners(prompts.front().awaitedEvent);
	prompts.pop();
}

LineSprites * TutorialLineSupplier::getNextLine() {
	auto lineInfo = lineQueue.front();
	return nullptr;
}