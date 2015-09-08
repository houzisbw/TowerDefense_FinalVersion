#ifndef _POWERPLANTTOWER_H_
#define _POWERPLANTTOWER_H_
#include "cocos2d.h"
#include "BaseTower.h"
class PowerPlantTower : public BaseTower
{
public:
	PowerPlantTower();
	~PowerPlantTower();
	virtual bool init();
	void shoot(float dt);//其实这个函数不起作用
	CREATE_FUNC(PowerPlantTower);
private:
	

};
#endif

