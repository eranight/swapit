#pragma once

#include "cocos2d.h"
#include "SwitchableLayer.hpp"

class BackableLayer : public SwitchableLayer {
public:
	static cocos2d::Layer * create(cocos2d::Node *);
	bool init(cocos2d::Node *);
private:
	void back(cocos2d::Ref *);
};
