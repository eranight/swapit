#include "MainMenuScene.hpp"
#include "MainMenuLayer.hpp"

USING_NS_CC;

bool MainMenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	layerType = LayerType::MENU;

	auto menuLayer = MainMenuLayer::create();
	layers.insert(LayerType::MENU, menuLayer);
	this->addChild(menuLayer);

	switchLayer(LayerType::MENU);
	return true;
}

void MainMenuScene::switchLayer(const LayerType & layerType) {
	layers.at(this->layerType)->setVisible(false);
	this->layerType = layerType;
	layers.at(this->layerType)->setVisible(true);
}