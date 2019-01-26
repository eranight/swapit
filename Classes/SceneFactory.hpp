#pragma once

#include "cocos2d.h"

class SceneFactory {
public:
	static cocos2d::Scene * createMenuScene();
	static cocos2d::Scene * createTutorialScene();
	static cocos2d::Scene * createGameScene();
};