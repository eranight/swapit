#include "TutorialScene.hpp"

USING_NS_CC;

TutorialScene::~TutorialScene() {

}

bool TutorialScene::init() {
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	return true;
}

void TutorialScene::update(float deltaTime) {

}

void TutorialScene::pause() {

}

void TutorialScene::resume() {

}