#ifndef __GENERATE_LAYER_H__
#define __GENERATE_LAYER_H__

#include "cocos2d.h"

class GenerateLayer : public cocos2d::Layer
{
public:
	virtual ~GenerateLayer();
	bool init(float leftColumn, float centerColumn, float rightColumn);
	static GenerateLayer * create(float leftColumn, float centerColumn, float rightColumn);
};

#endif // __PLAYING_LAYER_H__
