#ifndef __GENERATE_LAYER_H__
#define __GENERATE_LAYER_H__

#include "cocos2d.h"
#include "Blocking.h"
#include "LineSprites.h"
#include "LineBuilder.h"

class GenerateLayer : public cocos2d::Layer, public Blocking
{
public:
	~GenerateLayer();

public:
	CREATE_FUNC(GenerateLayer);
	bool init() override;
	void update(float) override;
	void pause() override;
	void resume() override;

public:
	void block() override;
	void unblock() override;

public:
	LineSprites * getFrontLine() { return lines.at(0); }
	LineSprites * getFirstLineAbove(float y);
	void start();
	void setVelocity(float velocity);
	void stop();

public:
	const cocos2d::Vec2 & getStartPosition() { return startPosition; }
	void setStartPosition(const cocos2d::Vec2 & position) { startPosition = position; recalculateTime(); }

	const cocos2d::Vec2 & getFinishPosition() { return finishPosition; }
	void setFinishPosition(const cocos2d::Vec2 & position) { finishPosition = position; recalculateTime(); }

	const cocos2d::Vec2 & getNextGenerationPosition() { return nextGenerationPosition; }
	void setNextGenerationPosition(const cocos2d::Vec2 & position) { nextGenerationPosition = position; }

	LineBuilder & getLineBuilder() { return lineBuilder; }
	void generateNewLine();

private:
	void recalculateTime();
	void removeLine(Node *);
	void recreateLineActions();

private:
	cocos2d::Vec2 startPosition;
	cocos2d::Vec2 finishPosition;
	cocos2d::Vec2 nextGenerationPosition;
	cocos2d::Vector<LineSprites *> lines;

	float velocity;
	float timer;

	LineBuilder lineBuilder;
};

#endif // __PLAYING_LAYER_H__
