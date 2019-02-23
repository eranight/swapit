#pragma once

#include "cocos2d.h"
#include "LayerType.hpp"

class MainMenuScene : public cocos2d::Scene {
public:
	CREATE_FUNC(MainMenuScene);
public:
	virtual bool init() override;
public:
	void switchLayer(const LayerType &);
private:
	LayerType layerType;
	cocos2d::Map<LayerType, cocos2d::Layer *> layers;
};