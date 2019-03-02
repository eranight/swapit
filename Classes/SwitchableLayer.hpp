#pragma once

#include "cocos2d.h"
#include "LayerType.hpp"

class SwitchableLayer : public cocos2d::LayerColor {
public:
	void switchLayer(const LayerType &);
};