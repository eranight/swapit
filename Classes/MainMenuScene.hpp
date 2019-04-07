#pragma once

#include "cocos2d.h"
#include "LayerType.hpp"
#include "LinesLayer.h"
#include "LineSupplier.hpp"

class MainMenuScene : public cocos2d::Scene {
public:
	CREATE_FUNC(MainMenuScene);
	MainMenuScene() : lineSupplier(nullptr) {}
	~MainMenuScene();
public:
	virtual bool init() override;
	virtual void onEnter() override;
public:
	void switchLayer(const LayerType &);
	void genetateNextLine();
private:
	LayerType layerType;
	cocos2d::Map<LayerType, cocos2d::Layer *> layers;
	LineSupplier * lineSupplier;
	cocos2d::Sequence * nextLineTimer;
};

class RepeatedLineSupplier : public LineSupplier {
public:
	RepeatedLineSupplier(const std::initializer_list<LineInfo> & lines);
	LineInfo getNextLine() override;
private:
	std::queue<LineInfo> lineQueue;
};