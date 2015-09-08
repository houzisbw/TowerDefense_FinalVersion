#ifndef _ENEMYTHROW_SON_H_
#define _ENEMYTHROW_SON_H_
#include "EnemyBase.h"
#include "BaseTower.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "AStarPath.h"
class EnemyThrow_son :
	public EnemyBase
{
public:
	EnemyThrow_son();
	~EnemyThrow_son();
	virtual bool init();
	static EnemyThrow_son* createEnemyThrow_son(cocos2d::Vector<cocos2d::Node*> points, int hp,float time);
	virtual void changeDirection(float dt);
	//敌人冻住之后的解冻计时函数
	virtual void deFroze(float dt);

	void createAndSetHpBar();

	//敌人能量衰减函数
	void decreaseEnergy(float dt);

	//等待数秒后冲向离自己最近的炮塔
	void waitToRunToNearestTower(float dt);
	//开始冲向离自己最近的防御塔
	void runToNearestTower(int srcX,int srcY,int destX,int destY);
	//沿着最短路vector行走
	void runFollowShortestRoad();

	//坐标转换:将map坐标转化为tile坐标
	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position);

	//暂时移除炮塔占地面积:让蘑菇能够寻路，否则无法寻路
	void removeOrAddLandOccupation(int type, cocos2d::Point pos, int removeOrAdd);

	//碰撞检测过程:如果检测到蘑菇与炮塔碰撞则移除蘑菇
	void collisionDetection(float dt);
	
	//寻路A*对象
	AStarPath *aStar;

	//开始等待
	void scheduleOnceToWalk(float time);
	//开启列表清空
	void openListClear();

	//是否可达目标
	CC_SYNTHESIZE(bool, isReachable, IsReachable);
	//最短路道路坐标vector
	std::vector<cocos2d::Vec2> roadPoint;
	//道路长度
	int roadLength;

	GameManager * instance;

	//死亡动画node
	cocos2d::Node * dieNode;
	cocos2d::Node * dieNode2;
	//移除炮塔
	void removeTower();
private:
	BaseTower* nearestTower;
	int towerType;
};
#endif

