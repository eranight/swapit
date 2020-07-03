#ifndef __LINES_LAYER_H__
#define __LINES_LAYER_H__

#include "cocos2d.h"
#include "Blockable.h"
#include "LineSupplier.hpp"
#include "LineSprites.h"

class LinesLayerConfiguration {
public:
	LinesLayerConfiguration(const cocos2d::Vec2 & startPosition, const cocos2d::Vec2 & finishPosition, float velocity) : startPosition(startPosition), finishPosition(finishPosition), velocity(velocity) {}
public:
	cocos2d::Vec2 getStartPosition() const { return startPosition; }
	cocos2d::Vec2 getFinishPosition() const { return finishPosition; }
	float getVelocity() const { return velocity; }
private:
	const cocos2d::Vec2 startPosition;
	const cocos2d::Vec2 finishPosition;
	const float velocity;
};

class LinesLayer : public cocos2d::Layer, public Blockable
{
public:
	static const std::string GENERATE_NEW_LINE_EVENT;
	static const std::string SET_VELOCITY_EVENT;
public:
	static LinesLayer * create(const LinesLayerConfiguration & configuration, LineSupplier * lineSupplier);
	~LinesLayer();

public:
	bool init(const LinesLayerConfiguration & configuration, LineSupplier * lineSupplier);
	void onEnter() override;
	void onExit() override;
	void update(float) override;
	void pause() override;
	void resume() override;
public:
	void block() override;
	void unblock() override;

public:
	LineSprites * getFrontLine() { return lines.at(0); }
	LineSprites * getFirstLineAbove(float y);

public:
	const cocos2d::Vec2 & getStartPosition() { return startPosition; }
	const cocos2d::Vec2 & getFinishPosition() { return finishPosition; }

private: //events
	void generateNewLine();
	void setVelocity(cocos2d::EventCustom *);
private:
	void recalculateTime();
	void removeLine(Node *);
	void recreateLineActions();

private:
	cocos2d::Vec2 startPosition;
	cocos2d::Vec2 finishPosition;
	cocos2d::Vector<LineSprites *> lines;

	float velocity;
	float timer;

	LineSupplier * lineSupplier;

	bool isBlocked;
};

#endif // __LINES_LAYER_H__
