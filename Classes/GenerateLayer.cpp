#include "GenerateLayer.h"
#include "SimpleAudioEngine.h"

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



	return true;
}