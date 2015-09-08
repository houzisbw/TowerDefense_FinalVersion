#ifndef _MULTITOWER_H_
#define _MULTITOWER_H_
#include "BaseTower.h"
#include "cocos2d.h"
#include "GameManager.h"
class MultiTower :
	public BaseTower
{
public:
	MultiTower();
	~MultiTower();
	virtual bool init();
	CREATE_FUNC(MultiTower);
	void shoot(float dt); //真正发射子弹
private:
	GameManager * instance;
};
#endif

