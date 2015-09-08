#ifndef _TRAININGTOWER_H_
#define _TRAININGTOWER_H_
#include "cocos2d.h"
#include "BaseTower.h"
#include "Soldier.h"
#include "GameManager.h"
#include "AStarPath.h"
#include <queue>
#define MAX_SOLDIER_NUM 10
class TrainingTower : public BaseTower
{
public:
	TrainingTower();
	~TrainingTower();
	virtual bool init();
	CREATE_FUNC(TrainingTower);
	void shoot(float dt);//其实这个函数不起作用
	cocos2d::Sprite * getTrainingSprite();

	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position); //将map坐标转化为tile坐标
	cocos2d::Vec2 convertToScreenCoord(cocos2d::Vec2 position); //将tile坐标转化为map整数坐标
	cocos2d::Point getNextRallyPoint(cocos2d::Point rally);//获取下一个集结点

	CC_SYNTHESIZE(int, produceTime, ProduceTime);//士兵生产时间
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, timeBar, TimeBar);//士兵生产时间进度条
	cocos2d::Sprite * produceTimeBarBg;//生产时间条的背景

	void produceSoldier(float dt);//生产士兵的函数

private:
	cocos2d::Sprite * trainingSprite;//训练士兵的button
	cocos2d::EventListenerTouchOneByOne * trainingButtonListener;
	Soldier * soldier;//士兵
	GameManager * instance;

	//士兵站位队列:士兵生产出来时默认的站位
	std::queue<cocos2d::Point> posQueue;
	cocos2d::Point rallyPoint;//默认士兵集结点

	cocos2d::Vec2 src;//源点：士兵出生的点
	cocos2d::Label * soldierNum;//待生产士兵的数目label

	int soldierToProduceCount;//待生产士兵的数目

	bool isTrainingSpriteSelected;

	

};
#endif

