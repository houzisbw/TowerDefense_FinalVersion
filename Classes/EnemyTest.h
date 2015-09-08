#ifndef _ENEMYTEST_H_
#define _ENEMYTEST_H_
#include "cocos2d.h"
#include "EnemyBase.h"
#include "GameManager.h"
class EnemyTest : public EnemyBase
{
public:
	EnemyTest();
	~EnemyTest();
	virtual bool init() /*override*/;
	void decreaseHp(float dt);//测试自残函数
	virtual void changeDirection(float dt);
	static EnemyTest* createEnemyTest(cocos2d::Vector<cocos2d::Node*> points, int hp);
	GameManager * instance;

	virtual void unscheduleChangeDirection();
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);

	
};

#endif

