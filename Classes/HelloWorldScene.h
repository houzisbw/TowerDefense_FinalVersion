#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BaseTower.h"
#include "SmallMap.h"
#include "EnemyBase.h"
#include "Hero.h"
#include "GroupEnemy.h"

#include <queue>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <memory.h>

#define MAP_SIZE 80
#define INF 10000

enum //技能冷却时间
{
	COOLDOWN1 = 30,
	COOLDOWN2 = 40,
	COOLDOWN3 = 60,
	COOLDOWN4 = 80

};
enum //技能耗蓝
{
	MANA1 = 40,
	MANA2 = 60,
	MANA3 = 80,
	MANA4 = 100
};

enum
{
	PRISM_TOWER = 0,
	ARTILLERY_TOWER = 1,
	ICE_TOWER = 2,
	ANTI_TOWER = 3,
	MULTI_TOWER = 4,
	POWER_PLANT = 5,
	FORETELLING_TOWER=6
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	//触摸函数
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
	
	//触摸函数
	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);

	//触摸函数
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);

	//初始化敌人路径点
	void initPointsVector();

	//添加敌人
	void addEnemy();

	//碰撞检测
	void collisionDetection();

	//炮塔list里面的每个炮塔的回调函数
	void SpriteCallBack(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type);

	//炮塔list里面信息显示函数
	void towerInfoDisplay(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type);

	virtual void update(float dt);

	//炮塔移动检测
	void towerMoveCheck(cocos2d::Vec2 position, int tag);

	//坐标转换公式:将屏幕坐标转换为tilemap坐标
	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position);
	//将tile坐标转换为screen坐标
	cocos2d::Vec2 convertToScreenCoord(cocos2d::Vec2 position);

	//检查要修建的炮塔是否与其他塔有交集，如果有就不能在那造,注意2者都是在map上，所以不需要坐标转换
	bool isBuildTowerIntersected(cocos2d::Sprite * tower);

	//如果炮塔范围内有敌人，则炮塔指示器变黄色
	void changeAttackRangeIndicator();

	//将屏幕左下角的点(世界坐标原点)转化为map上的点
	void converLeftDownToMap(cocos2d::Vec2 pos);

	//A* 最短路算法
	void AStar(int srcX,int srcY,int destX,int destY);

	cocos2d::ui::Widget * towerInfoSelected;
	CC_SYNTHESIZE_READONLY(cocos2d::ProgressTimer*, powerGreen, PowerGreen); //显示电量的进度条,绿色代表总共提供的电力
	CC_SYNTHESIZE_READONLY(cocos2d::ProgressTimer*, powerRed, PowerRed); //显示电量的进度条,红色代表消耗电力
	//A*算法函数
	inline bool isInMapAndValid(int x, int y);
	
	inline int Heuristic(int thisX, int thisY, int destX, int destY);  //估价函数:当前节点到目的节点的曼哈顿距离

	void runFollowShortestRoad();

	int getHeroDirection(cocos2d::Point now, cocos2d::Point next); //获取英雄行走方向的函数

	void buildTowerAccordingToTag(int tag,cocos2d::Point pos);//建造炮塔

	void yeqiAnimation();//楪祈动画效果

	void abilityEffect1();//技能1特效
	void ability1Damage(float dt);//技能1持续减血效果

	void abilityEffect2();//技能2特效
	void ability2Damage(float dt);//技能2效果

	void abilityEffect3();//技能3特效

	void abilityEffect4();//技能4特效
	void findHighestHpEnemy(float dt);
	void heroAndEnemyDieEffect();//英雄死亡特效


	void updateCoolDown(float dt);//更新4个技能冷却时间

	void removeOrAddLandOccupation(int type,cocos2d::Point pos,int removeOrAdd);//去除炮塔建筑占地面积,1为增加占地,0为去除占地

	void smallMapComeInAnimation(bool inOrOut);//小地图进入退出动画

	void calculateDestPositionSoldierDistribution(cocos2d::Point dest,int soldierNum);//计算目的地士兵分布阵型

	void calculateDestPositionMinerDistribution(cocos2d::Point dest, int minerNum);//计算目的地采矿者分布阵型

	void increaseRageIfHitEnemyWithRage(EnemyBase* enemy);//如果子弹击中怒气型敌人，则敌人增加怒气

	void EnemyThrowMushroomToTower(EnemyBase* enemy);//投掷型敌人投掷出蘑菇来攻击玩家防御塔

	CC_SYNTHESIZE(int, enemyCount, EnemyCount);//敌人总数
	CC_SYNTHESIZE(int, towerCount, TowerCount);//炮塔总数

	GroupEnemy* currentGroup();//获取当前波敌人
	GroupEnemy* nextGroup();// 获取下一波敌人
	void addEnemyLogic(float dt);//当前波敌人添加完毕后，添加下一波敌人
	void enemyWaveEffect(int enemyType,int enemyWave);//每一波敌人的特效：包含敌人介绍 和 波数信息
	void victoryEffect();//本关过关提示
	void defeatEffect();//本关失败提示

	void enemyReachedDestination();//敌人到达目的地（突破玩家防线）

	void updatePlayerHeadAndLifeBar();//让玩家头像和血条保持正常状态

	void countDown(float dt);//倒计时函数

	void waitToAddEnemy(float dt);//等待倒计时完成后添加敌人

	void initCrystalMine();// 初始化矿物资源

	void SoldierPollutionGasDamage(float dt);//士兵部署时产生的毒气攻击效果





	

private:

	cocos2d::Point towerListViewOriginPos;
	bool isLevelFail;//本关是否失败

	cocos2d::Node * rootNode;
	cocos2d::TMXTiledMap * map;
	cocos2d::TMXObjectGroup * object; //对象数组
	cocos2d::Vector<cocos2d::Node*> pointsVector;//存放敌人路径点
	cocos2d::ui::Button * build_btn; //建造按钮
	cocos2d::ui::ListView * towerListView; //炮塔的listView
	cocos2d::ui::ImageView * upDecoration;
	cocos2d::ui::ImageView * downDecoration;
	cocos2d::ui::ImageView * towerInfoBg;
	cocos2d::ui::Button * towerInfoBgClose;
	cocos2d::ui::ImageView * buildingInfoBg;
	cocos2d::ui::Button * buildingInfoBgClose;
	cocos2d::ui::ImageView * towerPanel;
	cocos2d::ui::ImageView * towerPanel_sell;   //按钮
	cocos2d::ui::ImageView * towerPanel_isSell; //是否出售炮塔的panel
	cocos2d::ui::ImageView * towerPanel_isSell_yes;
	cocos2d::ui::ImageView * towerPanel_isSell_no;
	cocos2d::ui::Text * towerName;//炮塔名字text
	cocos2d::ui::Button * soldierButton;//框选士兵的button
	cocos2d::ui::Button * minerButton;//采矿者button
	cocos2d::ui::Button * optionButton;//游戏选项button
	cocos2d::ui::Button * soldierPollutionButton;//士兵部署时的按钮（产生污染气体）

	bool isSoldierButtonSelected;
	bool isMinerButtonSelected;
	bool isSoldierSelected;//士兵是否被选中了
	bool isPollutionButtonSelected;//部署按钮是否被选中

	cocos2d::ui::ImageView * towerPanel_move;//移动按钮

	cocos2d::ui::ImageView * towerPanel_upgrade;//升级按钮
	cocos2d::ui::ImageView * towerPanel_isUpgrade;//是否升级炮塔的panel
	cocos2d::ui::ImageView * towerPanel_isUpgrade_yes;//
	cocos2d::ui::ImageView * towerPanel_isUpgrade_no;//
	cocos2d::ui::Text * text_cost; //升级花费文本
	cocos2d::ui::Text * text_preAttack; //升级前攻击力
	cocos2d::ui::Text * text_afterAttack; //升级后攻击力
	cocos2d::ui::Text * text_preRate; //升级前射击速率
	cocos2d::ui::Text * text_afterRate; //升级后设计速率

	cocos2d::ui::ImageView * powerBg;//电量背景
	cocos2d::ui::ImageView * powerIcon;//电力标志
	cocos2d::ui::ImageView * powerInfo;//电力说明面板
	cocos2d::ui::Button * powerInfoClose;//关闭面板按钮
	cocos2d::ui::ImageView * powerRedGreen;//电量进度条
	cocos2d::ui::Text * powerText;

	cocos2d::ui::ImageView * blueBottleIcon;//蓝色药水icon
	cocos2d::ui::ImageView * blueBottleSubIcon;//蓝色药水子icon，点击弹出说明面板
	cocos2d::ui::ImageView * blueBottlePanel;//蓝色药水说明面板
	cocos2d::ui::Button * blueBottlePanelClose;//蓝色药水说明面板关闭按钮
	cocos2d::ui::Button * addManaButton;//英雄回蓝按钮

	cocos2d::ui::ImageView* moneyPanel; //游戏金钱label
	cocos2d::ui::Text* moneyLabel;

	cocos2d::ui::ImageView* movePointPanel;//炮塔移动点数label
	cocos2d::ui::Text* movePointLabel;

	cocos2d::ui::ImageView* victoryButton;//游戏胜利继续按钮
	cocos2d::ui::ImageView* retryButton;//游戏失败重来按钮

	cocos2d::ui::ImageView* imageEnemyWave;//敌人波数图片
	cocos2d::ui::Text* textEnemyWave;//敌人波数文本

	//侦听器
	EventListenerTouchAllAtOnce* mapListener;

	//玩家生命值进度条
	CC_SYNTHESIZE(int, curLife, CurLife);//玩家当前生命值
	CC_SYNTHESIZE(int, maxLife, MaxLife);//玩家最大生命值
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, lifeBar, LifeBar);
	//玩家头像
	cocos2d::Sprite * playerHead;

	//敌人波数相关
	int groupCounter; //表示敌人波数的计数器
	bool firstWave;
	bool isSuccessful;

	//玩家受伤提示：敌人突破玩家防线
	cocos2d::Sprite* playerHurt;
	
	
	float redPercentage;
	float greenPercentage;

	bool isTowerListViewOut;
	bool canBuild;
	cocos2d::Sprite * buyTarget;
	cocos2d::Sprite * buildIcon;  // 是否可以建造的勾勾叉叉
	cocos2d::Sprite * attackRange;
	cocos2d::Sprite * finger;
	
	bool isFingerAdded;
	bool isTowerInfoButtonClicked;

	BaseTower * indicatorTower;
	bool  isTowerPanelOut;

	cocos2d::Sprite * towerRangeIndicator;
	cocos2d::Size visibleSize;

	SmallMap * smallMap;
	cocos2d::ui::ImageView * smallMapBg;

	Hero * hero;//英雄
	cocos2d::ui::Button * heroButton;
	bool isHeroSelected;
	cocos2d::Sprite * heroDestPointer;//英雄目标地点指示器

	//行走地图 80*80大小
	int walkMap[MAP_SIZE][MAP_SIZE];
	//A*算法点结构
	struct ANode
	{
		int x;
		int y;
		ANode * parent;
		int f, g, h;   //F = G + H,G为从起点开始移动到指定方格的耗费,h为预估耗费 整数比浮点数更高效
	};

	ANode * nodeMap[MAP_SIZE][MAP_SIZE]; //存放节点指针的数组

	//优先队列比较结构
	struct cmp{
		bool operator ()(ANode * a, ANode *b){
			return a->f > b->f;//最小值优先
		}
	};

	std::priority_queue<ANode*, std::vector<ANode*>, cmp> open; //开启列表

	bool visited[MAP_SIZE][MAP_SIZE];
	bool openList[MAP_SIZE][MAP_SIZE];
	float g[MAP_SIZE][MAP_SIZE];
	//int dire[8][2]; //放到cpp里去了
	std::vector<cocos2d::Vec2> roadPoint;//存放最短路的vector
	int roadLength;//路径的方格数目
	bool isReachable;//目的地是否可达
	
	//A*算法结束

	//2个变量记录上一次方向和这一次方向
	int prevDir;
	int nowDir;

	std::vector<cocos2d::Vec2> towerMove;//炮塔建造移动时的误差修正,使得炮塔摆放位置更精确

	//英雄的四个技能
	cocos2d::ui::Button * ability1; 
	cocos2d::ui::Button * ability2;
	cocos2d::ui::Button * ability3;
	cocos2d::ui::Button * ability4;
	//第二个技能是否过期
	bool isAbility2Expire;        
	//英雄技能说明面板
	cocos2d::ui::ImageView * heroAbilityPanel;
	cocos2d::ui::Text * heroAbilityDes;
	cocos2d::ui::ImageView * useAbility;
	cocos2d::ui::ImageView * abortAbility;
	cocos2d::ui::ImageView * abilityImage;
	cocos2d::ui::Text * heroAbilityName;
	//释放技能时的楪祈动画效果
	cocos2d::Sprite * yeqi;
	cocos2d::Sprite * yeqiBg;
	cocos2d::Label * yeqiWords;
	//每一波敌人开始时的提示效果
	cocos2d::Sprite * enemyWaveBg;
	cocos2d::Label * enemyWaveWords;
	cocos2d::Sprite * enemyWaveSprite;//敌人图片
	cocos2d::Label * enemyWaveCountLabel;//敌人波数数字
	//罗马时钟：配合技能3使用
	cocos2d::Sprite * romeClock;

	//技能特效node
	cocos2d::Node * effectNode1;
	cocos2d::Node * effectNode2;
	cocos2d::Node * effectNode4;
	//技能种类
	int abilityType;
	//第四个技能 : 血量最高的敌人
	EnemyBase * highestHpEnemy;
	//第四个技能瞄准器
	cocos2d::Sprite * gunSight;
	bool isHeroExisted;
	cocos2d::Node * heroDieNode;


	//技能冷却时间半透明遮罩
	cocos2d::ProgressTimer * coolDown1;
	cocos2d::ProgressTimer * coolDown2;
	cocos2d::ProgressTimer * coolDown3;
	cocos2d::ProgressTimer * coolDown4;
	bool isAbility1Ready;
	bool isAbility2Ready;
	bool isAbility3Ready;
	bool isAbility4Ready;

	//预言圣坛的建造个数
	int foreTellingTowerCount;
	bool isForeTellingTowerComeIn;

	//框选操作的左上角和右下角坐标
	cocos2d::Point leftUp;
	cocos2d::Point rightDown;

	cocos2d::DrawNode * dn;

	bool isSoldierInRect;
	bool isMinerInRect;

	//临时存放士兵目的地位置的vector:框选的士兵们行走所用
	std::vector<cocos2d::Point> soldierDestVector;
	//临时队列供bfs查询士兵位置用
	std::queue<cocos2d::Point> soldierDestQueue;

	//临时存放采矿者目的地位置的vector:框选的miner们行走所用
	std::vector<cocos2d::Point> minerDestVector;
	//临时队列供bfs查询采矿者位置用
	std::queue<cocos2d::Point> minerDestQueue;

	//游戏开始倒计时
	int startCount;//30秒默认
	cocos2d::ui::ImageView* imageCountDown;
	cocos2d::ui::Text* textCountDown;
	//敌人波次倒计时技术器
	int enemyWaveCounter;

	GameManager * instance1;
};

#endif // __HELLOWORLD_SCENE_H__
