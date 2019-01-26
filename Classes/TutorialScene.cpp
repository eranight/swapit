#include "TutorialScene.hpp"
#include "SceneFactory.hpp"

USING_NS_CC;

TutorialScene::TutorialScene() : prompts({ "tap to swap", "allow the square", "tap and hold" }) {

}

TutorialScene::~TutorialScene() {

}

bool TutorialScene::init() {
	if (!LayerColor::initWithColor(Color4B::WHITE)) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	swapLayer = SwapLayer::create();
	this->addChild(swapLayer);

	generateLayer = GenerateLayer::create();
	this->addChild(generateLayer);

	auto skipLabel = Label::create("skip tutorial", "fonts/Marker Felt.ttf", 25);
	auto skipItem = MenuItemLabel::create(skipLabel, CC_CALLBACK_1(TutorialScene::menuSkipCallback, this));
	skipItem->setColor(Color3B::BLACK);
	skipItem->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + skipItem->getContentSize().height * 0.5f));
	menu = Menu::createWithItem(skipItem);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void TutorialScene::update(float deltaTime) {

}

void TutorialScene::menuSkipCallback(Ref * sender) {
	Director::getInstance()->replaceScene(SceneFactory::createGameScene());
}