#include "SceneFactory.hpp"
#include "MainMenuScene.hpp"
#include "TutorialScene.hpp"

USING_NS_CC;

Scene * SceneFactory::createMenuScene() {
	auto scene = MainMenuScene::create();
	return scene;
}

Scene * SceneFactory::createTutorialScene() {
	auto scene = TutorialScene::create();
	return scene;
}

Scene * SceneFactory::createGameScene() {
	auto scene = Scene::create();
	return scene;
}