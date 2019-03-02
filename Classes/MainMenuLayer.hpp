#pragma once

#include "cocos2d.h"
#include "SwitchableLayer.hpp"

class MainMenuLayer : public SwitchableLayer {
public:
	CREATE_FUNC(MainMenuLayer);
public:
	bool init() override;
private:
	cocos2d::Menu * menu;
};