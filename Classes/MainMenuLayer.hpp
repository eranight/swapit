#pragma once

#include "cocos2d.h"
#include "LayerType.hpp"

class MainMenuLayer : public cocos2d::LayerColor {
public:
	CREATE_FUNC(MainMenuLayer);
public:
	bool init() override;
public:
	void switchLayer(const LayerType &);
private:
	cocos2d::Menu * menu;
};