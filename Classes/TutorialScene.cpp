#include "TutorialScene.hpp"
#include "SceneFactory.hpp"

USING_NS_CC;

TutorialScene::Prompt::Prompt(const std::string & text, const std::string & awaitedEvent) : text(text), awaitedEvent(awaitedEvent) {

}

TutorialScene::TutorialScene() : lineSupplier(nullptr) {
	prompts.push(Prompt("tap to swap", SwapLayer::ARRIVED_TO_SIDES));
	prompts.push(Prompt("allow the square", SwapLayer::ARRIVED_TO_SIDES));
	prompts.push(Prompt("tap and hold", SwapLayer::ARRIVED_TO_CENTER));
}

TutorialScene::~TutorialScene() {
	if (collisionDetector != nullptr) {
		collisionDetector->release();
	}
	if (lineSupplier != nullptr) {
		delete lineSupplier;
	}
}

bool TutorialScene::init() {
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	lineSupplier = new TutorialLineSupplier({
		LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue),
		LineInfo(LineInfo::Element::blue, LineInfo::Element::green, LineInfo::Element::red),
		LineInfo(LineInfo::Element::none, LineInfo::Element::violet, LineInfo::Element::none) });

	swapLayer = SwapLayer::create();
	swapLayer->pause();
	swapLayer->setVelocity(visibleSize.width * 0.25);
	this->addChild(swapLayer);

	linesLayer = LinesLayer::create(lineSupplier);
	linesLayer->generateNewLine();
	this->addChild(linesLayer);

	collisionDetector =  CollisionDetector::create(swapLayer, linesLayer, nullptr);
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
	linesLayer->pause();
	swapLayer->resume();
	promptLabel->setString(prompts.front().text);
	promptLabel->setVisible(true);
	getEventDispatcher()->addCustomEventListener(prompts.front().awaitedEvent, CC_CALLBACK_0(TutorialScene::hidePrompt, this));
}

void TutorialScene::hidePrompt() {
	linesLayer->resume();
	swapLayer->pause();
	promptLabel->setVisible(false);
	getEventDispatcher()->removeCustomEventListeners(prompts.front().awaitedEvent);
	prompts.pop();
}

TutorialLineSupplier::TutorialLineSupplier(const std::initializer_list<LineInfo> & lines) {
	for (auto line : lines) {
		lineQueue.push(line);
	}
}

LineInfo TutorialLineSupplier::getNextLine() {
	auto lineInfo = lineQueue.front();
	lineQueue.pop();
	return lineInfo;
}