
#ifndef _SOLDIER_H_
#define _SOLDIER_H_
#include "cocos2d.h"
#include "AStarPath.h"
#include "GameManager.h"
#include "EnemyBase.h"


class Soldier : public cocos2d::Sprite
{
public:
	
	Soldier();
	~Soldier();
	virtual bool init();
	void setAnimation(int dir);//设置英雄的行走动画,表示英雄行走方向的int变量dir:0表示左上角，1表示正上方..........7表示正左方
	void stopSoldierAction(int dir); //英雄停止时正确显示其站位
	void runFollowShortestRoad();//沿着最短路行走
	void pathFinding(int srcX,int srcY,int destX,int destY);
	int getHeroDirection(cocos2d::Point now, cocos2d::Point next); //获取英雄行走方向的函数
	void openListClear();//清空openlist列表
	CC_SYNTHESIZE(int, time, Time);//生存时间
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, timeBar, TimeBar);
	void decreaseSurviveTime(float dt);//生存时间减少函数
	void setTimeBarVisibleOrNot(bool is);//设置其属性条可见与否
	CREATE_FUNC(Soldier);

	//2个变量记录上一次方向和这一次方向
	int prevDir;
	int nowDir;
	std::vector<cocos2d::Vec2> roadPoint;//存放最短路的vector
	AStarPath * aStar; //Astar寻路对象指针

	cocos2d::Sprite * soldierSprite;
	cocos2d::Sprite * soldierDestPointer;//士兵目的地指示器
	CC_SYNTHESIZE(Sprite*, deployedSprite, DeployedSprite);//部署时产生的毒气图片
	CC_SYNTHESIZE(float, pollutionRange, PollutionRange);//毒气作用范围
	CC_SYNTHESIZE(bool, isReachable, IsReachable);//目的地是否可达

	CC_SYNTHESIZE(EnemyBase*, nearestEnemy, NearestEnemy);//该士兵的最近的敌人
	CC_SYNTHESIZE(int, attackRange, AttackRange);//该士兵的攻击范围
	CC_SYNTHESIZE(bool, isDeployed, IsDeployed);//该士兵是否部署了(部署状态该士兵不能攻击)
	CC_SYNTHESIZE(int, attackDamage, AttackDamage);//该士兵的攻击力
	void checkNearestEnemy(float dt);//搜寻附近的最近敌人
	void shoot(float dt);//射击函数
	void increaseRageIfHitEnemyWithRage(EnemyBase* enemy);//如果子弹击中怒气型敌人，则敌人增加怒气

	//添加触摸函数
	virtual void addTouch();
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

private:
	
	cocos2d::Sprite * ring;//选中士兵时产生的光圈
	cocos2d::Sprite * timeBarBg;//生存时间条的背景
	cocos2d::Label * timeLabel;//士兵生存时间文本

	cocos2d::Vector<cocos2d::SpriteFrame*> walkDown;//向下走的动画vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkUp;//向上走的动画vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeft;//向左走的动画vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRight;//向左走的动画vector

	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeftUp;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeftDown;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRightUp;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRightDown;

	cocos2d::Animation * animationWalkDown;
	cocos2d::Animation * animationWalkUp;
	cocos2d::Animation * animationWalkLeft;
	cocos2d::Animation * animationWalkRight;

	cocos2d::Animation * animationWalkLeftUp;
	cocos2d::Animation * animationWalkRightUp;
	cocos2d::Animation * animationWalkLeftDown;
	cocos2d::Animation * animationWalkRightDown;
	
	
	int roadLength;//最短路长度
	GameManager * instance;

	cocos2d::EventListenerTouchOneByOne *  listener;


};
#endif

