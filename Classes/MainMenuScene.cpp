#include "MainMenuScene.hpp"
#include "LinesLayer.h"
#include "SceneFactory.hpp"

USING_NS_CC;

MainMenuScene::~MainMenuScene() {
	SPR_MANAGER->release();
	if (lineSupplier != nullptr) {
		delete lineSupplier;
		lineSupplier = nullptr;
	}
	nextLineTimer->release();
}

bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	SPR_MANAGER->retain();

	Configuration * config = Configuration::getInstance();
	auto valueVector = config->getValue("mainMenuLines").asValueVector();
	std::vector<LineInfo> values = std::vector<LineInfo>();
	for (auto iter : valueVector) {
		values.push_back(LineInfo(iter));
	}
	lineSupplier = new RepeatedLineSupplier(values);

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = (origin + visibleSize) * 0.5f;
	Vec2 startPosition = Vec2(origin.x, (origin + visibleSize).y + SPR_MANAGER->getSpriteSize());
	Vec2 finishPosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());
	
	auto velocity = visibleSize.height * 0.25f;
	float time = (visibleSize.height - SPR_MANAGER->getSpriteSize()) / velocity;
	auto linesLayer = LinesLayer::create({ startPosition, finishPosition, velocity}, lineSupplier);
	this->addChild(linesLayer);
	nextLineTimer = Sequence::createWithTwoActions(
		DelayTime::create(time),
		CallFunc::create(CC_CALLBACK_0(MainMenuScene::genetateNextLine, this)));
	nextLineTimer->retain();

	auto mainMenuLayer = LayerColor::create(Color4B(Color3B::WHITE, 50));
	auto authorsLayer = LayerColor::create(Color4B(Color3B::WHITE, 50));
	auto recordLayer = LayerColor::create(Color4B(Color3B::WHITE, 50));

	// main menu layer
	auto playItem = MenuItemImage::create("playNormal.png", "playSelected.png", [this](Ref * ref) -> void {
		Director::getInstance()->replaceScene(SceneFactory::createTutorialScene());
	});
	playItem->setPosition(Vec2(center.x, center.y + SPR_MANAGER->getSpriteSize() * 1.5f));

	auto recordItem = MenuItemImage::create("recordNormal.png", "recordSelected.png", [mainMenuLayer, recordLayer](Ref * ref) -> void {
		mainMenuLayer->setVisible(false);
		recordLayer->setVisible(true);
	});
	recordItem->setPosition(Vec2(center.x - SPR_MANAGER->getSpriteSize(), center.y));

	auto authorItem = MenuItemImage::create("authorsNormal.png", "authorsSelected.png", [mainMenuLayer, authorsLayer](Ref * ref) -> void {
		mainMenuLayer->setVisible(false);
		authorsLayer->setVisible(true);
	});
	authorItem->setPosition(Vec2(center.x + SPR_MANAGER->getSpriteSize(), center.y));

	auto closeItem = MenuItemImage::create("closeNormal.png", "closeSelected.png", [this](Ref * ref) -> void {
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	});
	closeItem->setPosition(Vec2(center.x, center.y - SPR_MANAGER->getSpriteSize()));
	auto menu = Menu::create(playItem, recordItem, authorItem, closeItem, nullptr);
	menu->setPosition(origin);
	mainMenuLayer->addChild(menu);
	mainMenuLayer->setVisible(true);
	this->addChild(mainMenuLayer);

	// record layer
	auto label = Label::create(config->getValue("author").asString(), config->getValue("font").asString(), config->getValue("fontSize").asFloat());
	label->setColor(Color3B::BLACK);
	label->setPosition(center);
	authorsLayer->addChild(label);
	auto backItem = MenuItemImage::create("backNormal.png", "backSelected.png", [mainMenuLayer, authorsLayer](Ref * ref) -> void {
		mainMenuLayer->setVisible(true);
		authorsLayer->setVisible(false);
	});
	backItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backItem->setPosition(origin);
	menu = Menu::create(backItem, nullptr);
	menu->setPosition(origin);
	authorsLayer->addChild(menu);
	authorsLayer->setVisible(false);
	this->addChild(authorsLayer);

	// authors layer
	label = Label::create("best score", config->getValue("font").asString(), config->getValue("fontSize").asFloat());
	label->setColor(Color3B::BLACK);
	label->setPosition(center);
	recordLayer->addChild(label);
	backItem = MenuItemImage::create("backNormal.png", "backSelected.png", [mainMenuLayer, recordLayer](Ref * ref) -> void {
		mainMenuLayer->setVisible(true);
		recordLayer->setVisible(false);
	});
	backItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backItem->setPosition(origin);
	menu = Menu::create(backItem, nullptr);
	menu->setPosition(origin);
	recordLayer->addChild(menu);
	recordLayer->setVisible(false);
	this->addChild(recordLayer);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
			Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void MainMenuScene::onEnter() {
	Scene::onEnter();
	genetateNextLine();
}

void MainMenuScene::genetateNextLine() {
	getEventDispatcher()->dispatchCustomEvent(LinesLayer::GENERATE_NEW_LINE_EVENT);
	this->runAction(nextLineTimer->clone());
}

RepeatedLineSupplier::RepeatedLineSupplier(const std::vector<LineInfo> & lines) {
	for (auto line : lines) {
		lineQueue.push(line);
	}
}

LineInfo RepeatedLineSupplier::getNextLine() {
	auto lineInfo = lineQueue.front();
	lineQueue.pop();
	lineQueue.push(lineInfo);
	return lineInfo;
}