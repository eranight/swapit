#include "MainMenuScene.hpp"
#include "LinesLayer.h"
#include "MainMenuLayer.hpp"
#include "BackableLayer.hpp"

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

	layerType = LayerType::MENU;
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

	auto menuLayer = MainMenuLayer::create();
	layers.insert(LayerType::MENU, menuLayer);
	menuLayer->setVisible(false);
	this->addChild(menuLayer);

	auto label = Label::create(config->getValue("author").asString(), config->getValue("font").asString(), config->getValue("fontSize").asFloat());
	label->setColor(Color3B::BLACK);
	auto backableLayer = BackableLayer::create(label);
	layers.insert(LayerType::AUTHORS, backableLayer);
	backableLayer->setVisible(false);
	this->addChild(backableLayer);

	label = Label::create("best score", config->getValue("font").asString(), config->getValue("fontSize").asFloat());
	label->setColor(Color3B::BLACK);
	backableLayer = BackableLayer::create(label);
	layers.insert(LayerType::RECORD, backableLayer);
	backableLayer->setVisible(false);
	this->addChild(backableLayer);

	return true;
}

void MainMenuScene::onEnter() {
	Scene::onEnter();
	genetateNextLine();
	switchLayer(LayerType::MENU);
}

void MainMenuScene::switchLayer(const LayerType & layerType) {
	layers.at(this->layerType)->setVisible(false);
	this->layerType = layerType;
	layers.at(this->layerType)->setVisible(true);
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