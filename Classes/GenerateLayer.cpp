#include "GenerateLayer.h"
#include "SimpleAudioEngine.h"
#include "LineSprites.h"

USING_NS_CC;

GenerateLayer::~GenerateLayer() {}

GenerateLayer * GenerateLayer::create(float leftColumn, float centerColumn, float rightColumn)
{
	GenerateLayer * pRet = new (std::nothrow) GenerateLayer();
	if (pRet && pRet->init(leftColumn, centerColumn, rightColumn))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool GenerateLayer::init(float leftColumn, float centerColumn, float rightColumn)
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Director::getInstance()->getTextureCache()->addImage("ball.png");
	Director::getInstance()->getTextureCache()->addImage("square.png");

	std::vector<LineSprites::LineElement> vect = { LineSprites::LineElement::blue, LineSprites::LineElement::green, LineSprites::LineElement::red };
	LineSprites * line = LineSprites::create(vect);
	line->setPosition((origin + visibleSize) - Vec2((origin.x + visibleSize.width) * 0.5f, 0.0f));
	this->addChild(line);
	line->runAction(MoveBy::create(5.0f, Vec2(0.0f, visibleSize.height)));

	return true;
}