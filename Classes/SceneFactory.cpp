#include "SceneFactory.hpp"
#include "TutorialScene.hpp"

USING_NS_CC;

Scene * SceneFactory::createMenuScene() {
	auto scene = Scene::create();
	return scene;
}

Scene * SceneFactory::createTutorialScene() {
	auto scene = Scene::create();
	auto layer = TutorialScene::create();
	scene->addChild(layer);
	return scene;
}

Scene * SceneFactory::createGameScene() {
	auto scene = Scene::create();
	return scene;
}