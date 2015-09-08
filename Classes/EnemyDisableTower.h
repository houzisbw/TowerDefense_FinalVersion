#ifndef _ENEMYDISABLETOWER_H_
#define _ENEMYDISABLETOWER_H_
#include "EnemyBase.h"
#include "cocos2d.h"
#include "GameManager.h"
class EnemyDisableTower :
	public EnemyBase
{
public:
	EnemyDisableTower();
	~EnemyDisableTower();
	virtual bool init() /*override*/;
	virtual void changeDirection(float dt);
	static EnemyDisableTower* createEnemyDisableTower(cocos2d::Vector<cocos2d::Node*> points, int hp);
	GameManager * instance;

	virtual void unscheduleChangeDirection();
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);
	//沉默附近炮塔
	void disableNearbyTower(float dt);
private:
	cocos2d::Sprite * disableRing;//防御光环:可以让该敌人附近的炮塔停止攻击
};
#endif

