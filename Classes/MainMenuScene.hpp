#pragma once

#include "cocos2d.h"
#include "LayerType.hpp"
#include "LineSupplier.hpp"

class MainMenuScene : public cocos2d::Scene {
public:
	CREATE_FUNC(MainMenuScene);
	MainMenuScene() : lineSupplier(nullptr) {}
	~MainMenuScene();
public:
	virtual bool init() override;
public:
	void switchLayer(const LayerType &);
private:
	LayerType layerType;
	cocos2d::Map<LayerType, cocos2d::Layer *> layers;
	LineSupplier * lineSupplier;
};

class RepeatedLineSupplier : public LineSupplier {
public:
	RepeatedLineSupplier(const std::initializer_list<LineInfo> & lines);
	LineInfo getNextLine() override;
private:
	std::queue<LineInfo> lineQueue;
};