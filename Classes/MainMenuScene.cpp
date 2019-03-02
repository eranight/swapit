#include "MainMenuScene.hpp"
#include "MainMenuLayer.hpp"
#include "BackableLayer.hpp"

USING_NS_CC;

bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	layerType = LayerType::MENU;

	auto menuLayer = MainMenuLayer::create();
	layers.insert(LayerType::MENU, menuLayer);
	menuLayer->setVisible(false);
	this->addChild(menuLayer);

	auto config = Configuration::getInstance();
	auto label = Label::create(config->getValue("author").asString(), config->getValue("font").asString(), config->getValue("fontSize").asFloat());
	label->setColor(Color3B::BLACK);
	auto authorLayer = BackableLayer::create(label);
	layers.insert(LayerType::AUTHORS, authorLayer);
	authorLayer->setVisible(false);
	this->addChild(authorLayer);

	switchLayer(LayerType::MENU);
	return true;
}

void MainMenuScene::switchLayer(const LayerType & layerType) {
	layers.at(this->layerType)->setVisible(false);
	this->layerType = layerType;
	layers.at(this->layerType)->setVisible(true);
}