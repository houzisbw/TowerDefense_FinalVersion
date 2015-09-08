
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_
#include "cocos2d.h"
#include "EnemyBase.h"
#include "BaseTower.h"
#include "Hero.h"
#include "PrismTower.h"
#include "AntiVisibleTower.h"

#include "SmallMap.h"
#include "GroupEnemy.h"
#include "CrystalMine.h"


#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

//前向声明,防止头文件相互包含
class city;
class Soldier;
class Miner;
class CampTower;
//class SmallMap;

class GameManager
{
public:

	
	static GameManager* getInstance();
	cocos2d::Vector<EnemyBase*> enemyVector;
	cocos2d::Vector<BaseTower*> towerVector;
	cocos2d::Vector<cocos2d::Sprite*> bulletVector;
	cocos2d::Vector<PrismTower*> prismTowerVector;
	cocos2d::Vector<AntiVisibleTower*> antiVisibleTowerVector;
	std::vector<Soldier*> soldierVector;
	std::vector<Soldier*> soldierInRectVector;  //注意一个问题,如果报错invalid type for T 则表明前向声明的问题,还有问题
	std::vector<Miner*> minerInRectVector;//采矿者的vector
	std::vector<Miner*> minerVector;
	cocos2d::Vector<GroupEnemy*> groupVector; // 存储每一波敌人的信息
	cocos2d::Vector<CrystalMine*> crystalMineVector;//水晶矿vector
	std::vector<CampTower*> campTowerVector;//大本营vector
	cocos2d::TMXTiledMap * map;

	CC_SYNTHESIZE(int, money, Money);
	cocos2d::ui::Text* moneyLabel;//金钱label

	CC_SYNTHESIZE(int, movePoint, MovePoint);
	cocos2d::ui::Text* movePointLabel;//移动点数label

	CC_SYNTHESIZE(int, groupNum, GroupNum);
	CC_SYNTHESIZE(std::string, curMapName, CurMapName);
	CC_SYNTHESIZE(std::string, currLevelFile, CurrLevelFile);
	//CC_SYNTHESIZE(std::string, nextLevelFile, NextLevelFile);
	CC_SYNTHESIZE(bool, isFinishedAddGroup, IsFinishedAddGroup);
	//CC_SYNTHESIZE(std::string, curBgName, CurBgName);
	CC_SYNTHESIZE(int, greenPower, GreenPower);//总电力
	CC_SYNTHESIZE(int, redPower, RedPower);//消耗电力
	cocos2d::Vec2 walkingMapPoint;//选择关卡的地图上的城市坐标
	std::vector<cocos2d::Vec2> cityPoint; //存放城市坐标的vector
	cocos2d::Sprite * player;
	cocos2d::Sprite * cloud;
	bool isWalking; //player是否正在行走中
	city * destCity;//目的城市
	void clear();
	std::vector<city*> cityVector; //存放city指针的vector
	Hero * hero;
	//英雄面板
	cocos2d::ui::ImageView * heroPanel;
	//游戏行走地图的二维数组指针
	int(*walkMap)[80];
	int totalSoldierNum;//士兵总数
	CC_SYNTHESIZE(int, totalMinerNum, TotalMinerNum);//采矿者总数

	SmallMap * smallMap;

	cocos2d::Sprite* towerRangeIndicator;
	cocos2d::ui::Text* powerText;//电量文本

private:
	static GameManager * mgr;
	GameManager();
	~GameManager();
};
#endif
