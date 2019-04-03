#include "MainMenuScene.hpp"
#include "LinesLayer.h"
#include "MainMenuLayer.hpp"
#include "BackableLayer.hpp"

USING_NS_CC;

MainMenuScene::~MainMenuScene() {
	if (lineSupplier != nullptr) {
		delete lineSupplier;
		lineSupplier = nullptr;
	}
}

bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	layerType = LayerType::MENU;
	lineSupplier = new RepeatedLineSupplier({
		LineInfo(LineInfo::Element::blue, LineInfo::Element::none, LineInfo::Element::none),
		LineInfo(LineInfo::Element::red, LineInfo::Element::green, LineInfo::Element::none),
		LineInfo(LineInfo::Element::none, LineInfo::Element::violet, LineInfo::Element::none) });

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	auto linesLayer = LinesLayer::create(lineSupplier);
	this->addChild(linesLayer);

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
	return true;
}

void MainMenuScene::switchLayer(const LayerType & layerType) {
	layers.at(this->layerType)->setVisible(false);
	this->layerType = layerType;
	layers.at(this->layerType)->setVisible(true);
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