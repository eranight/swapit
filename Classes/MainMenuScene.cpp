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
	lineSupplier = new RepeatedLineSupplier({
		LineInfo(LineInfo::Element::blue, LineInfo::Element::none, LineInfo::Element::none),
		LineInfo(LineInfo::Element::none, LineInfo::Element::none, LineInfo::Element::red),
		LineInfo(LineInfo::Element::none, LineInfo::Element::violet, LineInfo::Element::none),
		LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue),
		LineInfo(LineInfo::Element::blue, LineInfo::Element::green, LineInfo::Element::none),
		LineInfo(LineInfo::Element::green, LineInfo::Element::violet, LineInfo::Element::green),
		LineInfo(LineInfo::Element::none, LineInfo::Element::green, LineInfo::Element::red),
		LineInfo(LineInfo::Element::red, LineInfo::Element::green, LineInfo::Element::blue),
		LineInfo(LineInfo::Element::green, LineInfo::Element::violet, LineInfo::Element::green)
	});

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 startPosition = Vec2(origin.x, (origin + visibleSize).y + SPR_MANAGER->getSpriteSize());
	Vec2 finishPosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());
	
	auto velocity = visibleSize.height * 0.25f;
	float time = (visibleSize.height - SPR_MANAGER->getSpriteSize()) / velocity;
	linesLayer = LinesLayer::create(lineSupplier);
	linesLayer->setStartPosition(startPosition);
	linesLayer->setFinishPosition(finishPosition);
	linesLayer->setVelocity(velocity);
	linesLayer->start();
	this->addChild(linesLayer);
	nextLineTimer = Sequence::createWithTwoActions(
		DelayTime::create(time),
		CallFunc::create(CC_CALLBACK_0(MainMenuScene::genetateNextLine, this)));
	nextLineTimer->retain();

	auto menuLayer = MainMenuLayer::create();
	layers.insert(LayerType::MENU, menuLayer);
	menuLayer->setVisible(false);
	this->addChild(menuLayer);

	auto config = Configuration::getInstance();
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

	switchLayer(LayerType::MENU);
	this->runAction(nextLineTimer->clone());
	return true;
}

void MainMenuScene::switchLayer(const LayerType & layerType) {
	layers.at(this->layerType)->setVisible(false);
	this->layerType = layerType;
	layers.at(this->layerType)->setVisible(true);
}

void MainMenuScene::genetateNextLine() {
	linesLayer->generateNewLine();
	this->runAction(nextLineTimer->clone());
}

RepeatedLineSupplier::RepeatedLineSupplier(const std::initializer_list<LineInfo> & lines) {
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