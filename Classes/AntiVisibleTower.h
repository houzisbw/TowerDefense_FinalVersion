#ifndef _ANTIVISIBLETOWER_H_
#define _ANTIVISIBLETOWER_H_
#include "cocos2d.h"
#include "BaseTower.h"
#include "EnemyBase.h"

class GameManager;

class AntiVisibleTower : public BaseTower
{
public:
	AntiVisibleTower();
	~AntiVisibleTower();
	virtual bool init();
	virtual void shoot(float dt);
	bool isEnemyInAnyAntiTowerRange(EnemyBase* enemy);//敌人是否处于任何一个反隐塔的范围内
	CREATE_FUNC(AntiVisibleTower);
private:
	GameManager * instance;
};
#endif
