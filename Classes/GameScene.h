#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GenerateLayer;
class SwapLayer;
class LineSprites;

class GameScene : public cocos2d::LayerColor
{
public:
	~GameScene();

public:
    static cocos2d::Scene* createScene();

	CREATE_FUNC(GameScene);
    virtual bool init() override;
	void update(float) override;
    
    void menuCloseCallback(cocos2d::Ref *);
	
private:
	bool gameOver;
	int goals;
	GenerateLayer * generateLayer;
	SwapLayer * swapLayer;

	LineSprites * lineForCollisionDetect;

	void collisionUpdate();
	void checkCollision(const cocos2d::Sprite *, const cocos2d::Sprite *);
};

#endif // __GAME_SCENE_H__
