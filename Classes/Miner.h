#ifndef _MINER_H_
#define _MINER_H_
#include "cocos2d.h"
#include "AStarPath.h"
#include "GameManager.h"
class CampTower;
enum minerState //采矿者状态
{
	stateIdle,
	stateWalkToMine,
	stateWalkToCamp,
	stateMining
};
class Miner : public cocos2d::Sprite  //采矿者
{
public:
	Miner();
	~Miner();
	virtual bool init();
	void setAnimation(int dir);//设置英雄的行走动画,表示英雄行走方向的int变量dir:0表示左上角，1表示正上方..........7表示正左方
	void stopMinerAction(int dir); //英雄停止时正确显示其站位
	void runFollowShortestRoad();//沿着最短路行走
	void pathFinding(int srcX, int srcY, int destX, int destY);
	int getMinerDirection(cocos2d::Point now, cocos2d::Point next); //获取英雄行走方向的函数
	cocos2d::Vec2 convertToTileCoord(cocos2d::Vec2 position); //将地图坐标转化为tile 坐标 
	void openListClear();//清空openlist列表
	CC_SYNTHESIZE(int, time, Time);//生存时间
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, timeBar, TimeBar);
	void decreaseSurviveTime(float dt);//生存时间减少函数
	void setTimeBarVisibleOrNot(bool is);//设置其属性条可见与否
	CREATE_FUNC(Miner);

	//2个变量记录上一次方向和这一次方向
	int prevDir;
	int nowDir;
	std::vector<cocos2d::Vec2> roadPoint;//存放最短路的vector
	AStarPath * aStar; //Astar寻路对象指针

	cocos2d::Sprite * minerSprite;
	cocos2d::Sprite * minerDestPointer;//士兵目的地指示器
	CC_SYNTHESIZE(cocos2d::Sprite*, stateSprite, StateSprite);//矿工状态标识
	CC_SYNTHESIZE(bool, isReachable, IsReachable);//目的地是否可达
	CC_SYNTHESIZE(bool, isReached, IsReached);//是否到达目的地
	CC_SYNTHESIZE(bool, isMiningFinished, IsMiningFinished);//采矿作业是否完成
	CC_SYNTHESIZE(int, miningTimer, MiningTimer);//采矿作业计时器
	void miningTimerIncrease(float dt);//采矿计时器函数
	//添加触摸函数;
	virtual void addTouch();
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);


	
	CC_SYNTHESIZE(cocos2d::Point, miningPoint, MiningPoint);//采矿点
	//有限状态机相关函数
	void update(float dt);
	//设置采矿者状态
	void setState(minerState nextState);
	//搜寻最近的大本营
	void searchNearestCamp();
	//找到离自己最近的空闲卸货位置
	void searchNearestUnloadPos(CampTower* campTower);


private:

	minerState curState;//当前状态
	CampTower * nearestCamp;//最近的大本营
	cocos2d::Point unloadPoint;//卸货位置

	
	

	cocos2d::Sprite * ring;//选中miner时产生的光圈
	cocos2d::Sprite * timeBarBg;//生存时间条的背景
	cocos2d::Label * timeLabel;//miner生存时间文本

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

