#ifndef _PRISMTOWER_H_
#define _PRISMTOWER_H_
#include "BaseTower.h"
#include "cocos2d.h"

class PrismTower : public BaseTower
{
public:
	PrismTower();
	~PrismTower();
	virtual bool init();
	CREATE_FUNC(PrismTower);
	void shoot(float dt); //真正发射子弹
	void towerEnergyGathering(); //光棱塔聚能效果
	int getTotalDamage(int towerNum);//获取聚能塔总攻击力：即聚能后的攻击力
	void increaseRageIfHitEnemyWithRage(EnemyBase* enemy);//如果子弹击中怒气型敌人，则敌人增加怒气
	void EnemyThrowMushroomToTower(EnemyBase* enemy);//投掷型敌人投掷出蘑菇来攻击玩家防御塔
private:
	cocos2d::DrawNode * dn;
	int nearbyTower;  //附近塔的个数
};
#endif

