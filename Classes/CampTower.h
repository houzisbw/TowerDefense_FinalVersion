#ifndef _CAMPTOWER_H_
#define _CAMPTOWER_H_
#include "BaseTower.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "AStarPath.h"
#include "Miner.h"
#include <queue>
#define MAX_MINER_NUM 20
class CampTower :
	public BaseTower
{
public:
	CampTower();
	~CampTower();
	virtual bool init();
	CREATE_FUNC(CampTower);
	void shoot(float dt);//其实这个函数不起作用
	cocos2d::Sprite * getTrainingSprite();

	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position); //将map坐标转化为tile坐标
	cocos2d::Vec2 convertToScreenCoord(cocos2d::Vec2 position); //将tile坐标转化为map整数坐标
	cocos2d::Point getNextRallyPoint(cocos2d::Point rally);//获取下一个集结点

	CC_SYNTHESIZE(int, produceTime, ProduceTime);//采矿者生产时间
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, timeBar, TimeBar);//采矿者生产时间进度条
	CC_SYNTHESIZE(cocos2d::Sprite*, produceTimeBarBg, ProduceTimeBarBg);//生产时间条的背景

	void produceMiner(float dt);//生产采矿者的函数
	CC_SYNTHESIZE(std::vector<cocos2d::Point>, vacantUnloadPos, VacantUnloadPos); //空闲卸货位置vector
	void updateVacantUnloadPos(cocos2d::Point Pos);//找到最近的卸货位置供采矿者卸货

private:
	cocos2d::Sprite * trainingSprite;//生产采矿者的button
	cocos2d::EventListenerTouchOneByOne * trainingButtonListener;
	GameManager * instance;
	Miner * miner;

	//采矿者站位队列:生产出来时默认的站位
	std::queue<cocos2d::Point> posQueue;
	cocos2d::Point rallyPoint;//默认士兵集结点

	cocos2d::Vec2 src;//源点：士兵出生的点
	cocos2d::Label * minerNum;//待生产士兵的数目label

	int minerToProduceCount;//待生产士兵的数目

	bool isTrainingSpriteSelected;
};
#endif

