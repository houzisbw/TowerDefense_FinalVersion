#ifndef _ENEMYTHROW_H_
#define _ENEMYTHROW_H_
#include "EnemyBase.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "EnemyThrow_son.h"
class EnemyThrow :
	public EnemyBase
{
public:
	EnemyThrow();
	~EnemyThrow();
	virtual bool init() /*override*/;
	virtual void changeDirection(float dt);
	static EnemyThrow* createEnemyThrow(cocos2d::Vector<cocos2d::Node*> points, int hp);
	GameManager * instance;

	virtual void unscheduleChangeDirection();
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);
};
#endif
