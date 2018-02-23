#ifndef __GAME_OVER_LAYER_H__
#define __GAME_OVER_LAYER_H__

#include "cocos2d.h"

class GameOverLayer : public cocos2d::LayerColor
{
public:
	static GameOverLayer * create(int goals);
	bool init(int goals);

private:
	void replay(cocos2d::Ref *);
	void back(cocos2d::Ref *);

};

#endif //__GAME_OVER_LAYER_H__