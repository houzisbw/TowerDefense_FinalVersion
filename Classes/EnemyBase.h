#ifndef _ENEMYBASE_H_
#define _ENEMYBASE_H_
#include "cocos2d.h"
class GameManager;

class EnemyBase : public cocos2d::Sprite
{
public:
	EnemyBase();
	virtual ~EnemyBase();
	virtual bool init()=0;

	cocos2d::Animation* createAnimation(std::string prefixName, int framesNum, float delay,int width,int height);
	virtual void changeDirection(float dt);
	cocos2d::Node* currPoint();
	cocos2d::Node* nextPoint();
	void runFollowPoint();
	void setPointsVector(cocos2d::Vector<cocos2d::Node*> points);
	virtual void createAndSetHpBar();
	void enemyExplosionAnimation();
	//取消敌人行走动画
	virtual void unscheduleChangeDirection();
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);

	CC_SYNTHESIZE(int, isVisible, IsVisible);//敌人是否隐形
	CC_SYNTHESIZE(int, type, Type);//敌人种类
	CC_SYNTHESIZE(bool, isFrozen, IsFrozen);//敌人是否被冻住
	CC_SYNTHESIZE(int, frozeTime, FrozenTime);//敌人被冻住后的计时变量,这个值要变
	CC_SYNTHESIZE(int, totalFrozeTime, TotalFrozeTime);//敌人被冻住后的总时间,这个值不变
	CC_SYNTHESIZE(int, isStopped, IsStopped);//敌人是否被封印住，配合技能3使用
	CC_SYNTHESIZE(int, maxHp, MaxHp);
	CC_SYNTHESIZE(int, currHp, CurrHp);
	CC_SYNTHESIZE(float, hpPercentage, HpPercentage);
	CC_SYNTHESIZE_READONLY(cocos2d::ProgressTimer*, hpBar, HpBar);
	CC_SYNTHESIZE(int, money, Money);//敌人金钱数目 
	CC_SYNTHESIZE(cocos2d::Sprite*, sprite, EnemySprite);//敌人图片
	CC_SYNTHESIZE(bool, enemySuccessful, EnemySuccessful); //敌人是否突破玩家防线

	//减慢敌人速度的2个有力指针
	cocos2d::CCScheduler *sched1;
	cocos2d::CCActionManager * actionManager1;

	GameManager* instance;



private:
	cocos2d::Vector<cocos2d::Node*> pointsVector;


protected:
	int pointCounter; //用来表示当前路径点的计数
	cocos2d::Animation *animationRight;
	cocos2d::Animation *animationLeft;
	//cocos2d::Animation *animationExplode;
	CC_SYNTHESIZE(float, runSpeed, RunSpeed);
	//cocos2d::Sprite* sprite; //怪物本身的图片
	cocos2d::Sprite* hpBgSprite;//怪物血量条背景图片
	cocos2d::Sprite * iceSprite;//怪物被冰冻后的冰块图片
};
#endif

