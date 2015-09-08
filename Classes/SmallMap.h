#ifndef _SMALLMAP_H_
#define _SMALLMAP_H_
#include "cocos2d.h"

class GameManager;

class SmallMap : public cocos2d::Sprite
{
public:
	SmallMap();
	~SmallMap();
	virtual bool init();
	CREATE_FUNC(SmallMap);
	cocos2d::Sprite * map;
	cocos2d::Vec2 converToSmallMap(cocos2d::Vec2 pos);
	void updateTowerPosition(float dt);
	cocos2d::Vector<cocos2d::Sprite * > greenVector; //存放绿色方块(炮塔)的vector
	cocos2d::Vector<cocos2d::Sprite * > redVector;//存放红色方块(敌人)的vector
	cocos2d::Point windowPoint;//白色window的左下角那个点
private:
	GameManager * instance;
	cocos2d::Sprite * window;//手机屏幕方框
	cocos2d::Sprite * heroIcon;//英雄的头像
	bool isHeroExisted;
};
#endif

