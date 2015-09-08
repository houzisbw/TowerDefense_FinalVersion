#ifndef _FORETELLINGTOWER_H_
#define _FORETELLINGTOWER_H_
#include "cocos2d.h"
#include "BaseTower.h"
class ForetellingTower : public BaseTower
{
public:
	ForetellingTower();
	~ForetellingTower();
	virtual bool init();
	void shoot(float dt);//其实这个函数不起作用
	CREATE_FUNC(ForetellingTower);
};
#endif

