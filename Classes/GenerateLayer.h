#ifndef __GENERATE_LAYER_H__
#define __GENERATE_LAYER_H__

#include "cocos2d.h"

class GenerateLayer : public cocos2d::Layer
{
public:
	~GenerateLayer();

public:
	CREATE_FUNC(GenerateLayer);
	bool init();

private:
	cocos2d::Vec2 startPosition;
	cocos2d::Vec2 finishPosition;
};

#endif // __PLAYING_LAYER_H__
