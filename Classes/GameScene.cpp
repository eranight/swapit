#include "GameScene.hpp"
#include "SceneFactory.hpp"
#include "SpriteManager.h"
#include "GameLineSupplier.hpp"

#include <algorithm>

USING_NS_CC;

GameScene::GameScene() {

}

GameScene::~GameScene() {
	SPR_MANAGER->release();
	if (lineSupplier != nullptr) {
		delete lineSupplier;
		lineSupplier = nullptr;
	}
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	SPR_MANAGER->retain();

	auto background = LayerColor::create(Color4B::WHITE);
	this->addChild(background);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	center = (origin + visibleSize) * 0.5f;

	swapLayer = SwapLayer::create();
	swapLayer->setVelocity(visibleSize.width * 0.3f);
	this->addChild(swapLayer);

	auto levels = convert();
	lineSupplier = new GameLineSupplier(LineInfo(LineInfo::Element::red, LineInfo::Element::none, LineInfo::Element::blue), levels);
	Vec2 startPosition = Vec2(origin.x, (origin + visibleSize).y + SPR_MANAGER->getSpriteSize());
	Vec2 finishPosition = Vec2(origin.x, origin.y - SPR_MANAGER->getSpriteSize());
	linesLayer = LinesLayer::create({ startPosition, finishPosition, visibleSize.height * 0.25f }, lineSupplier);
	this->addChild(linesLayer);

	gameOverLayer = LayerColor::create(Color4B(Color3B::WHITE, 50));
	this->addChild(gameOverLayer);
	
	auto scoreCover = Sprite::create("labelForScore.png");
	scoreCover->setPosition(center);
	gameOverLayer->addChild(scoreCover);
	
	float fontSize = Configuration::getInstance()->getValue("fontSize").asFloat();
	scoreLabel = Label::create("0", Configuration::getInstance()->getValue("font").asString(), fontSize);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(Vec2(center.x, (origin.y + visibleSize.height) - fontSize * 0.5f));
	this->addChild(scoreLabel);
	//gameOverLayer->addChild(scoreLabel);
	auto backItem = MenuItemImage::create("backNormal.png", "backSelected.png", [](Ref * ref) {
		Director::getInstance()->replaceScene(SceneFactory::createMenuScene());
	});
	backItem->setPosition(Vec2(center.x - (scoreCover->getContentSize().width * 0.5f - backItem->getContentSize().width * 0.5f),
		center.y - (scoreCover->getContentSize().height * 0.5f + backItem->getContentSize().height * 0.5f)));

	auto replayItem = MenuItemImage::create("replayNormal.png", "replaySelected.png", [](Ref * ref) {
		Director::getInstance()->replaceScene(SceneFactory::createGameScene());
	});
	replayItem->setPosition(Vec2(center.x + (scoreCover->getContentSize().width * 0.5f - replayItem->getContentSize().width * 0.5f),
		center.y - (scoreCover->getContentSize().height * 0.5f + replayItem->getContentSize().height * 0.5f)));

	menu = Menu::create(backItem, replayItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	gameOverLayer->addChild(menu, 1);

	detector = CollisionDetector::create(swapLayer, linesLayer, CC_CALLBACK_2(GameScene::collide, this));
	this->addChild(detector);

	score = 0;

	return true;
}

void GameScene::onEnter() {
	Scene::onEnter();
	menu->setEnabled(false);
	gameOverLayer->setVisible(false);
	auto children = gameOverLayer->getChildren();
	std::for_each(children.begin(), children.end(), [](Node * child) { child->setOpacity(0); });
	run();
}

void GameScene::run() {
	getEventDispatcher()->dispatchCustomEvent(LinesLayer::GENERATE_NEW_LINE_EVENT);
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create(CC_CALLBACK_0(GameScene::run, this))));
}

void GameScene::gameOver() {
	swapLayer->block();
	linesLayer->block();
	detector->unscheduleUpdate();
	gameOverLayer->setVisible(true);
	this->stopAllActions();
	scoreLabel->setOpacity(0);
	scoreLabel->setPosition(center);
	scoreLabel->setParent(nullptr);
	gameOverLayer->addChild(scoreLabel);
	int bestScore = UserDefault::getInstance()->getIntegerForKey("score", 0);
	if (bestScore < score) {
		UserDefault::getInstance()->setIntegerForKey("score", score);
		Sprite * cups = Sprite::create("cups.png");
		cups->setPosition(center);
		cups->setOpacity(0);
		gameOverLayer->addChild(cups);
	}
	auto fadeInAction = FadeIn::create(Configuration::getInstance()->getValue("fadeInTime").asFloat());
	auto children = gameOverLayer->getChildren();
	std::for_each(children.begin(), children.end(), [fadeInAction](Node * child) { child->runAction(fadeInAction->clone()); });
	this->runAction(Sequence::createWithTwoActions(fadeInAction, CallFunc::create([this]() { menu->setEnabled(true); })));
}

std::vector<LevelProbabilities> GameScene::convert() {
	std::vector<LevelProbabilities> levelVector;
	auto levels = Configuration::getInstance()->getValue("levels").asValueVector();
	for (auto & iter : levels) {
		auto map = iter.asValueMap();

		std::vector<const int> action;
		auto vec = map["action"].asValueVector();
		std::for_each(vec.begin(), vec.end(), [&action](Value & val) { action.push_back(val.asInt()); });

		std::vector<const int> amount;
		vec = map["amount"].asValueVector();
		std::for_each(vec.begin(), vec.end(), [&amount](Value & val) { amount.push_back(val.asInt()); });

		levelVector.push_back(LevelProbabilities({ action, amount, map["wall"].asInt(), map["forceSwap"].asBool(), map["forceMiddle"].asBool(), map["forceDouble"].asBool(),
			map["forceWall"].asBool() }));
	}
	return levelVector;
}

bool GameScene::collide(const LineInfo::Element & first, const LineInfo::Element & second) {
	if (first == second) {
		++score;
		scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
		return true;
	}
	else if (first != LineInfo::Element::violet && second != LineInfo::Element::violet) {
		gameOver();
	}
	return false;
}