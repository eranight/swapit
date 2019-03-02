#include "SwitchableLayer.hpp"
#include "MainMenuScene.hpp"

USING_NS_CC;

bool SwitchableLayer::init() {
	return LayerColor::initWithColor(Color4B(Color3B::WHITE, 50));
}

void SwitchableLayer::switchLayer(const LayerType & layerType) {
	dynamic_cast<MainMenuScene *>(Director::getInstance()->getRunningScene())->switchLayer(layerType);
}