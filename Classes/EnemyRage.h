#ifndef _ENEMYRAGE_H_
#define _ENEMYRAGE_H_
#include "cocos2d.h"
#include "EnemyBase.h"
#include "GameManager.h"
class EnemyRage :
	public EnemyBase
{
public:
	EnemyRage();
	~EnemyRage();
	virtual bool init() /*override*/;
	virtual void changeDirection(float dt);
	static EnemyRage* createEnemyRage(cocos2d::Vector<cocos2d::Node*> points, int hp);
	GameManager * instance;

	virtual void unscheduleChangeDirection();
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);
	//rage狂怒数值
	CC_SYNTHESIZE(int, rage, Rage);
	//狂怒值进度条
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, rageBar, RageBar);
	//狂怒状态图标
	CC_SYNTHESIZE(cocos2d::Sprite*, rageIcon, RageIcon);
	//创建狂怒条
	void createRageBar();
	//该敌人加速前进一段时间
	void runFastInPeroid();
	//敌人怒气值衰减函数
	void rageDecrease(float dt);

private:
	cocos2d::Sprite* rageBgSprite;//怪物狂怒条背景图片
};
#endif

