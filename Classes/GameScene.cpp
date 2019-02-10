#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SpriteManager.h"
#include "GameScene.h"

USING_NS_CC;

GameScene::~GameScene() {
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	SpriteManager::getInstance()->retain(); //initialization of the SpriteManager instance

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return true;
}

void GameScene::update(float dt) {
	
}

void GameScene::pause() {
	Layer::pause();
}

void GameScene::resume() {
	Layer::resume();
}