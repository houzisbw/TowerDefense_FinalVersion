#ifndef _ENEMYINVISIBLE_H_
#define _ENEMYINVISIBLE_H_
#include "EnemyBase.h"
#include "cocos2d.h"
#include "GameManager.h"
class EnemyInvisible :
	public EnemyBase
{
public:
	EnemyInvisible();
	~EnemyInvisible();
	virtual bool init() /*override*/;
	virtual void changeDirection(float dt);
	static EnemyInvisible* createEnemyInvisible(cocos2d::Vector<cocos2d::Node*> points, int hp);
	GameManager * instance;

	virtual void unscheduleChangeDirection();
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);
};
#endif

