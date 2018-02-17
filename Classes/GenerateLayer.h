#ifndef __GENERATE_LAYER_H__
#define __GENERATE_LAYER_H__

#include "cocos2d.h"
#include "LineSprites.h"
#include "LineBuilder.h"

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
	cocos2d::Vec2 nextGenerationPosition;
	cocos2d::Vector<LineSprites *> lines;

	float velocity;
	float timeToNextGeneration;

	LineBuilder lineBuilder;
	void generateNewLine();
	void eraseLine(LineSprites * line);
};

#endif // __PLAYING_LAYER_H__
