//已知bug 1: 击杀敌人时，小地图上的绿色方块会减少,而且敌人消失时有bug

//    bug 3: 蘑菇爆炸时会崩溃有时候


#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
//敌人种类
#include "EnemyTest.h"
#include "EnemyInvisible.h"
#include "EnemyDisableTower.h"
#include "EnemyRage.h"
#include "EnemyThrow.h"
#include "EnemyThrow_son.h"
//单例
#include "GameManager.h"
//水晶矿
#include "CrystalMine.h"
//炮塔种类
#include "ArtilleryTower.h"
#include "PrismTower.h"
#include "PowerPlantTower.h"
#include "ForetellingTower.h"
#include "TrainingTower.h"
#include "IceTower.h"
#include "AntiVisibleTower.h"
#include "MultiTower.h"
#include "CampTower.h"
#include "Miner.h"

#include "SmallMap.h"
#include "LevelSelectScene.h"
#include "LoadLevelInfo.h"
#include "OptionLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

using namespace cocostudio::timeline;

//表示方向的数组
int dir[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 } };

// 炮塔图样:被英雄技能2笼罩的炮塔
const char* tower_enhanced[10] =
{
	"towerEnhanced/Item1.png", "towerEnhanced/Item2.png", "towerEnhanced/Item3.png", "towerEnhanced/Item4.png", "towerEnhanced/Item5.png", "towerEnhanced/Item6.png", "towerEnhanced/Item7.png", "towerEnhanced/Item8.png", "towerEnhanced/Item1.png", "towerEnhanced/Item2.png"
};

// 炮塔图样:显示在炮塔建造列表中的
const char* tower_textures[10] =
{
	"towerItem/Item1.png", "towerItem/Item2.png", "towerItem/Item3.png", "towerItem/Item99.png", "towerItem/Item5.png", "towerItem/Item6.png", "towerItem/Item7.png", "towerItem/Item8.png", "towerItem/Item9.png", "towerItem/Item2.png"
};

// 拖动过程中，选中项正常的纹理图，这个正常是能被添加在拖动位置
const char* move_textures[10] =
{
	"towerItem/moveItem1.png", "towerItem/moveItem2.png", "towerItem/moveItem3.png", "towerItem/moveItem4.png", "towerItem/moveItem5.png", "towerItem/moveItem6.png", "towerItem/moveItem7.png", "towerItem/moveItem8.png", "towerItem/moveItem9.png", "towerItem/moveItem2.png"
};

// 拖动过程中，选中项不正常的纹理图
const char* move_textures_en[10] =
{
	"towerItemDisable/Item1.png", "towerItemDisable/Item2.png", "towerItemDisable/Item3.png", "towerItemDisable/Item4.png", "towerItemDisable/Item5.png", "towerItemDisable/Item6.png", "towerItemDisable/Item7.png", "towerItemDisable/Item8.png", "towerItemDisable/Item9.png", "towerItemDisable/Item2.png"
};
//塔的攻击范围，可修改，注意是初始射程，后面升级可修改
float rangeOfTower[10] =
{
	300, 300, 300, 300, 300, 300, 300, 300, 300, 300
};
//塔的攻击范围图片显示修正值,保证在塔在攻击范围的中心
float rangeDelta[10] =
{
	40, 150, 70, 60, 50,40, 40, 40, 40, 40
};

float towerMoveScale[10] =
{
	0.2, 0.2, 0.5, 0.7, 0.7, 0.6, 0.6, 0.6, 0.5, 0
};
float towerMoveScale_revise[10] =
{
	5, 5, 2, 1.4, 1.4, 1.4, 0.6, 0.6, 0.6, 0
};
////////////////////////////////////////////////////////////////////////////cocos studio 

// 炮塔所值价格
const int tower_money[10] =
{
	250, 50, 100, 300, 200, 50, 400, 450, 1000, 300
};
//炮塔攻击力
const std::string tower_attack[10] = 
{
	"5", "10","0","0","15","200","200","200","200","200"
};
//炮塔初始射程，一定注意
const std::string tower_range[10] =
{
	"300", "200", "200", "200", "450", "200", "200", "200", "200", "200"
};
//炮塔出售价格
const std::string tower_sale[10] =
{
	"125", "25", "50", "150", "100", "25", "200", "225", "500", "150"
};
//炮塔出售价格 整 数
const int tower_sale_int[10] =
{
	125,25,50,150,100,25,200,225,500,150
};

//炮塔消耗电力
const std::string tower_power[10] =
{
	"5", "0", "10", "40", "40", "15", "200", "200", "100", "200"
};

//炮塔图片信息 500*500
const std::string pic[10] = 
{
	"towerInfo/Item1.png", "towerInfo/Item2.png", "towerInfo/Item3.png", "towerInfo/Item4.png", "towerInfo/Item5.png", "towerInfo/Item6.png", "towerInfo/Item7.png", "towerInfo/Item8.png", "towerInfo/Item9.png", "towerInfo/Item1"
};

//炮塔升级费用(建造费用的1.5倍)
const std::string tower_upgrade_0[10] = 
{
	"375", "75", "150", "450", "300", "200", "75", "200", "200", "200"
};

int tower_upgrade_0_int[10] =
{
	375,75,150,450,300,200,75,200,200,200
};




//地图信息
const int ROAD = 18;

//攻击范围
#define ATTACK_RANGE_TAG 10001 //注意这个是map上的tag，不要和其他tag重复了

//发电厂电力
#define POWERPLANT_POWER 20

//亚丝娜第四个技能范围
#define ABILITY4_RANGE 300


//炮塔tag范围(下限)，要这样设置是因为不同的塔有着不同的tag范围，否则删除塔的时候不知道该删哪个塔
enum
{
	PRISM_TOWER_TAG=0,
	ARTILLERY_TOWER_TAG=1000,
	ICE_TOWER_TAG = 2000,
	ANTI_TOWER_TAG = 3000,
	MULTI_TOWER_TAG = 4000,
	POWERPLANT_TOWER_TAG = 5000,
	FORETELLING_TOWER_TAG = 8000,
	TRAINING_TOWER_TAG = 10000,
	CAMP_TOWER_TAG = 12000
};


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		log("playing music");
		SimpleAudioEngine::getInstance()->playBackgroundMusic("SwordLand.OGG", true);
	}

	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
	visibleSize = Director::getInstance()->getVisibleSize();

	instance1 = GameManager::getInstance();

	dn = DrawNode::create();
	addChild(dn, 35);



    //初始化变量
	enemyWaveCounter = 0; //敌人波次间间隔技术器
	startCount = 30;//每关开始前倒计时
	isLevelFail = false;
	firstWave = true;
	groupCounter = 0;
	isSuccessful = false;
	isReachable=true;//目的地是否可达
	isTowerListViewOut = false;
	canBuild = true;
	buyTarget = nullptr;
	isFingerAdded = false;
	isTowerInfoButtonClicked = false;
	indicatorTower = nullptr;
	towerRangeIndicator = nullptr;
	isTowerPanelOut = false;
	isHeroSelected = false;
	isForeTellingTowerComeIn = false;
	isSoldierButtonSelected = false;
	isMinerButtonSelected = false;
	isPollutionButtonSelected = false;
	isSoldierInRect = false;
	isMinerInRect = false;
	isAbility2Expire = true;
	highestHpEnemy = nullptr;
	isHeroExisted = true;
	isSoldierSelected = false;
	//亚丝娜4个技能是否可以释放
	isAbility1Ready = true;
	isAbility2Ready = true;
	isAbility3Ready = true;
	isAbility4Ready = true;
	//预言圣坛建造个数
	foreTellingTowerCount = 0;
	//用于小地图，这2个int表示tag，方便正确删除小地图的方块
	enemyCount = 0;
	towerCount = 10000;

	//初始化炮塔移动修正值
	towerMove.push_back(Vec2(0, 16));//聚能塔
	towerMove.push_back(Vec2(0, 16));//哨戒炮

	//初始化楪祈
	yeqi = Sprite::create("yeqi.png");
	yeqi->setScale(1.25);
	addChild(yeqi, 30);
	yeqi->setPosition(-400, 360);

	yeqiBg = Sprite::create("yeqiBg.png");
	yeqiBg->setScaleY(1.8);
	yeqiBg->setScaleX(3.3);
	yeqiBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(yeqiBg, 29);
	yeqiBg->setVisible(false);

	const char *str_yeqi = ((String*)dic->objectForKey("yeqiWords1"))->_string.c_str();
	std::string strTempYeqi = str_yeqi;
	yeqiWords = Label::createWithTTF(strTempYeqi, "b.ttf", 55);
	yeqiWords->setRotationSkewX(15);
	yeqiWords->setPosition(830,360);
	yeqiWords->setVisible(false);
	addChild(yeqiWords,30);

	//初始化敌人波数特效
	enemyWaveBg = Sprite::create("yeqiBg.png");
	enemyWaveBg->setScaleY(1.8);
	enemyWaveBg->setScaleX(3.3);
	enemyWaveBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(enemyWaveBg, 29);
	enemyWaveBg->setVisible(false);

	const char *str_wave = ((String*)dic->objectForKey("enemyWave"))->_string.c_str();
	std::string strTempWave = str_wave;
	enemyWaveWords = Label::createWithTTF(strTempWave, "b.ttf", 100);
	enemyWaveWords->setRotationSkewX(15);
	enemyWaveWords->setPosition(830, 360);
	enemyWaveWords->setVisible(false);
	addChild(enemyWaveWords, 30);

	enemyWaveSprite = Sprite::create("yeqiEnemyWave.png");
	enemyWaveSprite->setScale(1.25);
	addChild(enemyWaveSprite, 30);
	enemyWaveSprite->setPosition(-400, 360);

	char wave[10];
	sprintf(wave, "%d", groupCounter+1);
	std::string strWave = wave;
	enemyWaveCountLabel = Label::createWithTTF(strWave, "b.ttf", 125);
	enemyWaveCountLabel->setColor(Color3B(248, 236, 3));
	enemyWaveCountLabel->setVisible(false);
	enemyWaveCountLabel->setPosition(visibleSize.width-440,visibleSize.height/2);
	addChild(enemyWaveCountLabel, 32);

	//初始化玩家生命值血条
	lifeBar = ProgressTimer::create(Sprite::create("playerHp.png"));
	lifeBar->setScaleX(0.65);
	lifeBar->setScaleY(0.65);
	lifeBar->setType(ProgressTimer::Type::BAR);
	lifeBar->setMidpoint(Point(0, 0.5));
	lifeBar->setBarChangeRate(Point(1, 0));
	lifeBar->setPercentage(100);
	lifeBar->setAnchorPoint(Point(0, 0.5));
	lifeBar->setPosition(60,visibleSize.height-40);
	addChild(lifeBar, 19);

	curLife = maxLife = 50;//初始化生命值为50(满血)

	//初始化玩家头像
	playerHead = Sprite::create("heroIcon.png");
	playerHead->setScale(0.5);
	playerHead->setPosition(40, visibleSize.height - 40);
	addChild(playerHead, 20);

	//初始化玩家受伤
	playerHurt = Sprite::create("playerHurt.png");
	playerHurt->setPosition(visibleSize / 2);
	playerHurt->setVisible(false);
	addChild(playerHurt, 50);
	
	

	GameManager * instance = GameManager::getInstance();
	instance->towerRangeIndicator = towerRangeIndicator;

	//添加炮塔测试
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("artillery.plist");
	
	

    rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode,10); //顶层
	//获取UI控件
	build_btn = rootNode->getChildByName<ui::Button*>("Button_buildTower");
	towerListView = rootNode->getChildByName<ui::ListView*>("ListView_tower");
	towerListViewOriginPos = towerListView->getPosition();

	//获取倒计时控件
	imageCountDown = rootNode->getChildByName<ui::ImageView*>("Image_countDown");
	textCountDown = imageCountDown->getChildByName<ui::Text*>("Text_countDown");

	upDecoration = rootNode->getChildByName<ui::ImageView*>("Image_upDecoration");
	downDecoration = rootNode->getChildByName<ui::ImageView*>("Image_downDecoration");
	towerPanel = rootNode->getChildByName<ui::ImageView*>("Image_towerPanel");    //移动 出售 升级的panel
	heroButton = rootNode->getChildByName<ui::Button*>("Button_hero");
	soldierButton = rootNode->getChildByName<ui::Button*>("Button_soldier");
	minerButton = rootNode->getChildByName<ui::Button*>("Button_miner");
	optionButton = rootNode->getChildByName<ui::Button*>("Button_option");
	optionButton->addClickEventListener([=](Ref * ref){
		auto optionLayer = OptionLayer::create();
		optionLayer->setPosition(0, 0);
		this->addChild(optionLayer, 40);

	});
	//炮塔名字
	towerName = towerPanel->getChildByName<ui::Text*>("Text_description");
	//出售按钮
	towerPanel_sell = towerPanel->getChildByName<ui::ImageView*>("Image_sale");
	towerPanel_isSell = rootNode->getChildByName<ui::ImageView*>("Image_towerPanel_isSell");
	towerPanel_isSell_yes = towerPanel_isSell->getChildByName<ui::ImageView*>("Image_yes");
	towerPanel_isSell_no = towerPanel_isSell->getChildByName<ui::ImageView*>("Image_no");
	//移动按钮
	towerPanel_move = towerPanel->getChildByName<ui::ImageView*>("Image_move");
	//升级按钮
	towerPanel_upgrade = towerPanel->getChildByName<ui::ImageView*>("Image_upgrade");
	towerPanel_isUpgrade = rootNode->getChildByName<ui::ImageView*>("Image_towerPanel_isUpgrade");
	towerPanel_isUpgrade_yes = towerPanel_isUpgrade->getChildByName<ui::ImageView*>("Image_yes");
	towerPanel_isUpgrade_no = towerPanel_isUpgrade->getChildByName<ui::ImageView*>("Image_no");
	text_cost = towerPanel_isUpgrade->getChildByName<ui::Text*>("Text_cost");
	text_preAttack = towerPanel_isUpgrade->getChildByName<ui::Text*>("Text_preAttack");
	text_afterAttack = towerPanel_isUpgrade->getChildByName<ui::Text*>("Text_nextAttack");
	text_preRate = towerPanel_isUpgrade->getChildByName<ui::Text*>("Text_preRate");
	text_afterRate = towerPanel_isUpgrade->getChildByName<ui::Text*>("Text_nextRate");
	//金钱label以及初始化
	moneyPanel = rootNode->getChildByName<ui::ImageView*>("Image_money");
	moneyLabel = moneyPanel->getChildByName<ui::Text* >("money");
	instance->moneyLabel = moneyLabel;
	//instance->setMoney(3000);
	//游戏胜利按钮初始化
	victoryButton = rootNode->getChildByName<ui::ImageView*>("Image_victory");
	victoryButton->setPosition(Point(-500, -500));//开始让其不可见
	victoryButton->addClickEventListener([=](Ref * ref){
		//返回大地图界面 
		instance->clear();//清空单例模式里的数据
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f,LevelSelectScene::createScene()));
	
	});
	//游戏重来按钮初始化
	retryButton = rootNode->getChildByName<ui::ImageView*>("Image_defeat");
	retryButton->setPosition(Point(-500, -500));//开始让其不可见
	retryButton->addClickEventListener([=](Ref * ref){
		//清空单例模式里的数据
		instance->clear();
		//重新开始本关读入本关初始数据 ,暂时都是第一关
		auto info = LoadLevelInfo::createLoadLevelInfo("levelInfo/levelInfo_0.plist");
		info->readLevelInfo(); //在这个函数里面初始化了每一关的地图，敌人数据等，不用在helloWorld里再说明
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HelloWorld::createScene()));

	});
	//敌人波数文本初始化
	imageEnemyWave = rootNode->getChildByName<ui::ImageView*>("Image_EnemyWave");
	textEnemyWave = imageEnemyWave->getChildByName<ui::Text*>("Text_wave");
	char tempWave[20];
	sprintf(tempWave, "WAVE: 1/%d", instance->getGroupNum());
	std::string strTotalWave = tempWave;
	textEnemyWave->setString(strTotalWave);

	char m[20];
	sprintf(m, "%d", instance->getMoney());
	std::string mStr = m;
	moneyLabel->setString(mStr);
	
	//移动点数label初始化
	movePointPanel = rootNode->getChildByName<ui::ImageView*>("Image_movePointAndMana");
	movePointLabel = movePointPanel->getChildByName<ui::Text* >("Text");
	instance->movePointLabel = movePointLabel;
	instance->setMovePoint(10);

	char m1[20];
	sprintf(m1, "%d", instance->getMovePoint());
	std::string mStr1 = m1;
	movePointLabel->setString(mStr1);

	

	//小地图背景
	smallMapBg = rootNode->getChildByName<ui::ImageView*>("Image_smallMapBg");
	smallMapBg->setPosition(Point(smallMapBg->getPosition()+Point(smallMapBg->getBoundingBox().size.width+5,0)));

	//框选士兵按钮的执行函数
	soldierButton->addClickEventListener([=](Ref * ref){
		//log("select soldier");
		if (!isSoldierButtonSelected) //如果士兵没有被选中
		{
			//进入选中状态
			auto fo = FadeOut::create(0.7);
			auto seq = Sequence::create(fo, fo->reverse(), NULL);
			auto rep = RepeatForever::create(seq);
			soldierButton->runAction(rep);
			isSoldierButtonSelected = true;
			//采矿者取消选中状态
			minerButton->setOpacity(255);
			minerButton->stopAllActions();
			isMinerButtonSelected = false;
			isMinerInRect = false;
			//让属性条不可见
			for (int i = 0; i < instance->minerVector.size(); i++)
			{
				auto tempMiner = instance->minerVector.at(i);
				tempMiner->setTimeBarVisibleOrNot(false);
			}

			instance->minerInRectVector.clear();
		

		}
		else  //如果士兵被选中
		{
			soldierButton->setOpacity(255);
			soldierButton->stopAllActions();
			isSoldierButtonSelected = false;
			isSoldierInRect = false;
			//让属性条不可见
			for (int i = 0; i < instance->soldierVector.size(); i++)
			{
				auto tempSoldier = instance->soldierVector.at(i);
				tempSoldier->setTimeBarVisibleOrNot(false);
			}

			instance->soldierInRectVector.clear();
			//士兵部署按钮弹出屏幕
			isSoldierSelected = false;//有士兵被选中了 
			soldierPollutionButton->setVisible(false);

		}
	});
	//框选采矿者按钮的执行函数
	minerButton->addClickEventListener([=](Ref * ref){
		//log("select miner");
		if (!isMinerButtonSelected) //如果采矿者没有被选中
		{
			//进入选中状态
			auto fo = FadeOut::create(0.7);
			auto seq = Sequence::create(fo, fo->reverse(), NULL);
			auto rep = RepeatForever::create(seq);
			minerButton->runAction(rep);
			isMinerButtonSelected = true;
			//士兵按钮取消选中
			soldierButton->setOpacity(255);
			soldierButton->stopAllActions();
			isSoldierButtonSelected = false;
			isSoldierInRect = false;
			//让属性条不可见
			for (int i = 0; i < instance->soldierVector.size(); i++)
			{
				auto tempSoldier = instance->soldierVector.at(i);
				tempSoldier->setTimeBarVisibleOrNot(false);
			}

			instance->soldierInRectVector.clear();

		}
		else  //如果采矿者被选中
		{
			minerButton->setOpacity(255);
			minerButton->stopAllActions();
			isMinerButtonSelected = false;
			isMinerInRect = false;
			//让属性条不可见
			for (int i = 0; i < instance->minerVector.size(); i++)
			{
				auto tempMiner = instance->minerVector.at(i);
				tempMiner->setTimeBarVisibleOrNot(false);
			}

			instance->minerInRectVector.clear();

		}
	});

	//士兵部署时的按钮
	soldierPollutionButton = rootNode->getChildByName<ui::Button*>("Button_pollution");
	soldierPollutionButton->setVisible(false);
	soldierPollutionButton->addClickEventListener([=](Ref * ref){

		log("pollution");
		if (isPollutionButtonSelected == false)//如果该按钮未被选中
		{
			//进入选中状态
			auto fo = FadeOut::create(0.7);
			auto seq = Sequence::create(fo, fo->reverse(), NULL);
			auto rep = RepeatForever::create(seq);
			soldierPollutionButton->runAction(rep);
			isPollutionButtonSelected = true;

			for (int i = 0; i < instance->soldierInRectVector.size(); i++)
			{
				auto tempSoldier = instance->soldierInRectVector.at(i);
				tempSoldier->setIsDeployed(true);
				tempSoldier->getDeployedSprite()->setVisible(true);
			}
		}
		else //取消士兵部署
		{
			soldierPollutionButton->setOpacity(255);
			soldierPollutionButton->stopAllActions();
			isPollutionButtonSelected = false;
			for (int i = 0; i < instance->soldierInRectVector.size(); i++)
			{
				auto tempSoldier = instance->soldierInRectVector.at(i);
				tempSoldier->setIsDeployed(false);
				tempSoldier->getDeployedSprite()->setVisible(false);
			}
		}
	


	});


	//英雄面板
	auto heroPanel = rootNode->getChildByName<ui::ImageView*>("Image_hero");
	instance->heroPanel = heroPanel;
	auto heroPanelClose = heroPanel->getChildByName<ui::Button*>("Button_close");
	heroPanelClose->addClickEventListener([=](Ref * ref){
	
		auto fo = FadeOut::create(0.5);
		auto mt = CallFunc::create([=](){
			heroPanel->setPosition(Point(-1000, 1000));
		});
		auto seq = Sequence::create(fo, mt, NULL);
		heroPanel->runAction(seq);

	});
	//英雄技能说明面板
	heroAbilityPanel = rootNode->getChildByName<ui::ImageView*>("Image_heroAbility");
	heroAbilityDes = heroAbilityPanel->getChildByName<ui::Text*>("Text_description");
	useAbility = heroAbilityPanel->getChildByName<ui::ImageView*>("Image_yes");
	abortAbility = heroAbilityPanel->getChildByName<ui::ImageView*>("Image_no");
	abilityImage = heroAbilityPanel->getChildByName<ui::ImageView*>("Image_display");
	heroAbilityName = heroAbilityPanel->getChildByName<ui::Text*>("Text_name");
	//使用技能
	useAbility->addClickEventListener([=](Ref * ref){
		switch (abilityType)
		{

		case 1:
			if (isAbility1Ready && hero->getMana() >= MANA1)//如果技能1冷却完毕,可以释放
			{
				//技能进入冷却时间
				isAbility1Ready = false;
				coolDown1->setPercentage(100);
				//楪祈动画效果
				yeqiAnimation();

				//技能1特效
				abilityEffect1();


				//首先是耗蓝量(不同技能不同耗蓝):
				hero->setMana(hero->getMana() - MANA1);
				hero->getManaBar()->setPercentage(hero->getMana());

				//让面板消失
				auto fo = FadeOut::create(0.5);
				auto mt = CallFunc::create([=](){
					heroAbilityPanel->setPosition(Point(-1000, 1000));
				});
				auto seq = Sequence::create(fo, mt, NULL);
				heroAbilityPanel->runAction(seq);
			}
			break;
		case 2:
			if (isAbility2Ready && hero->getMana() >= MANA2)//如果技能2冷却完毕,可以释放
			{
				//技能进入冷却时间
				isAbility2Ready = false;
				coolDown2->setPercentage(100);
				//楪祈动画效果
				yeqiAnimation();
				//技能2特效
				abilityEffect2();

				//首先是耗蓝量(不同技能不同耗蓝):
				hero->setMana(hero->getMana() - MANA2);
				hero->getManaBar()->setPercentage(hero->getMana());

				//让面板消失
				auto fo = FadeOut::create(0.5);
				auto mt = CallFunc::create([=](){
					heroAbilityPanel->setPosition(Point(-1000, 1000));
				});
				auto seq = Sequence::create(fo, mt, NULL);
				heroAbilityPanel->runAction(seq);
			}
			break;
		case 3:
			if (isAbility3Ready && hero->getMana() >= MANA3)//如果技能3冷却完毕,可以释放
			{
				//技能进入冷却时间
				isAbility3Ready = false;
				coolDown3->setPercentage(100);
				//楪祈动画效果
				yeqiAnimation();
				//技能3实际效果
				abilityEffect3();

				//首先是耗蓝量(不同技能不同耗蓝):
				hero->setMana(hero->getMana() - MANA3);
				hero->getManaBar()->setPercentage(hero->getMana());

				//让面板消失
				auto fo = FadeOut::create(0.5);
				auto mt = CallFunc::create([=](){
					heroAbilityPanel->setPosition(Point(-1000, 1000));
				});
				auto seq = Sequence::create(fo, mt, NULL);
				heroAbilityPanel->runAction(seq);
			}
			break;
		case 4:
			if (isAbility4Ready && hero->getMana() >= MANA4)//如果技能4冷却完毕,可以释放
			{
				//技能进入冷却时间
				isAbility4Ready = false;
				//楪祈动画效果
				yeqiAnimation();
				//技能3实际效果
				abilityEffect4();

				//首先是耗蓝量(不同技能不同耗蓝):
				hero->setMana(hero->getMana() - MANA4);
				hero->getManaBar()->setPercentage(hero->getMana());

				//让面板消失
				auto fo = FadeOut::create(0.5);
				auto mt = CallFunc::create([=](){
					heroAbilityPanel->setPosition(Point(-1000, 1000));
				});
				auto seq = Sequence::create(fo, mt, NULL);
				heroAbilityPanel->runAction(seq);
			}
			break;

		default:
			break;
		}
		

	});
	//使用技能
	abortAbility->addClickEventListener([=](Ref * ref){

		auto fo = FadeOut::create(0.5);
		auto mt = CallFunc::create([=](){
			heroAbilityPanel->setPosition(Point(-1000, 1000));
		});
		auto seq = Sequence::create(fo, mt, NULL);
		heroAbilityPanel->runAction(seq);
	});


	//英雄的四个技能按钮
	ability1 = rootNode->getChildByName<ui::Button*>("Button_ability1");
	ability2 = rootNode->getChildByName<ui::Button*>("Button_ability2");
	ability3 = rootNode->getChildByName<ui::Button*>("Button_ability3");
	ability4 = rootNode->getChildByName<ui::Button*>("Button_ability4");
	
	
	//英雄四个技能按钮的具体代码
	ability1->addClickEventListener([=](Ref * ref){
	
		if (isAbility1Ready && hero->getMana() >= MANA1)
		{
			abilityType = 1;
			//初始化技能面板说明
			heroAbilityPanel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
			heroAbilityPanel->setOpacity(255);

			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
			const char *str_1 = ((String*)dic->objectForKey("heroAbility1"))->_string.c_str();
			std::string str = str_1;
			heroAbilityDes->setText(str);

			//英雄技能图片更新
			abilityImage->loadTexture("ability1.png");
			//英雄技能名字更新
			const char *str_2 = ((String*)dic->objectForKey("heroAbilityName1"))->_string.c_str();
			std::string str2 = str_2;
			heroAbilityName->setText(str2);


		}

	});
	//技能1冷却遮罩
	coolDown1 = ProgressTimer::create(Sprite::create("coolDown.png"));
	coolDown1->setType(ProgressTimer::Type::RADIAL);
	coolDown1->setScale(0.88);
	coolDown1->setPosition(50,50);
	coolDown1->setReverseDirection(false);
	coolDown1->setPercentage(0);
	ability1->addChild(coolDown1,10);
	//技能2冷却遮罩
	coolDown2 = ProgressTimer::create(Sprite::create("coolDown.png"));
	coolDown2->setType(ProgressTimer::Type::RADIAL);
	coolDown2->setScale(0.88);
	coolDown2->setPosition(50, 50);
	coolDown2->setReverseDirection(false);
	coolDown2->setPercentage(0);
	ability2->addChild(coolDown2, 10);
	//技能3冷却遮罩
	coolDown3 = ProgressTimer::create(Sprite::create("coolDown.png"));
	coolDown3->setType(ProgressTimer::Type::RADIAL);
	coolDown3->setScale(0.88);
	coolDown3->setPosition(50, 50);
	coolDown3->setReverseDirection(false);
	coolDown3->setPercentage(0);
	ability3->addChild(coolDown3, 10);
	//技能4冷却遮罩
	coolDown4 = ProgressTimer::create(Sprite::create("coolDown.png"));
	coolDown4->setType(ProgressTimer::Type::RADIAL);
	coolDown4->setScale(0.88);
	coolDown4->setPosition(50, 50);
	coolDown4->setReverseDirection(false);
	coolDown4->setPercentage(0);
	ability4->addChild(coolDown4, 10);

	ability2->addClickEventListener([=](Ref * ref){

		if (isAbility2Ready && hero->getMana() >= MANA2)
		{
			abilityType = 2;
			//初始化技能面板说明
			heroAbilityPanel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
			heroAbilityPanel->setOpacity(255);

			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
			const char *str_2 = ((String*)dic->objectForKey("heroAbility2"))->_string.c_str();
			std::string str = str_2;
			heroAbilityDes->setText(str);

			//英雄技能图片更新
			abilityImage->loadTexture("ability2.png");
			//英雄技能名字更新
			const char *str_3 = ((String*)dic->objectForKey("heroAbilityName2"))->_string.c_str();
			std::string str3 = str_3;
			heroAbilityName->setText(str3);

		}
	});

	ability3->addClickEventListener([=](Ref * ref){
		if (isAbility3Ready && hero->getMana() >= MANA3)
		{
			abilityType = 3;
			//初始化技能面板说明
			heroAbilityPanel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
			heroAbilityPanel->setOpacity(255);

			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
			const char *str_3 = ((String*)dic->objectForKey("heroAbility3"))->_string.c_str();
			std::string str = str_3;
			heroAbilityDes->setText(str);

			//英雄技能图片更新
			abilityImage->loadTexture("ability3.png");
			//英雄技能名字更新
			const char *str_4 = ((String*)dic->objectForKey("heroAbilityName3"))->_string.c_str();
			std::string str4 = str_4;
			heroAbilityName->setText(str4);

		}
	});

	ability4->addClickEventListener([=](Ref * ref){
		if (isAbility4Ready && hero->getMana() >= MANA4)
		{
			abilityType = 4;
			
			//初始化技能面板说明
			heroAbilityPanel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
			heroAbilityPanel->setOpacity(255);

			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
			const char *str_4 = ((String*)dic->objectForKey("heroAbility4"))->_string.c_str();
			std::string str = str_4;
			heroAbilityDes->setText(str);

			//英雄技能图片更新
			abilityImage->loadTexture("ability4.png");
			//英雄技能名字更新
			const char *str_5 = ((String*)dic->objectForKey("heroAbilityName4"))->_string.c_str();
			std::string str5 = str_5;
			heroAbilityName->setText(str5);

		}
	});

	//开始schedule技能冷却
	this->schedule(schedule_selector(HelloWorld::updateCoolDown),0.1);

	
	//英雄按钮
	heroButton->loadTextureDisabled("heroButton_disabled.png");
	heroButton->addClickEventListener([=](Ref * ref){
		if (isHeroExisted) //如果英雄存在的话
		{

			if (!isHeroSelected) //如果英雄没有被选中
			{
				//进入选中状态
				auto fo = FadeOut::create(0.7);
				auto seq = Sequence::create(fo, fo->reverse(), NULL);
				auto rep = RepeatForever::create(seq);
				heroButton->runAction(rep);
				isHeroSelected = true;
				//四个技能进入游戏界面
				ability1->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, 120)), 0.5f));
				ability2->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, 120)), 0.5f));
				ability3->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, 120)), 0.5f));
				ability4->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, 120)), 0.5f));

			}
			else  //如果英雄被选中
			{
				heroButton->setOpacity(255);
				heroButton->stopAllActions();
				isHeroSelected = false;
				heroDestPointer->setVisible(false);
				//四个技能退出游戏界面
				ability1->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));
				ability2->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));
				ability3->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));
				ability4->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));

			}
		}

	});
	//电量背景和标志
	powerBg = rootNode->getChildByName<ui::ImageView*>("Image_powerBg");
	powerIcon = rootNode->getChildByName<ui::ImageView*>("Image_power");
	powerInfo = rootNode->getChildByName<ui::ImageView*>("Image_powerInfo");
	powerInfoClose = powerInfo->getChildByName<ui::Button*>("Button_close");
	powerIcon->addClickEventListener([=](Ref * ref){
		powerInfo->setVisible(true);
		powerInfo->setOpacity(255);
		powerInfo->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
		
	});
	powerInfoClose->addClickEventListener([=](Ref * ref){
		auto fo = FadeOut::create(0.3);
		auto action = CallFunc::create([=](){
			powerInfo->setVisible(false);
		});
		auto seq = Sequence::create(fo, action, NULL);
		powerInfo->runAction(seq);
	
	});
	//初始化电力
	instance->setRedPower(0);
	instance->setGreenPower(20);
	//初始化listview炮塔说明面板
	towerInfoBg = rootNode->getChildByName<ui::ImageView*>("Image_towerInfo");
	towerInfoBgClose = towerInfoBg->getChildByName<ui::Button*>("Button_close");
	towerInfoBgClose->addClickEventListener([=](Ref * ref){
		isTowerInfoButtonClicked = false;
		auto st = ScaleTo::create(0.3, 1.6, 1.3);
		auto fo = FadeOut::create(0.3);
		auto sp = Spawn::create(st, fo, NULL);
		auto invis = CallFunc::create([=](){
			towerInfoBg->setVisible(false);
		});
		auto seq = Sequence::create(sp, invis, NULL);
		towerInfoBg->runAction(seq);
		

		//获取tag
		int i = towerInfoSelected->getTag();
		//去除listView对应的panel动画效果
		auto tempPanel = static_cast<ui::Layout*>(towerListView->getChildren().at(i));//获取list中每一个panel
		tempPanel->stopAllActions();
		tempPanel->setOpacity(255);

	});
	//初始化listview建筑物说明面板
	buildingInfoBg = rootNode->getChildByName<ui::ImageView*>("Image_buildingInfo");
	buildingInfoBg->setPosition(visibleSize/2);
	buildingInfoBgClose = buildingInfoBg->getChildByName<ui::Button*>("Button_close");
	buildingInfoBgClose->addClickEventListener([=](Ref * ref){
		
		isTowerInfoButtonClicked = false;
		auto st1 = ScaleTo::create(0.3, 1.6, 1.3);
		auto fo1 = FadeOut::create(0.3);
		auto sp1 = Spawn::create(st1, fo1, NULL);
		auto invis = CallFunc::create([=](){
			buildingInfoBg->setVisible(false);
		});
		auto seq1 = Sequence::create(sp1,invis,NULL);
		
		buildingInfoBg->runAction(seq1);

		//获取tag
		int i = towerInfoSelected->getTag();
		//去除listView对应的panel动画效果
		auto tempPanel = static_cast<ui::Layout*>(towerListView->getChildren().at(i));//获取list中每一个panel
		tempPanel->stopAllActions();
		tempPanel->setOpacity(255);

	});

	towerPanel_sell->addClickEventListener([=](Ref * ref){
		
			towerPanel_isSell->setVisible(true);
			towerPanel_isSell->setOpacity(255);
		
	});

	towerPanel_isSell_yes->addClickEventListener([=](Ref * ref){

		//获取tower的type，判断属于哪一种塔
		int towerTag = indicatorTower->getTag();	
		int towerType = indicatorTower->getTowerType();
		//金钱增加
		instance->setMoney(instance->getMoney() + tower_sale_int[towerType]);
		char m[20];
		sprintf(m, "%d", instance->getMoney());
		std::string mStr = m;
		moneyLabel->setString(mStr);

		//从prismTowerVector中移除
		if (towerType == PRISM_TOWER)
		{
			instance->prismTowerVector.eraseObject(dynamic_cast<PrismTower*>(indicatorTower));
		}
		//从反隐塔vector中移除
		if (towerType == ANTI_TOWER)
		{
			instance->antiVisibleTowerVector.eraseObject(dynamic_cast<AntiVisibleTower*>(indicatorTower));
		}

		//log("prism tower vector %d", instance->prismTowerVector.size());


		//如果是预言圣坛 则个数减少1
		if (towerType == FORETELLING_TOWER && foreTellingTowerCount>0)
		{
			foreTellingTowerCount--;
			//如果圣坛个数为0则取消小地图
			if (foreTellingTowerCount == 0) 
			{
				smallMapComeInAnimation(false);
				isForeTellingTowerComeIn = false;
			}
		}
		//去除占地面积
		Point towerSpriteCenterPos = convertTotileCoord(indicatorTower->getPosition());
		int centerX = (int)towerSpriteCenterPos.x;
		int centerY = (int)towerSpriteCenterPos.y;
		removeOrAddLandOccupation(towerType, Point(centerX, centerY),0);//0表示去除

		//小地图处理
		int towerSeq = indicatorTower->getTowerSequence();
		auto tempSmall = smallMap->map->getChildByTag(towerSeq); //得到的node父亲类，要转化为sprite类才行 ,注意移除哪一个绿色方块是根据tag决定的，tower的tag和绿色方块的tag一一对应，是一样的	
		//log("temp small tag %d", indicatorTower->getTag());
		smallMap->greenVector.eraseObject(dynamic_cast<Sprite*>(tempSmall));
		tempSmall->removeFromParent();

		
		
		//电量恢复 如果是卖掉发电厂则要减去多余电力
		if (towerType == POWER_PLANT)
		{
			instance->setGreenPower(instance->getGreenPower() - POWERPLANT_POWER);
			log("sell powerplant");
		}
		else
		{
			instance->setRedPower(instance->getRedPower() - indicatorTower->getPower());
		}
		char temp[10];
		sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
		std::string str = temp;
		powerText->setString(str);

	
		towerRangeIndicator = static_cast<Sprite*>(map->getChildByTag(ATTACK_RANGE_TAG)); //不能少
		if (towerRangeIndicator)
		{
			towerRangeIndicator->removeFromParent();
			towerRangeIndicator = nullptr;
		}
		//动画效果 
		auto st = ScaleTo::create(0.3, 1.0, 0.8);
		auto fo = FadeOut::create(0.3);
		auto sp = Spawn::create(st, fo, NULL);
		auto action = CallFunc::create([=](){
			towerPanel_isSell->setVisible(false);
		});
		auto seq = Sequence::create(sp, action, NULL);	
		towerPanel_isSell->runAction(seq);

		//炮塔panel退出界面
		auto mt = EaseElasticOut::create(MoveTo::create(1, Point(visibleSize.width/2, -towerPanel->getBoundingBox().size.height/2-80)));
		towerPanel->runAction(mt);
		isTowerPanelOut = false;

		//检测电力
		if (instance->getRedPower() <= instance->getGreenPower())
		{
			
			powerRedGreen->loadTexture("powerGreen.png");
			for (int i = 0; i < instance->towerVector.size(); i++)
			{
				auto tempTower = instance->towerVector.at(i);
				//如果该塔耗电
				if (tempTower->getIsPowerConsumption())
				{
					tempTower->noPowerSprite->setVisible(false);
					tempTower->setIsPowerEnough(true);				
				}
			}
		}
		else //电力不足
		{
			powerRedGreen->loadTexture("powerRed.png");
			for (int i = 0; i < instance->towerVector.size(); i++)
			{
				auto tempTower = instance->towerVector.at(i);
				//如果该塔耗电
				if (tempTower->getIsPowerConsumption())
				{
					tempTower->noPowerSprite->setVisible(true);
					tempTower->setIsPowerEnough(false);
				}
			}
			//小地图退出
			if (isForeTellingTowerComeIn && foreTellingTowerCount>0)
			{
				smallMapComeInAnimation(false);
				isForeTellingTowerComeIn = false;
			}
		}

	
		//vector中也要做同样的事,注意2个vector中都要移除  这句话得放最后面才行，否则vector越界
		instance->towerVector.eraseObject(indicatorTower);
		//将选中的炮塔移除
		indicatorTower->removeFromParent();
		indicatorTower = nullptr; //万万不能少，否则崩溃
		

	});

	towerPanel_isSell_no->addClickEventListener([=](Ref * ref){

		towerRangeIndicator = static_cast<Sprite*>(map->getChildByTag(ATTACK_RANGE_TAG)); //不能少
		if (towerRangeIndicator)
		{
			towerRangeIndicator->removeFromParent();
			towerRangeIndicator = nullptr;
		}
		//动画效果 
		auto st = ScaleTo::create(0.3, 1.0, 0.8);
		auto fo = FadeOut::create(0.3);
		auto sp = Spawn::create(st, fo, NULL);

		auto action = CallFunc::create([=](){
			towerPanel_isSell->setVisible(false);
			//log("7777777");
		});
		auto seq = Sequence::create(sp, action, NULL);
		towerPanel_isSell->runAction(seq);

		//炮塔panel退出界面
		auto mt = EaseElasticOut::create(MoveTo::create(1, Point(visibleSize.width / 2, -towerPanel->getBoundingBox().size.height/2-80)));
		towerPanel->runAction(mt);
		isTowerPanelOut = false;

	});

	//移动炮塔的函数
	towerPanel_move->addClickEventListener([=](Ref * ref){

		if (instance->getMovePoint()>0) //当移动点数大于0才能移动
		{

			if (indicatorTower->getTowerType() != 1) //哨戒炮不能移动
			{
				indicatorTower->setMove(true);
				indicatorTower->setIsSelected(true);//设置该建筑被选中
				indicatorTower->addBuildIcon(); //添加指示器

				//将血条隐藏
				indicatorTower->towerHpSprite->setVisible(false);
				indicatorTower->getTowerHp()->setVisible(false);
			}
		}
		else
		{
			auto sb = ScaleBy::create(0.3, 1.2);
			auto seq = Sequence::create(sb, sb->reverse(), NULL);
			movePointPanel->runAction(seq);
		}
			

	});
	//炮塔升级函数
	towerPanel_upgrade->addClickEventListener([=](Ref * ref){
		
		if (indicatorTower->getGrade() < 3 && indicatorTower->getTowerType()<5) //只有炮塔能升级
		{
			int towerType = indicatorTower->getTowerType();
			if (instance->getMoney()>tower_upgrade_0_int[towerType]) //金钱足够才能升级
			{		
				
				towerPanel_isUpgrade->setVisible(true);
				towerPanel_isUpgrade->setOpacity(255);
				towerPanel_isUpgrade->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 70));

				text_cost->setText(tower_upgrade_0[towerType]);

				//攻击力
				char c[10];
				sprintf(c, "%d", indicatorTower->getAttack());
				std::string str = c;
				text_preAttack->setText(str);

				char c1[10];
				sprintf(c1, "%d", indicatorTower->getAttack() * 2);
				std::string str1 = c1;
				text_afterAttack->setText(str1);

				//攻速
				char c2[10];
				sprintf(c2, "%0.1f", indicatorTower->getRate());
				std::string str2 = c2;
				text_preRate->setText(str2);

				char c3[10];
				sprintf(c3, "%0.1f", indicatorTower->getRate() - 0.5);
				std::string str3 = c3;
				text_afterRate->setText(str3);
			}
			else
			{
				auto sb = ScaleBy::create(0.3, 1.2);
				auto seq = Sequence::create(sb, sb->reverse(), NULL);
				moneyPanel->runAction(seq);
			}

		}

	});

	towerPanel_isUpgrade_no->addClickEventListener([=](Ref * ref){
		
		//动画效果 
		auto st = ScaleTo::create(0.3, 1.0, 0.8);
		auto fo = FadeOut::create(0.3);
		auto sp = Spawn::create(st, fo, NULL);

		auto action = CallFunc::create([=](){
			towerPanel_isUpgrade->setVisible(false);
			//log("7777777");
		});
		auto seq = Sequence::create(sp, action, NULL);
		towerPanel_isUpgrade->runAction(seq);
	});

	towerPanel_isUpgrade_yes->addClickEventListener([=](Ref * ref){
		int tempGrade = indicatorTower->getGrade();
		int nextGrade = tempGrade + 1;
		if (nextGrade <= 3)//只能升到3级
		{
			char c[20];
			sprintf(c, "level%d.png", nextGrade);
			std::string str = c;
			indicatorTower->gradeSprite->setTexture(str);
			indicatorTower->gradeSprite->setOpacity(255);
			indicatorTower->setGrade(nextGrade);

			//改变攻击力
			indicatorTower->setAttack(indicatorTower->getAttack()*2);
			//改变攻速
			indicatorTower->setRate(indicatorTower->getRate() - 0.5);
			int towerType = indicatorTower->getTowerType();
			switch (towerType)
			{
			case 0: //光棱塔
				indicatorTower->unschedule(schedule_selector(PrismTower::shoot));
				indicatorTower->schedule(schedule_selector(PrismTower::shoot), indicatorTower->getRate());
				break;
			case 1://哨戒炮
				indicatorTower->unschedule(schedule_selector(ArtilleryTower::shoot));
				indicatorTower->schedule(schedule_selector(ArtilleryTower::shoot), indicatorTower->getRate());
				break;
			case 2: //冰塔
				dynamic_cast<IceTower*>(indicatorTower)->unscheduleShoot();
				dynamic_cast<IceTower*>(indicatorTower)->scheduleShoot(indicatorTower->getRate());
				break;
			case 3://显形水晶
				indicatorTower->setScope(indicatorTower->getScope() + 100);
				break;
			case 4://散射炮塔 
				dynamic_cast<MultiTower*>(indicatorTower)->unschedule(schedule_selector(MultiTower::shoot));
				dynamic_cast<MultiTower*>(indicatorTower)->schedule(schedule_selector(MultiTower::shoot), indicatorTower->getRate());
				indicatorTower->setScope(indicatorTower->getScope() + 50);
				break;
			}
			//必须要重新指定schedule函数，因为初始的是在init中指定的，只会被执行一次，这样rate就不会被重新使用
			

			//减少金钱
			
			instance->setMoney(instance->getMoney() - tower_upgrade_0_int[towerType]);
			char m1[20];
			sprintf(m1, "%d", instance->getMoney());
			std::string mStr1 = m1;
			moneyLabel->setString(mStr1);

		}

		//升级完毕关闭该面板
		auto st = ScaleTo::create(0.3, 1.0, 0.8);
		auto fo = FadeOut::create(0.3);
		auto sp = Spawn::create(st, fo, NULL);

		auto action = CallFunc::create([=](){
			towerPanel_isUpgrade->setVisible(false);
			
		});
		auto seq = Sequence::create(sp, action, NULL);
		towerPanel_isUpgrade->runAction(seq);
	
	});

	//蓝色药水说明面板
	blueBottleIcon = rootNode->getChildByName<ui::ImageView*>("Image_movePointAndMana");
	blueBottleSubIcon = blueBottleIcon->getChildByName<ui::ImageView*>("Image");
	blueBottlePanel = rootNode->getChildByName<ui::ImageView*>("Image_blueBottle");
	blueBottlePanelClose = blueBottlePanel->getChildByName<ui::Button*>("Button_close");
	addManaButton = blueBottlePanel->getChildByName<ui::Button*>("Button_addMana");//英雄回蓝按钮
	blueBottleSubIcon->addClickEventListener([=](Ref * ref){
		blueBottlePanel->setVisible(true);
		blueBottlePanel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
		
			
	});
	blueBottlePanelClose->addClickEventListener([=](Ref * ref){
		blueBottlePanel->setVisible(false);

	});
	addManaButton->addClickEventListener([=](Ref * ref){
		if (instance->getMovePoint()>0)
		{
			instance->setMovePoint(instance->getMovePoint() - 1);
			hero->setMana(100);
			hero->getManaBar()->setPercentage(100);

			char m1[20];
			sprintf(m1, "%d", instance->getMovePoint());
			std::string mStr1 = m1;
			movePointLabel->setString(mStr1);

			//英雄蓝量文本更新
			char t1[10];
			sprintf(t1, "%d/100", hero->getMana());
			std::string str1 = t1;
			hero->getManaLabel()->setString(str1);


		}
		else
		{
			auto sb = ScaleBy::create(0.3, 1.2);
			auto seq = Sequence::create(sb, sb->reverse(), NULL);
			movePointPanel->runAction(seq);
		}

	});


	//电量绿色进度条 以及 电量文本初始化
	powerRedGreen = powerBg->getChildByName<ui::ImageView*>("Image_power");
	powerText = powerBg->getChildByName<ui::Text*>("Text_power");
	instance->powerText = powerText;


	//初始化让这些面板不可见
	towerInfoBg->setVisible(false);
	buildingInfoBg->setVisible(false);
	towerPanel_isSell->setVisible(false);


	//初始化towerList中每项数据
	for (int i = 0; i < 9/*towerListView->getChildrenCount()*/; i++)
	{
		
			auto tempPanel = static_cast<ui::Layout*>(towerListView->getChildren().at(i));//获取list中每一个panel
			tempPanel->setTag(100 + i);//方便后面拖拽物品调用

			ui::ImageView * buySprite = tempPanel->getChildByName<ui::ImageView*>("display");

		    buySprite->loadTexture(tower_textures[i]); //loadtexture加载纹理
			
			//为每一个商品绑定回调函数，用于拖拽商品到地图中
			buySprite->addTouchEventListener(CC_CALLBACK_2(HelloWorld::SpriteCallBack, this));

			ui::Text * moneyText = tempPanel->getChildByName<ui::Text*>("money");
			char temp[10];
			sprintf(temp, "%d", tower_money[i]);
			std::string tempStr = temp;
			moneyText->setText(tempStr);//不能用to_string安卓编译不通过，vs下可以，我勒个去呀

			//炮塔信息显示的回调函数绑定 正方形i
			ui::ImageView * towerInfo = tempPanel->getChildByName<ui::ImageView*>("info");
			towerInfo->setTag(i);
			towerInfo->addTouchEventListener(CC_CALLBACK_2(HelloWorld::towerInfoDisplay, this));
		
		
		
	}


	build_btn->addTouchEventListener([=](Ref * ref, ui::Widget::TouchEventType type){ //注意widget作用域不能少，另一种是addclickevent

		switch (type)
		{
			case ui::Widget::TouchEventType::BEGAN:
			{
				build_btn->runAction(EaseElasticInOut::create(Sequence::create(ScaleBy::create(0.5f, 2), ScaleBy::create(0.2f, 0.5f), NULL), 0.8f));  //这个类是弹性动画类，让动画具有弹性效果
			}
				break;
			case ui::Widget::TouchEventType::MOVED:
			{

			}
				break;
			case ui::Widget::TouchEventType::ENDED:
			{
				if (isTowerListViewOut == true) //如果弹出就收回
				{
					towerListView->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width-30, 0)), 0.5f));   //不同于EaseElasticInOut，这个效果有一个回弹的效果
					build_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width-30, 0)), 0.5f));
					upDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width - 30, 0)), 0.5f));
					downDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width - 30, 0)), 0.5f));
					isTowerListViewOut = false;
					
					if (isFingerAdded == true)
					{
						finger->removeFromParent();
						isFingerAdded = false;
					}
					
				}
				else
				{
					towerListView->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width+30 , 0)), 0.5f));   //不同于EaseElasticInOut，这个效果有一个回弹的效果
					build_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width+30, 0)), 0.5f));
					upDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width + 30, 0)), 0.5f));
					downDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width + 30, 0)), 0.5f));
					isTowerListViewOut = true;
					//添加手势说明
					if (!isFingerAdded)
					{
						finger = Sprite::create("finger.png");
						finger->setPosition(200, visibleSize.height - 80);
						this->addChild(finger, 20);
						isFingerAdded = true;

						//动画
						auto mt = MoveBy::create(2.0, Point(200, 0));
						auto fo = FadeOut::create(0.5);				
						auto action1 = CallFunc::create([=](){
						
							finger->setPosition(200, visibleSize.height - 80);
							finger->setOpacity(255);
						
						});
						auto deleteFinger = CallFunc::create([=](){

							finger->removeFromParent();
							isFingerAdded = false;

						});

						auto seq = Sequence::create(mt, fo, action1,NULL);
						auto rp = Repeat::create(seq, 4);
						auto seq1 = Sequence::create(rp, deleteFinger, NULL);
						finger->runAction(seq1);
					}
				}
			 }
				break;
		}

	});


	map = TMXTiledMap::create(instance->getCurMapName()); //注意这里面不能添加额外路径，只能用最原始的文件名
	map->setAnchorPoint(Point(0, 0)); //注意，锚点在左下角
	map->setPosition(0, 0);
	this->addChild(map, -1);
	instance->map = map;

	object = map->getObjectGroup("enemyWay"); //得到敌人路径点数组
	initPointsVector();//初始化敌人路径点

	//初始化小地图
	smallMap = SmallMap::create();
	smallMap->setScale(0.8);
	smallMap->setPosition(visibleSize.width - smallMap->map->getBoundingBox().size.width / 2+2 + smallMapBg->getBoundingBox().size.width+5, smallMap->map->getBoundingBox().size.height / 2 -2);
	addChild(smallMap, 50);
	instance->smallMap = smallMap;

	//第四个技能瞄准器初始化
	gunSight = Sprite::create("gunsight.png");
	gunSight->setPosition(-1000, -1000);
	map->addChild(gunSight, 40);
	gunSight->setScale(1.3);
	

	//英雄
	hero = Hero::create();
	hero->setPosition(300, 2200);
	instance->hero = hero;
	map->addChild(hero, 20);

	//英雄目标地点指示器
	heroDestPointer = Sprite::create("pointer.png");
	heroDestPointer->setScale(1.5);
	map->addChild(heroDestPointer, 14);
	heroDestPointer->setPosition(-400,-400); //让其不可见
	heroDestPointer->setVisible(false);

	//初始化行走地图 : 注意地图坐标和数组坐标有差别呢!
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			int gid = map->getLayer("bg")->getTileGIDAt(Point(i, j));
			//log("%d ", gid);
			auto p = map->getPropertiesForGID(gid);   //有问题 valueMap没用到
			if (p.isNull() == false && p.asInt() == 0) //0 表示没法行走
			{
				walkMap[i][j] = 1;//1表示没法行走
			}
			else 
			{
				walkMap[i][j] = 0;//0表示可以行走
			}
		}
	}
	instance->walkMap = walkMap;//初始化gameMananger的地图指针

	//初始化矿物资源
	initCrystalMine();


	//创建监听器
	mapListener = EventListenerTouchAllAtOnce::create();
	mapListener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	mapListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	mapListener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	//添加监听器到map上去
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mapListener, map);

	scheduleOnce(schedule_selector(HelloWorld::waitToAddEnemy), 30.0f); //等待30秒添加敌人
	schedule(schedule_selector(HelloWorld::countDown), 1.0f); //倒计时开始
	schedule(schedule_selector(HelloWorld::SoldierPollutionGasDamage), 0.5f); //毒气函数

	scheduleUpdate();
	
    return true;
}


void HelloWorld::onTouchesBegan(const std::vector<Touch*> &touches, Event* event)
{
	//碰到其他地方是清除攻击范围指示器
	towerRangeIndicator = static_cast<Sprite*>(map->getChildByTag(ATTACK_RANGE_TAG)); //注意指示器是添加在map上的，所以要map->getChildByTag
	if (towerRangeIndicator){
		towerRangeIndicator->removeFromParent();
		towerRangeIndicator = nullptr;
		//log("no indicator");
	}


	
	//停止炮塔的动作，恢复不透明
	if (indicatorTower)
	{
		indicatorTower->towerSprite->stopAllActions();
		indicatorTower->towerSprite->setOpacity(255);
	}

	//士兵框选操作,当button被点击后才能执行此操作
	if (isSoldierButtonSelected)
	{
		leftUp = touches[0]->getLocation();//得到左上角的世界坐标
	}

	//采矿者框选操作,当button被点击后才能执行此操作
	if (isMinerButtonSelected)
	{
		leftUp = touches[0]->getLocation();//得到左上角的世界坐标
	}



}
void HelloWorld::onTouchesMoved(const std::vector<Touch*> &touches, Event* event)
{
	auto winSize = Director::getInstance()->getWinSize();
	if (touches.size() == 1 && isHeroSelected == false && !isSoldierButtonSelected && !isMinerButtonSelected) //英雄没选中时 且 框选按钮没选中时才能移动 
	{
		//获得移动量
		auto diff = touches[0]->getDelta();
		//获得map的当前位置
		auto curPos = map->getPosition();
		//计算map的移动后位置
		auto nextPos = curPos + diff;
		//更新map 的位置,注意边界条件
		if (nextPos.x <= 0 && nextPos.x >= -(map->getContentSize().width - winSize.width) && nextPos.y <= 0 && nextPos.y >= -(map->getContentSize().height - winSize.height))
		{
			map->setPosition(nextPos);
		}

	}
	//士兵框选操作
	if (isSoldierButtonSelected && !isSoldierInRect)
	{
		
		dn->clear();
		Point rectangle[4];
		rectangle[0] = { leftUp.x, leftUp.y };
		rectangle[1] = { touches[0]->getLocation().x,leftUp.y};
		rectangle[2] = { touches[0]->getLocation().x,touches[0]->getLocation().y };
		rectangle[3] = { leftUp.x, touches[0]->getLocation().y };
		rightDown = { touches[0]->getLocation().x, touches[0]->getLocation().y };//获取方框右下角坐标
		dn->drawPolygon(rectangle, 4, Color4F(1, 0, 0, 0), 1, Color4F(1, 1, 1, 1));
	}
	//采矿者框选操作
	if (isMinerButtonSelected && !isMinerInRect)
	{

		dn->clear();
		Point rectangle[4];
		rectangle[0] = { leftUp.x, leftUp.y };
		rectangle[1] = { touches[0]->getLocation().x, leftUp.y };
		rectangle[2] = { touches[0]->getLocation().x, touches[0]->getLocation().y };
		rectangle[3] = { leftUp.x, touches[0]->getLocation().y };
		rightDown = { touches[0]->getLocation().x, touches[0]->getLocation().y };//获取方框右下角坐标
		dn->drawPolygon(rectangle, 4, Color4F(1, 0, 0, 0), 1, Color4F(1, 1, 1, 1));
	}
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*> &touches, Event* event)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	GameManager * instance = GameManager::getInstance();
	bool isTowerSelected = false;
	if (touches.size() == 1)
	{
		auto touch = touches[0];
		auto screenPos = touch->getLocation(); //得到世界坐标
		auto mapPos = map->convertToNodeSpaceAR(screenPos);
		
		
		//士兵寻路过程,只有当方框内有人时且按钮选中时才能执行该过程
		if (isSoldierInRect && isSoldierButtonSelected)
		{
			soldierDestVector.clear();
			//先计算出目的地的士兵分布阵型
			int tempSoldierNum = instance->soldierInRectVector.size();
			//log("soldier num %d",tempSoldierNum);

			auto tilePoint1 = convertTotileCoord(mapPos);
			calculateDestPositionSoldierDistribution(tilePoint1, tempSoldierNum);

			if (instance->soldierInRectVector.size() >= soldierDestVector.size() && soldierDestVector.size()!=0) //注意这个if非常重要,由于点击不可行走的地方会导致soldierDestVector.size()=0，则下面访问soldierDestVector.at(i)会越界，所以必须加以判断
			{
				for (int i = 0; i < instance->soldierInRectVector.size(); i++) //注意这里是soldierInRectVector而不是soldierVector
				{
					auto tempSoldier = instance->soldierInRectVector.at(i);
					//士兵目的地
					auto tilePoint = soldierDestVector.at(i);
					//士兵源地点
					auto soldierTilePoint = convertTotileCoord(tempSoldier->getPosition());		
					//如果点击的目的地可以行走才能执行A星算法
					int gid = map->getLayer("bg")->getTileGIDAt(tilePoint);
					log("gid %d", gid);
					auto p = map->getPropertiesForGID(gid);   //有问题 valueMap没用到
					if ((p.isNull()||p.asInt()==2) && walkMap[(int)tilePoint.x][(int)tilePoint.y] == 0) //可以行走的条件
					{
						
						//计算最短路
						tempSoldier->aStar->roadPoint.clear(); //清空的是astar对象的roadpoint
						tempSoldier->openListClear();
						tempSoldier->stopAllActions();
						tempSoldier->pathFinding(soldierTilePoint.x, soldierTilePoint.y, tilePoint.x, tilePoint.y);

						tempSoldier->prevDir = tempSoldier->nowDir = -1;//初始化人物方向
						
						if (tempSoldier->getIsReachable())//如果目的地可达才能走
						{
							tempSoldier->runFollowShortestRoad();//开始沿着最短路行走
						}

						//目的地指示器显示
						tempSoldier->soldierDestPointer->stopAllActions();
						tempSoldier->soldierDestPointer->setOpacity(255);
						tempSoldier->soldierDestPointer->setVisible(true);
						auto screenCoord = convertToScreenCoord(tilePoint);
						tempSoldier->soldierDestPointer->setPosition(screenCoord);

					}
				}
			}
		}

		//士兵框选操作处理
		if (isSoldierButtonSelected && !isSoldierInRect)
		{
			dn->clear();	
			instance->soldierInRectVector.clear();
			
			//计算方框在map上的坐标
			auto mapLeftUp = map->convertToNodeSpaceAR(leftUp);
			auto mapRightDown = map->convertToNodeSpaceAR(rightDown);

			//获取rect左下角坐标:是为了让框选从不同方向进行都能够框到士兵
			auto rectX = mapLeftUp.x > mapRightDown.x ? mapRightDown.x : mapLeftUp.x; 
			auto rectY = mapLeftUp.y > mapRightDown.y ? mapRightDown.y : mapLeftUp.y;

			//log("mapLeftUp: %f %f", mapLeftUp.x, mapLeftUp.y);
			//log("mapRightDown: %f %f", mapRightDown.x, mapRightDown.y);
			auto rect = Rect{ rectX, rectY, abs(mapRightDown.x - mapLeftUp.x), abs(mapRightDown.y - mapLeftUp.y) };//方框rect
			//处理选中的士兵
			for (int i = 0; i < instance->soldierVector.size(); i++)
			{
				auto tempSoldier = instance->soldierVector.at(i);
				if (rect.containsPoint(tempSoldier->getPosition()))
				{
					//log("soldier in rect");
					isSoldierInRect = true;
					
					tempSoldier->setTimeBarVisibleOrNot(true);//可见属性条
					instance->soldierInRectVector.push_back(tempSoldier);//放到rect的vector中去
				}
			}

			if (instance->soldierInRectVector.size() == 0) //如果没有任何士兵被选中
			{
				isSoldierSelected = false;
			}
			else
			{
				isSoldierSelected = true;//有士兵被选中了 
				soldierPollutionButton->setVisible(true);
			}

			
		}

		//采矿者寻路过程,只有当方框内有人时且按钮选中时才能执行该过程
		if (isMinerInRect && isMinerButtonSelected)
		{
			minerDestVector.clear();
			//先计算出目的地的采矿者分布阵型
			int tempMinerNum = instance->minerInRectVector.size();
			//log("soldier num %d",tempSoldierNum);

			auto tilePoint1 = convertTotileCoord(mapPos);
			calculateDestPositionMinerDistribution(tilePoint1, tempMinerNum);

			if (instance->minerInRectVector.size() >= minerDestVector.size() && minerDestVector.size() != 0) //注意这个if非常重要,由于点击不可行走的地方会导致soldierDestVector.size()=0，则下面访问soldierDestVector.at(i)会越界，所以必须加以判断
			{
				for (int i = 0; i < instance->minerInRectVector.size(); i++) //注意这里是InRectVector而不是Vector
				{
					auto tempMiner = instance->minerInRectVector.at(i);
					//士兵目的地
					auto tilePoint = minerDestVector.at(i);
					//士兵源地点
					auto minerTilePoint = convertTotileCoord(tempMiner->getPosition());
					//如果点击的目的地可以行走才能执行A星算法
					int gid = map->getLayer("bg")->getTileGIDAt(tilePoint);
					//log("gid %d", gid);
					auto p = map->getPropertiesForGID(gid);   //有问题 valueMap没用到
					if ((p.isNull() || p.asInt() == 2) && walkMap[(int)tilePoint.x][(int)tilePoint.y] == 0 && walkMap[(int)tilePoint1.x][(int)tilePoint1.y] != 3) //可以行走的条件
					{

						//计算最短路
						tempMiner->aStar->roadPoint.clear(); //清空的是astar对象的roadpoint
						tempMiner->openListClear();
						tempMiner->stopAllActions();
						tempMiner->pathFinding(minerTilePoint.x, minerTilePoint.y, tilePoint.x, tilePoint.y);

						tempMiner->prevDir = tempMiner->nowDir = -1;//初始化人物方向

						if (tempMiner->getIsReachable())//如果目的地可达才能走
						{
							tempMiner->setState(stateIdle);//空闲状态
							tempMiner->runFollowShortestRoad();//开始沿着最短路行走
						}

						//目的地指示器显示
						tempMiner->minerDestPointer->stopAllActions();
						tempMiner->minerDestPointer->setOpacity(255);
						tempMiner->minerDestPointer->setVisible(true);
						auto screenCoord = convertToScreenCoord(tilePoint);
						tempMiner->minerDestPointer->setPosition(screenCoord);

					}
					if (walkMap[(int)tilePoint1.x][(int)tilePoint1.y] == 3)//该地形是水晶矿，执行采矿过程
					{
						//log("crystal mine");
						CrystalMine * targetCrystalMine = nullptr;
						//得到目标水晶矿
						for (int i = 0; i < instance->crystalMineVector.size(); i++)
						{
							auto tempMine = instance->crystalMineVector.at(i);
							if (tempMine->getCrystalBar()->isVisible()) //目标水晶矿
							{
							
								targetCrystalMine = tempMine;
								break;
							}

						}
						//更新水晶矿空闲采矿位置
						auto pos = targetCrystalMine->getPosition();
						//log("target Pos %f %f", pos.x, pos.y);
						auto tilePos = convertTotileCoord(pos);
						//log("tilePos %f %f", tilePos.x, tilePos.y);
						targetCrystalMine->updateVacantMiningPos(tilePos);
						//如果仍然有采矿空闲位置，则开始采矿
						if (targetCrystalMine->getVacantMiningPos().size() != 0)
						{					
							
							auto miningPos = targetCrystalMine->getVacantMiningPos().at(0);	
							tempMiner->setMiningPoint(miningPos);//设置采矿位置
							//log("dest pos %f %f", miningPos.x, miningPos.y);
							//玩家行走到该采矿位置,计算最短路
							tempMiner->aStar->roadPoint.clear(); //清空的是astar对象的roadpoint
							tempMiner->openListClear();
							tempMiner->stopAllActions();
							tempMiner->pathFinding(minerTilePoint.x, minerTilePoint.y, miningPos.x, miningPos.y);					
							

							tempMiner->prevDir = tempMiner->nowDir = -1;//初始化人物方向

							if (tempMiner->getIsReachable())//如果目的地可达才能走
							{
								tempMiner->setIsReached(false);
								tempMiner->getStateSprite()->setTexture("miner_readyToWork.png");//更新状态图标
								tempMiner->setState(stateWalkToMine);//添加miner头文件即可,状态转换为 行走到采矿区域
								tempMiner->runFollowShortestRoad();//开始沿着最短路行走
							}

							//采矿者占地.别忘了恢复占地
							//instance->walkMap[(int)miningPos.x][(int)miningPos.y] = 1;

						}

					}
				}
			}
		}

		//采矿者框选操作处理
		if (isMinerButtonSelected && !isMinerInRect)
		{
			dn->clear();

			//计算方框在map上的坐标
			auto mapLeftUp = map->convertToNodeSpaceAR(leftUp);
			auto mapRightDown = map->convertToNodeSpaceAR(rightDown);

			//获取rect左下角坐标:是为了让框选从不同方向进行都能够框到士兵
			auto rectX = mapLeftUp.x > mapRightDown.x ? mapRightDown.x : mapLeftUp.x;
			auto rectY = mapLeftUp.y > mapRightDown.y ? mapRightDown.y : mapLeftUp.y;

			//log("mapLeftUp: %f %f", mapLeftUp.x, mapLeftUp.y);
			//log("mapRightDown: %f %f", mapRightDown.x, mapRightDown.y);
			auto rect = Rect{ rectX, rectY, abs(mapRightDown.x - mapLeftUp.x), abs(mapRightDown.y - mapLeftUp.y) };//方框rect
			//处理选中的采矿者
			for (int i = 0; i < instance->minerVector.size(); i++)
			{
				auto tempMiner = instance->minerVector.at(i);
				if (rect.containsPoint(tempMiner->getPosition()))
				{
					//log("miner in rect");
					isMinerInRect = true;
					tempMiner->setTimeBarVisibleOrNot(true);//可见属性条
					instance->minerInRectVector.push_back(tempMiner);//放到rect的vector中去
				}
			}
		}

		if (isHeroSelected) //英雄选中时点击屏幕让英雄移动到那去
		{
			heroDestPointer->stopAllActions();
			heroDestPointer->setOpacity(255);
			heroDestPointer->setVisible(true);
			heroDestPointer->setPosition(mapPos);

			auto mt = MoveTo::create(0.5, heroDestPointer->getPosition() - Point(0, 10));
			auto actionBack = CallFunc::create([=](){
				heroDestPointer->setPosition(heroDestPointer->getPosition() + Point(0, 10));
			});
			auto seq = Sequence::create(mt, actionBack, NULL);
			auto rep = RepeatForever::create(seq);
			heroDestPointer->runAction(rep);

			//计算出当前点击位置的tile map瓦片位置
			auto tilePoint = convertTotileCoord(mapPos);
			//log("tile pos: %f %f walkOrNot: %d", tilePoint.x, tilePoint.y, walkMap[(int)tilePoint.x][(int)tilePoint.y]);
			auto heroTilePoint = convertTotileCoord(hero->getPosition());
			//log("hero pos: %f %f", heroTilePoint.x, heroTilePoint.y);

			//如果点击的目的地可以行走才能执行A星算法
			int gid = map->getLayer("bg")->getTileGIDAt(tilePoint);
			auto p = map->getPropertiesForGID(gid-1);   //有问题 valueMap没用到
			
			if ((p.isNull() || p.asInt() == 2) && walkMap[(int)tilePoint.x][(int)tilePoint.y] == 0)
			{
				

				//计算最短路
				roadPoint.clear();
				while (!open.empty()) //不能少！！
				{
					open.pop();
				}

				hero->stopAllActions();
				AStar(heroTilePoint.x, heroTilePoint.y, tilePoint.x, tilePoint.y);
				roadLength = roadPoint.size() - 1;
				prevDir = nowDir = -1;
				if (isReachable)
				{
					runFollowShortestRoad();
				}

			}

		}

		


		if (isHeroSelected == false) //当英雄没选中时才能点击炮塔
		{
			for (int i = 0; i < instance->towerVector.size(); i++)
			{
				auto tempTower = instance->towerVector.at(i);
				//log("tower pos : %f %f", tempTower->getPosition().x, tempTower->getPosition().y);
				//log("tower boundingbox: %f %f", tempTower->towerSprite->getBoundingBox().size.width, tempTower->towerSprite->getBoundingBox().size.height);
				int tag = tempTower->getTag();
				
				Rect towerRect = { tempTower->getPosition().x - tempTower->towerSprite->getBoundingBox().size.width / 2, tempTower->getPosition().y - tempTower->towerSprite->getBoundingBox().size.height / 2, tempTower->towerSprite->getBoundingBox().size.width, tempTower->towerSprite->getBoundingBox().size.height };
				if (towerRect.containsPoint(mapPos))
				{
					//显示被选中的特效
					auto foo = FadeOut::create(0.8);
					auto seq = Sequence::create(foo, foo->reverse(), NULL);
					auto rep = RepeatForever::create(seq);
					tempTower->towerSprite->runAction(rep); //尼玛找了好久的错，必须是sprite的fade动作

					int type = tempTower->getTowerType();
					//log("type %d", type);
					if (type < 5)//如果是炮塔才显示攻击范围
					{

						//显示攻击范围
						auto tempAttackRange = Sprite::create("attackRange_noEnemy.png");
						//计算比例大小
						float rangeScale = tempTower->getScope() / 180.0;  //200是攻击范围图片本来的宽(攻击范围是半径)
						tempAttackRange->setScale(rangeScale);   //要恢复原来的scale
						map->addChild(tempAttackRange, 20, ATTACK_RANGE_TAG);

						instance->towerRangeIndicator = tempAttackRange;

						tempAttackRange->setPosition(tempTower->getPosition().x, tempTower->getPosition().y);
						//让攻击范围旋转
						auto scaleBy = RotateBy::create(2, 90);
						auto rp = RepeatForever::create(scaleBy);
						tempAttackRange->runAction(rp);
					}

					//towerPanel从下往上推出
					auto mt = EaseElasticOut::create(MoveTo::create(1, Point(visibleSize.width / 2, towerPanel->getBoundingBox().size.height / 2)));
					if (!isTowerPanelOut)
					{
						//这里要设置炮塔名字
						Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
						
						char name[20];
						sprintf(name, "towerName%d", tempTower->getTowerType()+1); //注意这里要+1
						std::string strName = name;
						const char *str_name = ((String*)dic->objectForKey(strName))->_string.c_str();					
						std::string strTowerName = str_name;					
						towerName->setText(strTowerName);
			
						towerPanel->runAction(mt);
						isTowerPanelOut = true;
					}
					else  //如果炮塔面板已经显示，则不用播放动画
					{
						towerPanel->setPosition(Point(visibleSize.width / 2, towerPanel->getBoundingBox().size.height / 2));
						isTowerPanelOut = true;
					}

					//indicatorTower是当前选中的炮塔
					indicatorTower = tempTower;
					isTowerSelected = true;
					break;
				}

			}
		}
		//如果没有炮塔被选中 而且 炮塔面板显示 ，则让其移出屏幕
		if (isTowerSelected == false)
		{
			if (isTowerPanelOut)
			{
				auto mt = EaseElasticOut::create(MoveTo::create(1, Point(visibleSize.width / 2, -towerPanel->getBoundingBox().size.height / 2 - 80)));
				towerPanel->runAction(mt);
				isTowerPanelOut = false;

			}
		}

	}

}

void HelloWorld::initPointsVector()
{
	Node* runOfPoint = nullptr;
	ValueMap point;//这个就是object中的每一个键值对
	char c[10];
	int count = 0;
	sprintf(c, "%d", count);
	std::string str = c;
	point = object->getObject(str);//得到objects中的ValueMap
	while (point.begin() != point.end())
	{
		float x = point.at("x").asFloat();
		float y = point.at("y").asFloat();

		runOfPoint = Node::create();
		runOfPoint->setPosition(Point(x, y+20)); //加的20是为了防止所有敌人在一条水平线上路上 20---60随机值
		this->pointsVector.pushBack(runOfPoint); //保存路径点，以后要用
		count++;

		char c1[10];
		sprintf(c1, "%d", count);
		std::string str1 = c1;
		point = object->getObject(str1);
	}
}

void HelloWorld::waitToAddEnemy(float dt)//等待倒计时完成后添加敌人
{
	schedule(schedule_selector(HelloWorld::addEnemyLogic), 3.0f); //添加敌人
}

void HelloWorld::addEnemyLogic(float dt)
{
	
	
	GameManager* instance = GameManager::getInstance();
	//播放第一波敌人的动画
	if (firstWave)
	{
		enemyWaveEffect(1, 1);
		firstWave = false;
	}
	// 1 获取当前波敌人
	GroupEnemy* groupEnemy = this->currentGroup();
	//log("group counter %d", groupCounter);
	if (groupEnemy == NULL){ return; }
	// 2 如果当前波敌人添加完毕 而且 当前波敌人已被消灭 而且 没到最后一波
	if (groupEnemy->getIsFinishedAddGroup() == true &&/* enemyWaveCounter>15*/instance->enemyVector.size() == 0 && groupCounter < instance->getGroupNum() && curLife>0)
	{
		log("next group");
		enemyWaveCounter = 0;
		groupEnemy = this->nextGroup();
	}

	//计数器更新
	//enemyWaveCounter++;
	//log("enemyWaveCounter %d", enemyWaveCounter);
	
	// 每隔dt时间添加一个敌人
	this->addEnemy();
}

void HelloWorld::addEnemy()
{
	GameManager * instance = GameManager::getInstance();
	GroupEnemy* groupEnemy = this->currentGroup(); //得到当前波敌人的信息
	if (groupEnemy == nullptr)
	{
		return;
	}
	auto restEnemy = groupEnemy->getEnemyTotal();
	if (restEnemy <= 0)
	{
		groupEnemy->setIsFinishedAddGroup(true); //当前波敌人已经添加完毕
		return;
	}
	restEnemy--;
	groupEnemy->setEnemyTotal(restEnemy);
	log("restEnemy %d", restEnemy);

	// 依次添加敌人
	EnemyBase* enemy = NULL;
	if (groupEnemy->getType1Total() > 0){
		enemy = EnemyTest::createEnemyTest(pointsVector, groupEnemy->getType1Hp());
		groupEnemy->setType1Total(groupEnemy->getType1Total() - 1);
	}
	else if (groupEnemy->getType2Total() > 0){
		enemy = EnemyRage::createEnemyRage(pointsVector, groupEnemy->getType2Hp());
		groupEnemy->setType2Total(groupEnemy->getType2Total() - 1);
	}
	else if (groupEnemy->getType3Total() > 0){
		enemy = EnemyInvisible::createEnemyInvisible(pointsVector, groupEnemy->getType3Hp());
		groupEnemy->setType3Total(groupEnemy->getType3Total() - 1);
	}
	else if (groupEnemy->getType4Total() > 0){
		enemy = EnemyThrow::createEnemyThrow(pointsVector, groupEnemy->getType4Hp());
		groupEnemy->setType4Total(groupEnemy->getType4Total() - 1);
	}
	else if (groupEnemy->getType5Total() > 0){
		enemy = EnemyDisableTower::createEnemyDisableTower(pointsVector, groupEnemy->getType5Hp());
		groupEnemy->setType5Total(groupEnemy->getType5Total() - 1);
	}

	map->addChild(enemy, 16, enemyCount);
	instance->enemyVector.pushBack(enemy);
	
	//敌人和小地图的敌人要一一对应起来，用tag表示，以便于删除
	enemy->setScale(0.9);

	auto redSprite = Sprite::create("smallMap_enemy.png");
	redSprite->setTag(enemyCount);
	smallMap->map->addChild(redSprite, 10); //要设置tag
	smallMap->redVector.pushBack(redSprite);

	enemyCount++;
    
}

void HelloWorld::collisionDetection()
{
	
	//auto bulletVector = instance1->bulletVector;
	//auto enemyVector = instance1->enemyVector;

	if (instance1->bulletVector.empty() || instance1->enemyVector.empty()){
		return;
	}
	for (int i = 0; i < instance1->bulletVector.size(); i++)
	{
		auto bullet = instance1->bulletVector.at(i);

		//auto temp = bullet->convertToWorldSpace(Point(0, 0));
		auto bulletPos = bullet->convertToWorldSpaceAR(Point(0, 0));//	                 

		auto bulletRect = Rect{ bulletPos.x, bulletPos.y, bullet->getBoundingBox().size.width, bullet->getBoundingBox().size.height };

		for (int j = 0; j < instance1->enemyVector.size(); j++)
		{
			auto enemy = instance1->enemyVector.at(j);
			auto enemyPos = enemy->getPosition();
			auto enemyWorldPos = enemy->convertToWorldSpaceAR(Point(0, 0));
			//auto enemyRect = enemy->getBoundingBox();
			auto enemyRect = Rect{ enemyWorldPos.x, enemyWorldPos.y, enemy->getBoundingBox().size.width, enemy->getBoundingBox().size.height };
			//log("enemyPos: %f %f", enemyWorldPos.x, enemyWorldPos.y);
			//log("bulletPos: %f %f", bulletPos.x, bulletPos.y);
			if (enemyRect.intersectsRect(bulletRect))                     //碰撞检测一定注意二者必须在同一坐标系下，做法是都将其转化到world坐标系下，用AR函数
			{
				//如果敌人是怒气型敌人，则要增加怒气
				int enemyType = enemy->getType();
				if (enemyType == 4)
				{
					increaseRageIfHitEnemyWithRage(enemy);
				}
				

				//敌人掉血
				auto currHp = enemy->getCurrHp();			
				currHp -= bullet->getTag();
				enemy->setCurrHp(currHp);  //别忘了设置hp，否则下次检查hp不会变化
				auto hpPercent = (float)currHp / (float)enemy->getMaxHp();
				enemy->getHpBar()->setPercentage(hpPercent * 100);
				

				if (currHp <= 0)
				{
					//如果敌人是投掷型敌人,则要投掷出蘑菇攻击玩家防御塔
					if (enemyType == 5)
					{
						EnemyThrowMushroomToTower(enemy);
					}
		
					instance1->enemyVector.eraseObject(enemy); //不能用enemyVector，必须用instance的，因为只是一个复制过程	
					//小地图处理 万万不能忘记，这个bug找了好几天
					auto tempSmallRed = smallMap->map->getChildByTag(enemy->getTag()); 				
					smallMap->redVector.eraseObject(dynamic_cast<Sprite*>(tempSmallRed));
					tempSmallRed->removeFromParent();

					enemy->enemyExplosionAnimation();//爆炸动画顺带移除敌人

				}				

				//移除子弹：从屏幕移除，从vector中移除,
						
				instance1->bulletVector.eraseObject(bullet);
				bullet->removeFromParent();
				
				break;
			}
		

		}
	
	}

}

void HelloWorld::increaseRageIfHitEnemyWithRage(EnemyBase* enemy)//如果子弹击中怒气型敌人，则敌人增加怒气
{
	auto enemyRage = dynamic_cast<EnemyRage*>(enemy);
	auto curRage = enemyRage->getRage();
	auto nextRage = curRage + 25;
	if (nextRage >= 100)
	{
		nextRage = 100;
		//加速运动过程
		enemyRage->runFastInPeroid();
		enemyRage->getRageIcon()->setVisible(true);
	}
	enemyRage->setRage(nextRage);
	enemyRage->getRageBar()->setPercentage(nextRage);

}

void HelloWorld::update(float dt)
{
	
	collisionDetection();
	enemyReachedDestination();	
	smallMap->updateTowerPosition(1);
	changeAttackRangeIndicator();
	converLeftDownToMap(Point(0, 0));
	updatePlayerHeadAndLifeBar();
	if (isSuccessful)//如果敌人都被消灭,则进入胜利界面
	{
		unschedule(schedule_selector(HelloWorld::addEnemyLogic)); //取消调度函数
		isSuccessful = false;
		mapListener->setEnabled(false);
		victoryEffect();
	}
	
}
void HelloWorld::buildTowerAccordingToTag(int tag,Point pos)//建造炮塔
{
	GameManager * instance = GameManager::getInstance();
	//log("tag %d", tag);
	switch (tag)
	{
		case 0:  //聚能塔
		{
					
					 //建造炮塔
					 auto prismTower = PrismTower::create();
					 map->addChild(prismTower, 15, PRISM_TOWER_TAG + towerCount++); //tag是有范围的，每一种塔的范围为1000，也就是可以造1000个
					 prismTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 //prismTower->setTag(0); //便于后来调用
					 instance->prismTowerVector.pushBack(prismTower);
					 instance->towerVector.pushBack(prismTower);
					 prismTower->setTowerSequence(towerCount);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(prismTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;
					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);

					 //如果耗电就增加电力消耗
					 if (prismTower->getIsPowerConsumption())
					 {
						 instance->setRedPower(instance->getRedPower() + prismTower->getPower());
						 char temp[20];
						 sprintf(temp, "%d/%d", instance->getRedPower(),instance->getGreenPower());
						 std::string str = temp;
						 powerText->setText(str);
					 }
		}
		break;

		case 1:  //哨戒炮
		{
					 
					 //建造炮塔
					 auto artilleryTower = ArtilleryTower::create();
					 artilleryTower->setScale(0.5);
					 static int towerCount1 = 0;
					 map->addChild(artilleryTower, 15, ARTILLERY_TOWER_TAG + towerCount1); //同样要设置tag
					 artilleryTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(artilleryTower);
					 artilleryTower->setTowerSequence(towerCount);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(artilleryTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;
					 //log("pos: %f %f", mapTileCoord.x, mapTileCoord.y);

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //如果耗电才造
					 if (artilleryTower->getIsPowerConsumption())
					 {
						 instance->setRedPower(instance->getRedPower() + artilleryTower->getPower());
						 
					 }
		}
		break;

		case 2:  //冰塔
		{

					 //建造炮塔
					 auto iceTower = IceTower::create();
					 iceTower->setScale(0.5);
					 static int towerCount2 = 0;
					 map->addChild(iceTower, 15, ICE_TOWER_TAG + towerCount2); //同样要设置tag
					 iceTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(iceTower);
					 iceTower->setTowerSequence(towerCount);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(iceTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;
					 //log("pos: %f %f", mapTileCoord.x, mapTileCoord.y);

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //如果耗电才造
					 if (iceTower->getIsPowerConsumption())
					 {
						 instance->setRedPower(instance->getRedPower() + iceTower->getPower());
						 char temp[20];
						 sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
						 std::string str = temp;
						 powerText->setText(str);
					 }
		}
		break;

		case 3:  //反隐炮塔
		{

					 //建造炮塔
					 auto antiTower = AntiVisibleTower::create();
					 antiTower->setScale(0.7);
					 static int towerCount3 = 0;
					 map->addChild(antiTower, 15, ANTI_TOWER_TAG + towerCount3); //同样要设置tag
					 antiTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(antiTower);
					 instance->antiVisibleTowerVector.pushBack(antiTower);
					 antiTower->setTowerSequence(towerCount);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(antiTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;
					 //log("pos: %f %f", mapTileCoord.x, mapTileCoord.y);

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //如果耗电才造
					 if (antiTower->getIsPowerConsumption())
					 {
						 instance->setRedPower(instance->getRedPower() + antiTower->getPower());
						 char temp[20];
						 sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
						 std::string str = temp;
						 powerText->setText(str);
					 }
		}
		break;

		case 4:  //多方向攻击塔
		{

					 //建造炮塔
					 auto multiTower = MultiTower::create();
					 multiTower->setScale(0.7);
					 static int towerCount4 = 0;
					 map->addChild(multiTower, 15, MULTI_TOWER_TAG + towerCount4); //同样要设置tag
					 multiTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(multiTower);
					 multiTower->setTowerSequence(towerCount);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(multiTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;
					 //log("pos: %f %f", mapTileCoord.x, mapTileCoord.y);

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //如果耗电才造
					 if (multiTower->getIsPowerConsumption())
					 {
						 instance->setRedPower(instance->getRedPower() + multiTower->getPower());
						 char temp[20];
						 sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
						 std::string str = temp;
						 powerText->setText(str);
					 }
		}
	    break;

		case 5: //发电厂
		{

				     //建造发电厂
					auto powerPlantTower = PowerPlantTower::create();
					powerPlantTower->setScale(1.6);
					static int towerCount5 = 0;
					map->addChild(powerPlantTower, 15, POWERPLANT_TOWER_TAG + towerCount5); //同样要设置tag
					powerPlantTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					instance->towerVector.pushBack(powerPlantTower);
					powerPlantTower->setTowerSequence(towerCount);

					//更新系统电力
					instance->setGreenPower(instance->getGreenPower() + POWERPLANT_POWER);
					char temp[20];
					sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
					std::string str = temp;
					powerText->setText(str);

					//测试图片中心节点在map上的位置(tile 坐标)
					Point towerSpriteCenterPos = convertTotileCoord(powerPlantTower->getPosition());
					int centerX = (int)towerSpriteCenterPos.x;
					int centerY = (int)towerSpriteCenterPos.y;
					//log("pos: %f %f", mapTileCoord.x, mapTileCoord.y);
					
					//增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);

					//检测电力是否恢复，这一步不能少
					if (instance->getRedPower() <= instance->getGreenPower())
					{
						//电力进度条颜色更新
						powerRedGreen->loadTexture("powerGreen.png");
						for (int i = 0; i < instance->towerVector.size(); i++)
						{
							auto tempTower = instance->towerVector.at(i);
							//如果该塔耗电
							if (tempTower->getIsPowerConsumption())
							{
								tempTower->noPowerSprite->setVisible(false);
								tempTower->setIsPowerEnough(true);
							}
						}
						//小地图进入
						if (!isForeTellingTowerComeIn && foreTellingTowerCount>0)
						{
							smallMapComeInAnimation(true);
							isForeTellingTowerComeIn = true;
						}
					}
					

					
		}
		break;
		case 6:  //预言圣坛
		{
					 //建造预言圣坛
					 auto foretellingTower = ForetellingTower::create();
					 foretellingTower->setScale(1.6);
					 static int towerCount6 = 0;
					 map->addChild(foretellingTower, 15, FORETELLING_TOWER_TAG + towerCount6); //同样要设置tag
					 foretellingTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(foretellingTower);
					 foretellingTower->setTowerSequence(towerCount);

					 //更新系统电力
					 instance->setRedPower(instance->getRedPower() + foretellingTower->getPower());
					 char temp[10];
					 sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
					 std::string str = temp;
					 powerText->setText(str);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(foretellingTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //小地图进入动画,注意有电力要求，电力足够才能进入
					 if (foreTellingTowerCount == 0 && instance->getRedPower() <= instance->getGreenPower() && isForeTellingTowerComeIn==false)
					 {
						 smallMapComeInAnimation(true);
						 isForeTellingTowerComeIn = true;
					 }
					 //预言圣坛个数增加
					 foreTellingTowerCount++;
                     
		}
		break;
		case 7:  //训练中心
		{
					 //建造预言圣坛
					 auto trainingTower = TrainingTower::create();
					 trainingTower->setScale(1.6);
					 static int towerCount7 = 0;
					 map->addChild(trainingTower, 15, TRAINING_TOWER_TAG + towerCount7); //同样要设置tag
					 trainingTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(trainingTower);
					 trainingTower->setTowerSequence(towerCount);

					 //更新系统电力
					 instance->setRedPower(instance->getRedPower() + trainingTower->getPower());
					 char temp[10];
					 sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
					 std::string str = temp;
					 powerText->setText(str);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(trainingTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //小地图进入动画,注意有电力要求，电力足够才能进入
					 if (foreTellingTowerCount == 0 && instance->getRedPower() <= instance->getGreenPower() && isForeTellingTowerComeIn == false)
					 {
						 smallMapComeInAnimation(true);
						 isForeTellingTowerComeIn = true;
					 }

		}
		break;

		case 8:  //大本营
		{
					 //建造大本营
					 auto campTower = CampTower::create();
					 campTower->setScale(1.2);
					 static int towerCount8 = 0;
					 map->addChild(campTower, 15, CAMP_TOWER_TAG + towerCount8); //同样要设置tag
					 campTower->setPosition(pos + Point(0, buyTarget->getBoundingBox().size.height / 2)); //为了让炮塔不被手指遮住
					 instance->towerVector.pushBack(campTower);
					 campTower->setTowerSequence(towerCount);

					 //更新系统电力
					 instance->setRedPower(instance->getRedPower() + campTower->getPower());
					 char temp[10];
					 sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
					 std::string str = temp;
					 powerText->setText(str);

					 //测试图片中心节点在map上的位置(tile 坐标)
					 Point towerSpriteCenterPos = convertTotileCoord(campTower->getPosition());
					 int centerX = (int)towerSpriteCenterPos.x;
					 int centerY = (int)towerSpriteCenterPos.y;

					 //增加占地面积,更新walkMap就可以了,没有用到towerSize这个变量
					 removeOrAddLandOccupation(tag, Point(centerX, centerY), 1);
					 //小地图进入动画,注意有电力要求，电力足够才能进入
					 if (foreTellingTowerCount == 0 && instance->getRedPower() <= instance->getGreenPower() && isForeTellingTowerComeIn == false)
					 {
						 smallMapComeInAnimation(true);
						 isForeTellingTowerComeIn = true;
					 }
					 //加入vector管理 
					 instance->campTowerVector.push_back(campTower);

		}
			break;
	}
}


void HelloWorld::SpriteCallBack(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{

	log("touch");
	GameManager * instance = GameManager::getInstance();

	Size winSize = Director::getInstance()->getWinSize();
	// 获得所选择的Widget，和它的父Widget（也就是商品项）   Widget是所有小挂件的父类
	ui::Widget* widget = static_cast<ui::Widget*>(ref);
	ui::Widget* parent = static_cast<ui::Widget*>(widget->getParent());
	int tag = parent->getTag();

	switch (type) //cancel:松开按钮时，手指在不按钮上,所以这里必须要在cancel里处理，手指必然不会在按钮上  end:松开按钮时，手指在按钮上

	{
	case ui::Widget::TouchEventType::BEGAN:   //按下手指时放大商品,scaleTo是缩放到多少倍，注意啦，图片在cocostudio中缩放倍数已经被修改了，务必按照那个值来进行处理,不能再是按1来处理
	{
       
	}
	break;

	case ui::Widget::TouchEventType::CANCELED: //取消触摸,放下炮塔到地图

		//金钱不足的提示动画
		if (instance->getMoney() < tower_money[tag - 100])
		{
			auto st = ScaleBy::create(0.3, 1.2);
			auto seq = Sequence::create(st, st->reverse(), NULL);
			moneyPanel->runAction(seq);			
		}
		// 生成炮塔,金钱要足够才能放置炮塔
		if (canBuild == true && instance->getMoney()>=tower_money[tag-100])
		{
			
			//金钱减少
			instance->setMoney(instance->getMoney() - tower_money[tag - 100]);
			char tempMoney[20];
			sprintf(tempMoney, "%d", instance->getMoney());
			std::string strMoney = tempMoney;
			moneyLabel->setString(strMoney);

			// 得到放手时位置
			auto endPos = Vec2(widget->getTouchEndPos().x , widget->getTouchEndPos().y);
			//将世界坐标转化为map的坐标，左下角为原点
			Vec2 mapPos = map->convertToNodeSpaceAR(endPos);

			//log("tag %d", tag);
			buildTowerAccordingToTag(tag-100, mapPos);

			//检查电力是否足够:不足够就停电
			if (instance->getRedPower() > instance->getGreenPower())
			{
				//将绿色电量进度条变为红色
				powerRedGreen->loadTexture("powerRed.png");
				for (int i = 0; i < instance->towerVector.size(); i++)
				{
					auto tempTower = instance->towerVector.at(i);
					//如果该塔耗电
					if (tempTower->getIsPowerConsumption())
					{
						tempTower->noPowerSprite->stopAllActions();
						tempTower->noPowerSprite->setOpacity(255);  //万万不能少，否则闪电图标不会显示正常的透明值
						tempTower->noPowerSprite->setVisible(true);
						tempTower->setIsPowerEnough(false);
						auto fo = FadeOut::create(1);
						auto seq = Sequence::create(fo, fo->reverse(), NULL);
						auto rp = RepeatForever::create(seq);
						tempTower->noPowerSprite->runAction(rp);
					}
				}
				//小地图退出界面
				if (isForeTellingTowerComeIn)
				{			
					smallMapComeInAnimation(false);
					isForeTellingTowerComeIn = false;
			    }

			}
			
			canBuild = false;

			//小地图处理
			auto greenSprite = Sprite::create("smallMap_tower.png");
			smallMap->map->addChild(greenSprite, 10, towerCount); //要设置tag，以便于消除该绿色方块时能够获取该炮塔
			towerCount++;
			//smallMap->map->addChild(greenSprite, 10, instance->towerVector.size());
			smallMap->greenVector.pushBack(greenSprite);

			
		}
		else{
			//// 得到放手时鼠标/手指的屏幕坐标，这个坐标是相对于地图的。所以计算它时应该要考虑到地图的移动和缩放。
			//auto endPos = Vec2((widget->getTouchEndPos().x - bgOrigin.x) / bgSprite->getScale(), (widget->getTouchEndPos().y - bgOrigin.y) / bgSprite->getScale());
			//// 把上面得到的屏幕坐标转换围地图坐标
			//auto coord = convertTotileCoord(endPos);
			//// 再把地图坐标转换为固定的一些屏幕坐标
			//auto screenPos = this->convertToScreenCoord(coord);
			//// 创建提醒项，把它设置在screenPos处
			//auto tips = Sprite::create("tip.png");
			//tips->setPosition(screenPos);
			//bgSprite->addChild(tips, 20);
			//// 让提醒项出现一段时间后移除它
			//tips->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tips)), NULL));
		}

		// 移除buyTarget
		if (buyTarget != nullptr)
		{
			buyTarget->removeFromParent();
			buyTarget = nullptr;
		}
		if (buildIcon != nullptr)
		{ 
			buildIcon->removeFromParent();
			buildIcon = nullptr;
		}
		// 弹出购物滚动面板          
		if (isTowerListViewOut== false)
		{
			towerListView->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width + 30, 0)), 0.5f));   //不同于EaseElasticInOut，这个效果有一个回弹的效果
			build_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width + 30, 0)), 0.5f));
			upDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width + 30, 0)), 0.5f));
			downDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(towerListView->getBoundingBox().size.width + 30, 0)), 0.5f));
			isTowerListViewOut = true;
		}
		break;

	case ui::Widget::TouchEventType::ENDED: //在原来商品上松开手指，没有拖动
		// 移除buyTarget和icon
		if (buyTarget != nullptr)
		{
			buyTarget->removeFromParent();
			buyTarget = nullptr;
		}
		if (buildIcon != nullptr)
		{
			//buildIcon->removeFromParent();
			buildIcon = nullptr;
		}
		
		break;

	case ui::Widget::TouchEventType::MOVED:
		if (isTowerListViewOut)
		{
			towerListView->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width - 30, 0)), 0.5f));   //不同于EaseElasticInOut，这个效果有一个回弹的效果
			build_btn->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width - 30, 0)), 0.5f));
			upDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width - 30, 0)), 0.5f));
			downDecoration->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-towerListView->getBoundingBox().size.width - 30, 0)), 0.5f));
			isTowerListViewOut = false;
			if (isFingerAdded == true)
			{
				finger->removeFromParent();
				isFingerAdded = false;
			}
		}
		if (buyTarget == nullptr) //临时的商品项
		{
			
			buyTarget = Sprite::create(move_textures[tag - 100]);

			//log("tag %d", tag - 100);
			buyTarget->setScale(towerMoveScale[tag-100]);
			// 把buyTarget添加到bgSprite上，这样buyTarget的位置就是相对于bgSprite了。
			map->addChild(buyTarget, 20);
			//添加是否可以建造的icon	
			buildIcon = Sprite::create("canBuild.png");
			buildIcon->setScale(0.3);
			map->addChild(buildIcon, 10);

			//log("buy tag %d", tag);
			if (tag-100 < 5) //如果是炮塔才添加攻击范围
			{
     			//添加攻击范围
				attackRange = Sprite::create("attackRange_noEnemy.png");
				attackRange->setAnchorPoint(Point(0.5, 0.5));
				//计算比例大小
				float rangeScale = rangeOfTower[tag - 100] / 180.0;  //200是攻击范围图片本来的宽(攻击范围是半径)
				attackRange->setScale(rangeScale * towerMoveScale_revise[tag - 100]);   //要恢复原来的scale
				buyTarget->addChild(attackRange, 2);
				attackRange->setPosition(rangeDelta[tag - 100], rangeDelta[tag - 100]); //添加修正误差
				//让攻击范围旋转
				auto scaleBy = RotateBy::create(2, 90);
				auto rp = RepeatForever::create(scaleBy);
				attackRange->runAction(rp);
			}
		
		}
		else  //移动商品项
		{
			Vec2 pos;
			// 因为buyTarget的位置是相对于地图的,而getTouchMovePos得到的是gl坐标(世界坐标)，以左下角为原点
			pos.x = widget->getTouchMovePos().x; //变化的，随着触摸点变化而变化
			pos.y = widget->getTouchMovePos().y;


			//将世界坐标转化为map的坐标，左下角为原点
			Vec2 mapPos = map->convertToNodeSpaceAR(pos );

			
			// 检查是否可以放置炮塔
			towerMoveCheck(mapPos, tag - 100);
		}

		break;

	}
}

//炮塔移动检测
void HelloWorld::towerMoveCheck(Vec2 position, int tag)
{
	//得到map的大小：以格子数计算
	auto mapSize = map->getMapSize();
	//log("mapSize : %f %f", mapSize.width, mapSize.height);
	// 将position转化为地图坐标
	//log("move tag %d", tag);
	auto tilePos = this->convertTotileCoord(position + /*towerMove[tag]*/Point(0,16)); //加上地图格子的一半长度，不是全部长度
	//log("tilePos : %f %f", tilePos.x, tilePos.y);
	// canBliud是用于判断是否可生成瓦片的变量

	bool isIntersect = isBuildTowerIntersected(buyTarget);

	int gid = map->getLayer("bg")->getTileGIDAt(tilePos);
	//log("gid:%d", gid);
	auto p = map->getPropertiesForGID(gid);

	if (!isIntersect && p.isNull()) //无交集，可以建造,Road指的是敌人行走的路
	{
		canBuild = true;
		buyTarget->setTexture(move_textures[tag]);
		buildIcon->setTexture("canBuild.png");

	}
	else //有交集，显示红色炮塔
	{
		buyTarget->setTexture(move_textures_en[tag]); 
		buildIcon->setTexture("cannotBuild.png");
		canBuild = false; 
	}

	
	buyTarget->setPosition(position+Point(0,buyTarget->getBoundingBox().size.height/2)); //为了让炮塔不被手指遮住
	buildIcon->setPosition(position + Point(0, buyTarget->getBoundingBox().size.height / 2) + (tag==8?Point(0,140):Point(0,80)));
	
}

Vec2 HelloWorld::convertTotileCoord(Vec2 position)  //将map坐标转化为tile坐标
{
	auto mapSize = map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = map->getBoundingBox().size.width / map->getMapSize().width;
	auto tileHeight = map->getBoundingBox().size.height / map->getMapSize().height;
	// 把position转换为瓦片坐标，确保得到的是整数
	int posX = position.x / tileWidth;
	int posY = map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}

Vec2 HelloWorld::convertToScreenCoord(Vec2 position) //position是tile坐标
{
	Vec2 mapCoord;

	auto mapSize = map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = map->getBoundingBox().size.width / map->getMapSize().width;
	auto tileHeight = map->getBoundingBox().size.height / map->getMapSize().height;

	mapCoord.x = position.x * tileWidth;
	mapCoord.y = map->getBoundingBox().size.height - position.y * tileHeight;

	

	return mapCoord;
}

bool HelloWorld::isBuildTowerIntersected(Sprite * tower) //检测炮塔和其他炮塔的碰撞
{
	GameManager * instance = GameManager::getInstance();
	bool isIntersect = false;
	for (int i = 0; i < instance->towerVector.size(); i++)
	{
		auto tempTower = instance->towerVector.at(i);
		if (tempTower->getBoundingBox().intersectsRect(tower->getBoundingBox()))
		{
			isIntersect = true;
			break;
		}
	}

	return isIntersect;

}

void HelloWorld::towerInfoDisplay(Ref * ref ,cocos2d::ui::Widget::TouchEventType type)
{
	if (isTowerInfoButtonClicked == false)
	{
		//获取tag
		// 获得所选择的Widget，和它的父Widget（也就是商品项）   Widget是所有小挂件的父类
		ui::Widget* widget = static_cast<ui::Widget*>(ref);
		towerInfoSelected = widget;
		ui::Widget* parent = static_cast<ui::Widget*>(widget->getParent());
		int i = parent->getTag() - 100;
		towerInfoSelected->setTag(i);
		//log("towerInfo tag:%d", i);

		//listView对应的panel动画效果
		auto tempPanel = static_cast<ui::Layout*>(towerListView->getChildren().at(i));//获取list中每一个panel
		auto fo = FadeOut::create(1.5);  
		auto seq = Sequence::create(fo, fo->reverse(), NULL);
		auto rp = RepeatForever::create(seq);
		tempPanel->runAction(rp);
			
		isTowerInfoButtonClicked = true;
		
		towerInfoBg = rootNode->getChildByName<ui::ImageView*>("Image_towerInfo");
		

		if (i < 5)//如果是炮塔
		{		
			towerInfoBg->setVisible(true);
			towerInfoBg->setOpacity(0);
			towerInfoBg->setScale(0.3);
			auto st = ScaleTo::create(0.3, 1.3, 1.1);
			auto fi = FadeIn::create(0.3);
			auto sp = Spawn::create(st, fi, NULL);
			towerInfoBg->runAction(sp);

			auto display = towerInfoBg->getChildByName<ui::ImageView*>("display");
			display->loadTexture(pic[i]);

			auto text_attack = towerInfoBg->getChildByName<ui::Text*>("Text_attack");
			text_attack->setText(tower_attack[i]);

			auto text_range = towerInfoBg->getChildByName<ui::Text*>("Text_range");
			text_range->setText(tower_range[i]);

			auto text_power = towerInfoBg->getChildByName<ui::Text*>("Text_power");
			text_power->setText(tower_power[i]);

			auto text_sale = towerInfoBg->getChildByName<ui::Text*>("Text_sale");
			text_sale->setText(tower_sale[i]);

			//中文描述初始化,放在这里初始化的原因是不能在init中初始化
			//用xml保存中文，记得xml文件要存在resource文件夹里面才行
			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

			const char *str_1 = ((String*)dic->objectForKey("item1"))->_string.c_str();
			std::string temp1 = str_1;

			const char *str_2 = ((String*)dic->objectForKey("item2"))->_string.c_str();
			std::string temp2 = str_2;

			const char *str_3 = ((String*)dic->objectForKey("item3"))->_string.c_str();
			std::string temp3 = str_3;

			const char *str_4 = ((String*)dic->objectForKey("item4"))->_string.c_str();
			std::string temp4 = str_4;

			const char *str_5 = ((String*)dic->objectForKey("item5"))->_string.c_str();
			std::string temp5 = str_5;

			const char *str_6 = ((String*)dic->objectForKey("item6"))->_string.c_str();
			std::string temp6 = str_6;


			// 商品描述
			std::string towerDes[10] =
			{
				temp1, temp2, temp3, temp4, temp5, temp6, temp1, temp1, temp1, temp1
			};

			auto description = towerInfoBg->getChildByName<ui::Text*>("Text_description");
			description->setText(towerDes[i]);
		}
		else//如果是建筑物
		{
			
			
			buildingInfoBg->setVisible(true);
			buildingInfoBg->setOpacity(0);
			buildingInfoBg->setScale(0.3);
			auto st = ScaleTo::create(0.3, 1.3, 1.1);
			auto fi = FadeIn::create(0.3);
			auto sp = Spawn::create(st, fi, NULL);
			buildingInfoBg->runAction(sp);

			auto display = buildingInfoBg->getChildByName<ui::ImageView*>("display");
			//log("display  i %d", i);
			display->loadTexture(pic[i]);

			//中文描述初始化,放在这里初始化的原因是不能在init中初始化
			//用xml保存中文，记得xml文件要存在resource文件夹里面才行
			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

			const char *str_1 = ((String*)dic->objectForKey("item1"))->_string.c_str();
			std::string temp1 = str_1;

			const char *str_2 = ((String*)dic->objectForKey("item2"))->_string.c_str();
			std::string temp2 = str_2;

			const char *str_6 = ((String*)dic->objectForKey("item6"))->_string.c_str();
			std::string temp6 = str_6;

			const char *str_7 = ((String*)dic->objectForKey("item7"))->_string.c_str();
			std::string temp7 = str_7;

			const char *str_8 = ((String*)dic->objectForKey("item8"))->_string.c_str();
			std::string temp8 = str_8;

			const char *str_9 = ((String*)dic->objectForKey("item9"))->_string.c_str();
			std::string temp9 = str_9;

			// 商品描述
			std::string towerDes[10] =
			{
				temp1, temp2, temp1, temp1, temp1, temp6, temp7, temp8, temp9, temp1
			};

			auto description = buildingInfoBg->getChildByName<ui::Text*>("Text_description");
			description->setText(towerDes[i]);

		}


	}

}

void HelloWorld::changeAttackRangeIndicator()
{
	auto tempAttackRangeIndicator = static_cast<Sprite*>(map->getChildByTag(ATTACK_RANGE_TAG));
	if (tempAttackRangeIndicator)
	{
		if (indicatorTower->getNearestEnemy())
		{
			tempAttackRangeIndicator->setTexture("attackRange_hasEnemy.png");
		}
		else
		{
			tempAttackRangeIndicator->setTexture("attackRange_noEnemy.png");
		}
	}
}

void  HelloWorld::converLeftDownToMap(Vec2 pos)
{
	auto temp = map->convertToNodeSpaceAR(pos);
	auto scaleX = (float)temp.x / (float)map->getBoundingBox().size.width;
	auto scaleY = (float)temp.y / (float)map->getBoundingBox().size.height;

	auto smallMapX = scaleX * smallMap->map->getBoundingBox().size.width;
	auto smallMapY = scaleY * smallMap->map->getBoundingBox().size.height;

	auto target = Point(smallMapX, smallMapY);

	smallMap->windowPoint = target;
}

void HelloWorld::AStar(int srcX, int srcY, int destX, int destY)
{
	//A*算法初始化
	memset(visited, false, sizeof(visited));
	memset(openList, false, sizeof(openList));
	for (int i = 0; i<MAP_SIZE; i++)
	for (int j = 0; j<MAP_SIZE; j++)
	{
		g[i][j] = INF;
		nodeMap[i][j] = NULL;
	}
	
	//初始化源节点
	ANode  * src = (ANode*)malloc(sizeof(ANode));
	src->x = srcX;
	src->y = srcY;
	src->parent = NULL;
	src->f = INF; //开始要放入开启列表中
	src->g = 0;
	g[srcX][srcY] = 0;
	nodeMap[srcX][srcY] = src;
	openList[srcX][srcY] = true;
	open.push(src);

	ANode * dest; //不能在这里malloc，因为搜寻周围格子的过程就已经malloc了

	while (!open.empty()) //开启列表不为空,若为空则没有找到目的节点
	{
		//将该节点(F值最小的一个)从开启列表中去除

		ANode * temp = open.top();
		open.pop();
		int tempX = temp->x;
		int tempY = temp->y;
		visited[tempX][tempY] = true; //已加入关闭列表
		//temp->isVisited=true;
		openList[tempX][tempY] = false;//开启列表
		//temp->isInOpenList=false;
		//如果找到目的节点则退出算法
		if (temp->x == destX && temp->y == destY)
		{
			
			dest = temp;
			dest->x = destX;
			dest->y = destY;
			isReachable = true; //目的地可达 此变量不能少

			//保存最短路径
			ANode * t = dest;
			while (t != NULL)
			{
				roadPoint.push_back(Point(t->x, t->y));
				t = t->parent;
			}

			//释放内存
			for (int i = 0; i<MAP_SIZE; i++)
			for (int j = 0; j<MAP_SIZE; j++)
			{
				if (nodeMap[i][j] != NULL)
				{
					free(nodeMap[i][j]);
					nodeMap[i][j] = NULL;
				}
			}

			return;//直接返回
		}

		//先将源节点周围的节点(上下左右8个)加入到开启列表中
		for (int i = 0; i<8; i++)
		{
			int tx, ty;
			tx = tempX + dir[i][0];
			ty = tempY + dir[i][1];
			

			if (isInMapAndValid(tx, ty) && visited[tx][ty] == false ) //如果该节点不在关闭列表中而且可以行走
			{
				ANode  * add = (ANode*)malloc(sizeof(ANode));
				add->x = tx; //不能忘记！！！我勒个去
				add->y = ty;
				float tempG;
				add->h = Heuristic(tx, ty, destX, destY);
				if (i == 0 || i == 2 || i == 4 || i == 6)
				{
					tempG = g[tempX][tempY] + 14;//斜边
				}
				else
				{
					tempG = g[tempX][tempY] + 10;//上下左右
				}

				if (openList[tx][ty] == true)//如果该点在开启列表中则不用再加入进开启列表
				{
					if (tempG < g[tx][ty])
					{
						g[tx][ty] = tempG;
						nodeMap[tx][ty]->f = g[tx][ty] + nodeMap[tx][ty]->h;
						nodeMap[tx][ty]->parent = temp;//设定父亲节点

					}

				}
				else
				{
					g[tx][ty] = tempG;
					add->f = g[tx][ty] + add->h;
					openList[tx][ty] = true;
					open.push(add);//加入到开启列表中
					add->parent = temp;
					nodeMap[tx][ty] = add;//加入到nodeMap中去

				}

			}
		}

	}

	isReachable = false;//目的地不可达	

}

inline bool HelloWorld::isInMapAndValid(int x, int y)
{
	if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && walkMap[x][y] == 0) //==0表示空地,可以行走
		return true;
	return false;
}

inline int HelloWorld::Heuristic(int thisX, int thisY, int destX, int destY)  //估价函数:当前节点到目的节点的曼哈顿距离
{
	return (abs(destX - thisX) + abs(destY - thisY));
}

void HelloWorld::runFollowShortestRoad()
{
	//递归调用该函数以达到不断行走的目的

	if (roadLength != 0)
	{
		//获取当前坐标和下一个坐标：用来判断玩家行走方向
		Point nowPoint = roadPoint[roadLength];
		roadLength--;
		Point nextPoint = roadPoint[roadLength];
		
		nowDir = getHeroDirection(nowPoint, nextPoint);

		if (prevDir != nowDir)
		{
			hero->setAnimation(nowDir);
		}
		prevDir = nowDir;

		Point temp = Point(roadPoint[roadLength].x * 32, 80*32-roadPoint[roadLength].y * 32);
		hero->runAction(Sequence::create(MoveTo::create(0.2, temp) 
			, CallFuncN::create(CC_CALLBACK_0(HelloWorld::runFollowShortestRoad, this))
			, NULL));
	}
	else // 玩家已经到达目的地城市
	{
		heroDestPointer->setVisible(false);
		hero->stopHeroAction(nowDir);
		return;
	}
}

int HelloWorld::getHeroDirection(Point now, Point next) //获取英雄行走方向的函数
{
	int tempX = next.x - now.x;
	int tempY = next.y - now.y;

	if (tempX == 0 && tempY == 1) //向正下方走
	{
		return 5;
	}
	else if (tempX == 0 && tempY == -1)//正上方走
	{
		return 1;
	}
	else if (tempX == 1 && tempY == 0)//正右方走
	{
		return 3;
	}
	else if (tempX == -1 && tempY == 0)//正左方走
	{
		return 7;
	}
	else if (tempX == 1 && tempY == -1)//右上方走
	{
		return 2;
	}
	else if (tempX == -1 && tempY == 1)//左下方走
	{
		return 6;
	}
	else if (tempX == -1 && tempY == -1)//左上方走
	{
		return 0;
	}
	else if (tempX == 1 && tempY == 1)//右下方走
	{
		return 4;
	}



}
void HelloWorld::yeqiAnimation()//楪祈动画效果
{
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	switch (abilityType)
	{
		case 1:
		{
				  const char *str_1 = ((String*)dic->objectForKey("yeqiWords1"))->_string.c_str();
				  std::string strYeqi1 = str_1;
				  yeqiWords->setString(strYeqi1);
				  break;
		}
		case 2:
		{
				  const char *str_2 = ((String*)dic->objectForKey("yeqiWords2"))->_string.c_str();
				  std::string strYeqi2 = str_2;
				  yeqiWords->setString(strYeqi2);
				  break;
		}
		case 3:
		{
				  const char *str_3 = ((String*)dic->objectForKey("yeqiWords3"))->_string.c_str();
				  std::string strYeqi3 = str_3;
				  yeqiWords->setString(strYeqi3);
				  break;
		}
		case 4:
		{
				  const char *str_4 = ((String*)dic->objectForKey("yeqiWords4"))->_string.c_str();
				  std::string strYeqi4 = str_4;
				  yeqiWords->setString(strYeqi4);
				  break;
		}

	}
	//技能3时钟效果
	if (abilityType == 3)
	{
		romeClock = Sprite::create("clock.png");
		romeClock->setPosition(380,visibleSize.height/2);
		addChild(romeClock, 40);

		auto st = ScaleTo::create(1.5, 7);
		auto fo = FadeOut::create(1.0f);
		auto sp = Spawn::createWithTwoActions(st, fo);
		auto del = CallFunc::create([=](){
			romeClock->removeFromParent();
			romeClock = nullptr;
		});
		auto delay = DelayTime::create(2);
		auto seq = Sequence::create(delay,sp, del, NULL);
		romeClock->runAction(seq);
	}
	
	//楪祈动画
	auto mt = MoveTo::create(1, Point(200, 360));
	ActionInterval * delayYeqi = DelayTime::create(2);
	auto mt2 = MoveTo::create(1, Point(-400, 360));
	auto seqYeqi = Sequence::create(mt,delayYeqi,mt2,NULL);
	yeqi->runAction(seqYeqi);

	//楪祈bg动画
	yeqiBg->setVisible(true);
	yeqiBg->setScaleY(0.01);
	auto st = ScaleTo::create(0.9, 3.3, 1.8);
	ActionInterval * delayYeqiBg = DelayTime::create(2);
	auto st2 = ScaleTo::create(0.9, 3.3, 0.01);	
	auto deleteYeqiBg = CallFunc::create([=](){
		yeqiBg->setVisible(false);
	});
	auto seqYeqiBg = Sequence::create(st, delayYeqiBg, st2, deleteYeqiBg, NULL);
	yeqiBg->runAction(seqYeqiBg);

	//文字动画
	ActionInterval * delayWord = DelayTime::create(0.9);
	auto visWord = CallFunc::create([=](){
		yeqiWords->setVisible(true);
	});
	ActionInterval * delayWord2 = DelayTime::create(2);
	auto deleteWord = CallFunc::create([=](){
		yeqiWords->setVisible(false);
	});
	yeqiWords->runAction(Sequence::create(delayWord, visWord, delayWord2,deleteWord, NULL));

	

   
}
void HelloWorld::abilityEffect1()//技能1特效
{
	//技能1特效,每次要添加一个node
	effectNode1 = CSLoader::createNode("Ability1Scene.csb");
	addChild(effectNode1, 11); 

	auto delay = DelayTime::create(7);
	auto timeLine = CSLoader::createTimeline("Ability1Scene.csb");
	timeLine->gotoFrameAndPlay(0);

	//掉血,2秒触发一次
	this->schedule(schedule_selector(HelloWorld::ability1Damage), 2);
	
	//播放特效
	effectNode1->runAction(timeLine);
	
	//停止播放特效 和 删除特效
	auto stopEffect1 = CallFunc::create([=](){
		effectNode1->stopAllActions();
		effectNode1->removeFromParent();
		this->unschedule(schedule_selector(HelloWorld::ability1Damage));
	});
	this->runAction(Sequence::create(delay, stopEffect1, NULL));
}
void HelloWorld::ability1Damage(float dt)//技能1持续减血效果:一次百分比掉血10%
{
	GameManager * instance = GameManager::getInstance();
	for (int i = 0; i < instance->enemyVector.size(); i++)
	{
		auto enemy = instance->enemyVector.at(i);
		//敌人掉血
		auto currHp = enemy->getCurrHp();
		//百分比掉血
		currHp -= enemy->getMaxHp() * 0.1;
		enemy->setCurrHp(currHp);  //别忘了设置hp，否则下次检查hp不会变化
		auto hpPercent = (float)currHp / (float)enemy->getMaxHp();
		enemy->getHpBar()->setPercentage(hpPercent * 100);
		if (currHp <= 0)
		{

			instance->enemyVector.eraseObject(enemy); //不能用enemyVector，必须用instance的，因为只是一个复制过程	
			//小地图处理 万万不能忘记，这个bug找了好几天
			auto tempSmallRed = smallMap->map->getChildByTag(enemy->getTag());
			smallMap->redVector.eraseObject(static_cast<Sprite*>(tempSmallRed));
			tempSmallRed->removeFromParent();
			enemy->enemyExplosionAnimation();//爆炸动画顺带移除敌人

		}
	}

}
void HelloWorld::abilityEffect2()//技能2特效
{
	GameManager * instance = GameManager::getInstance();

	isAbility2Expire = false;
	effectNode2 = CSLoader::createNode("Ability2Effect.csb");
	hero->addChild(effectNode2, 11);
	auto timeLine = CSLoader::createTimeline("Ability2Effect.csb");
	timeLine->gotoFrameAndPlay(0);

	this->schedule(schedule_selector(HelloWorld::ability2Damage),0.1);
	//攻击力增加：攻击力 * 2
	for (int i = 0; i < instance->towerVector.size(); i++)
	{
		auto tower = instance->towerVector.at(i);
		int type = tower->getTowerType();
		if (type != ARTILLERY_TOWER)//哨戒炮不行
		{
			tower->setAttack(tower->getAttack() * 2);
		}
		if (type == ICE_TOWER) //冰塔的特效是攻速增加
		{
			float rate = tower->getRate();		
			dynamic_cast<IceTower*>(tower)->unscheduleShoot();
			dynamic_cast<IceTower*>(tower)->scheduleShoot(rate / 2);
			tower->setRate(rate / 2);
		}
		if (type == ANTI_TOWER) //反隐塔，检测范围扩大
		{
			dynamic_cast<AntiVisibleTower*>(tower)->setScope(tower->getScope() + 100);
		}
	}

	//播放特效
	effectNode2->runAction(timeLine);
	//停止播放特效 和 删除特效
	auto delay = DelayTime::create(15);
	auto st = ScaleTo::create(0.5, 0.01);
		
	auto stopEffect2 = CallFunc::create([=](){
		effectNode2->stopAllActions();
		effectNode2->removeFromParent();
		isAbility2Expire = true;//技能已过期
		this->unschedule(schedule_selector(HelloWorld::ability2Damage));
		//取消所有效果
		for (int i = 0; i < instance->towerVector.size(); i++)
		{		
			auto tower = instance->towerVector.at(i);
			int type = tower->getTowerType();
			if (type != ARTILLERY_TOWER)//哨戒炮不行
			{
     			tower->towerSprite->setTexture(tower_textures[type]);
				tower->setAttack(tower->getAttack() / 2);
			}
			if (type == ICE_TOWER) //冰塔
			{
				float rate = tower->getRate();
				dynamic_cast<IceTower*>(tower)->unscheduleShoot();
				dynamic_cast<IceTower*>(tower)->scheduleShoot(rate * 2);
				tower->setRate(rate * 2);
			}
			if (type == ANTI_TOWER) //反隐塔
			{
				dynamic_cast<AntiVisibleTower*>(tower)->setScope(tower->getScope() - 100);
			}
			
			
		}
	});
	effectNode2->runAction(Sequence::create(delay, st,stopEffect2, NULL));

}
void HelloWorld::ability2Damage(float dt)//技能2效果
{
	GameManager * instance = GameManager::getInstance();
	auto heroPos = hero->getPosition();
	for (int i = 0; i < instance->towerVector.size(); i++)
	{
		auto tower = instance->towerVector.at(i);
		auto towerPos = tower->getPosition();
		int type = tower->getTowerType();
		if (type < 5) //只对炮塔有效
		{
			//如果在技能2范围内
			if ((towerPos.x - heroPos.x)*(towerPos.x - heroPos.x) + (towerPos.y - heroPos.y)*(towerPos.y - heroPos.y) <= hero->ability2Range * hero->ability2Range)
			{


				if (type != ARTILLERY_TOWER)//哨戒炮不行
				{
					tower->towerSprite->setTexture(tower_enhanced[type]);

				}
			}
			else
			{

				if (type != ARTILLERY_TOWER)//哨戒炮不行
				{
					tower->towerSprite->setTexture(tower_textures[type]);
				}
			}
		}
	}
}

void HelloWorld::abilityEffect3() //技能3效果
{
	GameManager * instance = GameManager::getInstance();

	//所有敌人禁锢在原地
	for (int i = 0; i < instance->enemyVector.size(); i++)
	{
		auto enemy = instance->enemyVector.at(i);
		enemy->sched1->setTimeScale(0);
		enemy->unscheduleChangeDirection(); //停止敌人动画播放
		enemy->getEnemySprite()->setColor(Color3B(50, 50, 50));//改变敌人的颜色以表明封印的效果
		enemy->setIsStopped(true);
	}
}

void HelloWorld::abilityEffect4() //技能4效果
{
	GameManager * instance = GameManager::getInstance();

	//技能4特效
	effectNode4 = CSLoader::createNode("Ability4Effect.csb");
	hero->addChild(effectNode4, 11);
	auto timeLine = CSLoader::createTimeline("Ability4Effect.csb");
	timeLine->gotoFrameAndPlay(0);

	
	heroAndEnemyDieEffect();
	//英雄死亡特效
	//heroDieNode = CSLoader::createNode("HeroDie.csb");
	

	//搜寻附近血量最高的敌人
	this->schedule(schedule_selector(HelloWorld::findHighestHpEnemy), 0.01);


	//延时和缩小动作
	auto delay = DelayTime::create(12);
	auto st = ScaleTo::create(0.5, 0.01);

	//播放特效
	effectNode4->runAction(timeLine);
	//停止播放特效
	auto stopEffect = CallFunc::create([=](){
		effectNode4->stopAllActions();
		effectNode4->removeFromParent();
		this->unschedule(schedule_selector(HelloWorld::findHighestHpEnemy));
		gunSight->setPosition(-1000, -1000);
		//死亡特效
		heroAndEnemyDieEffect();
		//敌人和亚丝娜删除
		if (highestHpEnemy)
		{
			instance->enemyVector.eraseObject(highestHpEnemy);
			highestHpEnemy->removeFromParent();
			highestHpEnemy = nullptr;
		}
		hero->removeFromParent();
		isHeroExisted = false;

		heroButton->setOpacity(255);
		heroButton->stopAllActions();
		isHeroSelected = false;
		heroDestPointer->setVisible(false);
		//四个技能退出游戏界面
		ability1->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));
		ability2->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));
		ability3->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));
		ability4->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(0, -120)), 0.5f));

		heroButton->setBright(false); //setTouchDisable并不能改变外观到禁用状态
		instance->hero = nullptr;	

	});
	effectNode4->runAction(Sequence::create(delay, st, stopEffect, NULL));

	auto delayDie = DelayTime::create(12.7);
	auto play = CallFunc::create([=](){

		heroAndEnemyDieEffect();//英雄死亡特效
	});
	//heroDieNode->runAction(Sequence::create(delayDie,play,NULL));

}
void HelloWorld::findHighestHpEnemy(float dt)
{
	GameManager * instance = GameManager::getInstance();
	auto heroPos = hero->getPosition();
	int tempHp = 0;
	for (int i = 0; i < instance->enemyVector.size(); i++)
	{
		auto enemy = instance->enemyVector.at(i);
		auto enemyPos = enemy->getPosition();
		if ((enemyPos.x - heroPos.x)*(enemyPos.x - heroPos.x) + (enemyPos.y - heroPos.y)*(enemyPos.y - heroPos.y) <= ABILITY4_RANGE*ABILITY4_RANGE)
		{
			if (enemy->getCurrHp()>tempHp)
			{
				tempHp = enemy->getCurrHp();
				highestHpEnemy = enemy;
			}
		}
	}
	if (highestHpEnemy)
	{
		gunSight->setPosition(highestHpEnemy->getPosition());
	}
		
}
void HelloWorld::heroAndEnemyDieEffect()//英雄死亡特效
{
	ParticleExplosion* effect = ParticleExplosion::create();
	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("dieEffect.png"));
	effect->setTotalParticles(15);

	//让其为图片本身的颜色
	effect->setStartColor(Color4F(255, 255, 255, 255));
	effect->setEndColor(Color4F(255, 255, 255, 0));

	effect->setStartSize(25.0f);

	effect->setLife(0.5f);
	effect->setSpeed(200);
	effect->setSpeedVar(10);

	effect->setPosition(200, 200);
	hero->addChild(effect, 20);


	//hero->addChild(heroDieNode, 11);
	//
	////播放特效
	//auto timeLine1 = CSLoader::createTimeline("HeroDie.csb");
	//timeLine1->gotoFrameAndPlay(0);
	//heroDieNode->runAction(timeLine1);
}


void HelloWorld::updateCoolDown(float dt)//更新4个技能冷却时间
{
	//技能1
	coolDown1->setPercentage(coolDown1->getPercentage() - (float)COOLDOWN1/(float)100);
	if (coolDown1->getPercentage() <= 0)
	{
		isAbility1Ready = true;
		coolDown1->setPercentage(0);
	}
	//技能2
	coolDown2->setPercentage(coolDown2->getPercentage() - (float)10 / (float)COOLDOWN2);
	if (coolDown2->getPercentage() <= 0)
	{
		isAbility2Ready = true;
		coolDown2->setPercentage(0);
	}
	//技能3
	coolDown3->setPercentage(coolDown3->getPercentage() - (float)10 / (float)COOLDOWN3);
	if (coolDown3->getPercentage() <= 0)
	{
		isAbility3Ready = true;
		coolDown3->setPercentage(0);
	}
	//技能4 只能用一次
	if (!isAbility4Ready)
	{		
		coolDown4->setPercentage(100);
	}
}

void HelloWorld::removeOrAddLandOccupation(int type, Point pos, int removeOrAdd)
{
	int centerX = (int)pos.x;
	int centerY = (int)pos.y;
	switch (type)
	{
			case 5: //发电厂
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY - 1] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY - 1] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY - 1] = removeOrAdd;
				break;
			case 1: //一般炮塔
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY - 1] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY - 1] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY - 1] = removeOrAdd;
			    break;
			case 0: //聚能塔
				walkMap[centerX][centerY] = removeOrAdd;				
				walkMap[centerX - 1][centerY] = removeOrAdd;		
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;				
				break;
			case 2: //冰塔
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				break;
			case 3: //反隐塔
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				break;
			case 4: //多方向攻击塔
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				break;
			case 6://预言圣坛
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY - 1] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY - 1] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY - 1] = removeOrAdd;
				break;
			case 7://训练中心
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY - 1] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY - 1] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY - 1] = removeOrAdd;		
				break;
			case 8://大本营(占地面积大)
				walkMap[centerX][centerY] = removeOrAdd;
				walkMap[centerX - 1][centerY - 1] = removeOrAdd;
				walkMap[centerX - 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY - 1] = removeOrAdd;
				walkMap[centerX + 1][centerY] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY + 1] = removeOrAdd;
				walkMap[centerX - 1][centerY + 1] = removeOrAdd;
				walkMap[centerX + 1][centerY - 1] = removeOrAdd;
				//特殊处理下
				walkMap[centerX ][centerY + 2] = removeOrAdd;
				walkMap[centerX - 1][centerY + 2] = removeOrAdd;
				walkMap[centerX + 1][centerY + 2] = removeOrAdd;

				walkMap[centerX + 2][centerY -1] = removeOrAdd;
				walkMap[centerX + 2][centerY ] = removeOrAdd;
				walkMap[centerX + 2][centerY +1] = removeOrAdd;
				walkMap[centerX + 2][centerY + 2] = removeOrAdd;
				break;
			case 10://水晶矿(左下角为原点)
				walkMap[centerX-1][centerY-1] = removeOrAdd;
				walkMap[centerX ][centerY-1] = removeOrAdd;
				walkMap[centerX-1 ][centerY] = removeOrAdd;
				walkMap[centerX][centerY ] = removeOrAdd;
				walkMap[centerX -1][centerY+1] = removeOrAdd;
				walkMap[centerX][centerY + 1] = removeOrAdd;

				break;
	}
}

void HelloWorld::smallMapComeInAnimation(bool inOrOut)//小地图进入退出动画
{
	if (inOrOut) //进入动画
	{
		//log("in come");
		smallMap->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-smallMapBg->getBoundingBox().size.width - 5, 0)), 0.5f));
		smallMapBg->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(-smallMapBg->getBoundingBox().size.width - 5, 0)), 0.5f));
	}
	else
	{
		smallMap->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(smallMapBg->getBoundingBox().size.width + 5, 0)), 0.5f));
		smallMapBg->runAction(EaseElasticOut::create(MoveBy::create(1, Vec2(smallMapBg->getBoundingBox().size.width + 5, 0)), 0.5f));

	}
}

void  HelloWorld::calculateDestPositionMinerDistribution(cocos2d::Point dest, int minerNum)//计算目的地采矿者分布阵型
{
	//表示方向的数组:自己原点也算
	int dir[9][2] = { { 0, 0 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 } };

	//清空队列
	while (!minerDestQueue.empty())
	{
		minerDestQueue.pop();
	}
	minerDestVector.clear();

	//加入目的地
	minerDestQueue.push(dest);

	while (minerNum > 0 && !minerDestQueue.empty())//注意要有非空这个条件
	{

		auto tempDest = minerDestQueue.front();
		minerDestQueue.pop();

		for (int i = 0; i < 9; i++)
		{
			int tempX = (int)tempDest.x + dir[i][0];
			int tempY = (int)tempDest.y + dir[i][1];
			if (walkMap[tempX][tempY] == 0)//可以行走
			{
				//log("tempx tempy %d %d",tempX,tempY);
				minerNum--;
				walkMap[tempX][tempY] = 1;//占地
				minerDestVector.push_back(Point(tempX, tempY));//加入目的地到vector中去
				if (i != 0)
				{
					minerDestQueue.push(Point(tempX, tempY));//入队.自己那个节点不能入队
				}
				if (minerNum <= 0)
				{
					break;
				}
			}
		}

	}
	//消除占地,一定不能少.否则没法执行寻路过程，因为目的地已经不能走了
	for (int i = 0; i < minerDestVector.size(); i++)
	{
		auto temp = minerDestVector.at(i);
		walkMap[(int)temp.x][(int)temp.y] = 0;
	}
}

void HelloWorld::calculateDestPositionSoldierDistribution(cocos2d::Point dest, int soldierNum)//计算目的地士兵分布阵型,dest是tile坐标   bfs过程
{
	//表示方向的数组:自己原点也算
	int dir[9][2] = {{0,0},{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 } };

	//清空队列
	while (!soldierDestQueue.empty())
	{
		soldierDestQueue.pop();
	}
	soldierDestVector.clear();
	
	//加入目的地
	soldierDestQueue.push(dest);

	while (soldierNum > 0 && !soldierDestQueue.empty())//注意要有非空这个条件
	{
		
		auto tempDest = soldierDestQueue.front();
		soldierDestQueue.pop();

		for (int i = 0; i < 9; i++)
		{
			int tempX = (int)tempDest.x + dir[i][0];
			int tempY = (int)tempDest.y + dir[i][1];
			if (walkMap[tempX][tempY] == 0)//可以行走
			{
				//log("tempx tempy %d %d",tempX,tempY);
				soldierNum--;
				walkMap[tempX][tempY] = 1;//占地
				soldierDestVector.push_back(Point(tempX, tempY));//加入目的地到vector中去
				if (i != 0)
				{
					soldierDestQueue.push(Point(tempX, tempY));//入队.自己那个节点不能入队
				}
				if (soldierNum <= 0)
				{
					break;
				}
			}
		}

	}
	//消除占地,一定不能少.否则没法执行寻路过程，因为目的地已经不能走了
	for (int i = 0; i < soldierDestVector.size(); i++)
	{
		auto temp = soldierDestVector.at(i);
		walkMap[(int)temp.x][(int)temp.y] = 0;
	}

}

void HelloWorld::EnemyThrowMushroomToTower(EnemyBase* enemy)//投掷型敌人投掷出蘑菇来攻击玩家防御塔
{
	auto pos = enemy->getPosition();
	GameManager* instance = GameManager::getInstance();
	Vector<Node*> vec;
	int dirTotal = 6;
	//向四面八方投掷出6个蘑菇
	float timeToDelay = 3.3f;
	for (int i = 0; i < dirTotal; i++)
	{
		//该敌人不加入到enemyVector中,而是有一定的生存时间，时间到了就自己死亡
		auto enemyMushroom = EnemyThrow_son::createEnemyThrow_son(vec, 10,timeToDelay+i*0.1);//10能量值,所有蘑菇不是同一时间开始训寻路
		enemyMushroom->setPosition(pos);
		instance->map->addChild(enemyMushroom,30); 
		
		auto moveDuration = 3;
		Point shootVector;
		shootVector.x = 1;
		shootVector.y = tan(i * 2 * M_PI / dirTotal);
		Point normalizedShootVector;
		if (i >= dirTotal / 2)
		{
			normalizedShootVector = shootVector.getNormalized();
		}
		else{
			normalizedShootVector = -shootVector.getNormalized();
		}
		float farthestDistance = 300;
		Point overshotVector = normalizedShootVector * farthestDistance;
		Point offscreenPoint = (pos - overshotVector);

		enemyMushroom->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint), NULL));
	}
}

GroupEnemy* HelloWorld::currentGroup()
{
	GameManager* instance = GameManager::getInstance();
	GroupEnemy * groupEnemy;
	if (!instance->groupVector.empty())
	{
		groupEnemy = (GroupEnemy*)instance->groupVector.at(groupCounter);
		//log("group counter %d", groupCounter);
	}
	else
		groupEnemy = nullptr;

	return groupEnemy;
}

GroupEnemy* HelloWorld::nextGroup()
{
	GameManager* instance = GameManager::getInstance();
	if (instance->groupVector.empty())
	{
		return NULL;
	}

	if (groupCounter < instance->getGroupNum() - 1)
	{
		//每波敌人的动画效果
		enemyWaveEffect(1, 1);
		groupCounter++;
		//更新敌人波数文本
		char tempWave[20];
		sprintf(tempWave, "WAVE: %d/%d", groupCounter+1,instance->getGroupNum());
		std::string strTotalWave = tempWave;
		textEnemyWave->setString(strTotalWave);
	}
	else
	{
		isSuccessful = true;//本关敌人都被消灭
	}
	
	GroupEnemy* groupEnemy = (GroupEnemy*)instance->groupVector.at(groupCounter);
	return groupEnemy;
}

void HelloWorld::enemyWaveEffect(int enemyType, int enemyWave)//每一波敌人的特效：包含敌人介绍 和 波数信息
{

	

	GameManager * instance = GameManager::getInstance();

	//楪祈动画
	auto mt = MoveTo::create(1, Point(200, 360));
	ActionInterval * delayYeqi = DelayTime::create(2);
	auto mt2 = MoveTo::create(1, Point(-400, 360));
	auto seqYeqi = Sequence::create(mt, delayYeqi, mt2, NULL);
	enemyWaveSprite->runAction(seqYeqi);

	//楪祈bg动画
	enemyWaveBg->setVisible(true);
	enemyWaveBg->setScaleY(0.01);
	auto st = ScaleTo::create(0.9, 3.3, 1.8);
	ActionInterval * delayYeqiBg = DelayTime::create(2);
	auto st2 = ScaleTo::create(0.9, 3.3, 0.01);
	auto deleteYeqiBg = CallFunc::create([=](){
		enemyWaveBg->setVisible(false);
	});
	auto seqYeqiBg = Sequence::create(st, delayYeqiBg, st2, deleteYeqiBg, NULL);
	enemyWaveBg->runAction(seqYeqiBg);

	//文字动画
	ActionInterval * delayWord = DelayTime::create(0.9);
	auto visWord = CallFunc::create([=](){
		enemyWaveWords->setVisible(true);
		enemyWaveCountLabel->setVisible(true);

		char temp[2];
		sprintf(temp, "%d", groupCounter + 1);
		std::string str = temp;
		if (groupCounter + 1 == instance->getGroupNum()) //最后一波
		{
			str = "FINAL WAVE!";
			enemyWaveCountLabel->setVisible(false);
			enemyWaveWords->setVisible(true);
			enemyWaveWords->setString(str);
		}
		else
		{
			enemyWaveCountLabel->setString(str);
		}
	});
	ActionInterval * delayWord2 = DelayTime::create(2);
	auto deleteWord = CallFunc::create([=](){
		enemyWaveWords->setVisible(false);
		enemyWaveCountLabel->setVisible(false);
	});
	enemyWaveWords->runAction(Sequence::create(delayWord, visWord, delayWord2, deleteWord, NULL));
}

void HelloWorld::victoryEffect()//本关过关提示
{
	GameManager * instance = GameManager::getInstance();
	
	//楪祈动画
	auto mt = MoveTo::create(1, Point(200, 360));
	auto seqYeqi = Sequence::create(mt, NULL);
	enemyWaveSprite->runAction(seqYeqi);

	//楪祈bg动画
	enemyWaveBg->setVisible(true);
	enemyWaveBg->setScaleY(0.01);
	auto st = ScaleTo::create(0.9, 3.3, 1.8);
	auto seqYeqiBg = Sequence::create(st, NULL);
	enemyWaveBg->runAction(seqYeqiBg);

	//文字动画
	ActionInterval * delayWord = DelayTime::create(0.9);
	auto visWord = CallFunc::create([=](){
		enemyWaveWords->setPosition(enemyWaveWords->getPosition() - Point(70, 0));
		enemyWaveWords->setVisible(true);
		Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
		const char *str_vic = ((String*)dic->objectForKey("victory"))->_string.c_str();
		std::string str = str_vic;
		enemyWaveWords->setString(str);

		victoryButton->setPosition(Point(visibleSize.width/2+80,visibleSize.height/2-250));
		
	});
	
	enemyWaveWords->runAction(Sequence::create(delayWord, visWord, NULL));
}
void HelloWorld::defeatEffect()//本关失败提示
{
	GameManager * instance = GameManager::getInstance();

	//楪祈动画
	auto mt = MoveTo::create(1, Point(200, 360));
	auto seqYeqi = Sequence::create(mt, NULL);
	enemyWaveSprite->runAction(seqYeqi);

	//楪祈bg动画
	enemyWaveBg->setVisible(true);
	enemyWaveBg->setScaleY(0.01);
	auto st = ScaleTo::create(0.9, 3.3, 1.8);
	auto seqYeqiBg = Sequence::create(st, NULL);
	enemyWaveBg->runAction(seqYeqiBg);

	//文字动画
	ActionInterval * delayWord = DelayTime::create(0.9);
	auto visWord = CallFunc::create([=](){
		enemyWaveWords->setPosition(enemyWaveWords->getPosition() - Point(70, 0));
		enemyWaveWords->setVisible(true);
		Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
		const char *str_vic = ((String*)dic->objectForKey("defeat"))->_string.c_str();
		std::string str = str_vic;
		enemyWaveWords->setString(str);

		victoryButton->setScale(0.8);
		victoryButton->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 250));

		retryButton->setScale(0.8);
		retryButton->setPosition(Point(visibleSize.width / 2 + 200, visibleSize.height / 2 - 250));

	});

	enemyWaveWords->runAction(Sequence::create(delayWord, visWord, NULL));
}

void HelloWorld::enemyReachedDestination()//敌人到达目的地（突破玩家防线）
{
	
	
	for (int i = 0; i < instance1->enemyVector.size(); i++)
	{
		auto enemy = instance1->enemyVector.at(i);
		if (enemy->getEnemySuccessful())
		{
			if (!isLevelFail) //如果本关还没有失败
			{
				//屏幕出现红色受伤动画
				playerHurt->setVisible(true);
				playerHurt->stopAllActions();
				playerHurt->setOpacity(0);
				auto fi = FadeIn::create(0.5);
				auto fo = FadeOut::create(0.5);
				auto seq = Sequence::create(fi,fo, NULL);
				playerHurt->runAction(seq);


				//玩家减血
				curLife--;
				float percentage = (float)curLife / (float)maxLife * 100;
				lifeBar->setPercentage(percentage);

				if (curLife <= 0) //本关失败
				{
					isLevelFail = true;
					mapListener->setEnabled(false);
					defeatEffect();//失败画面

				}
			}

			
			instance1->enemyVector.eraseObject(enemy); 
			auto tempSmallRed = instance1->smallMap->map->getChildByTag(enemy->getTag());
			instance1->smallMap->redVector.eraseObject(static_cast<Sprite*>(tempSmallRed));
			tempSmallRed->removeFromParent();
			enemy->removeFromParent();
		}
	}
}

void HelloWorld::updatePlayerHeadAndLifeBar()//让玩家头像和血条保持正常状态
{
	if (towerListView->getPosition().x <=0)
	{
		lifeBar->setVisible(true);
		playerHead->setVisible(true);
	}
	else
	{
		lifeBar->setVisible(false);
		playerHead->setVisible(false);
	}
}

void HelloWorld::countDown(float dt)//倒计时函数
{
	startCount--;
	char temp[10];
	sprintf(temp, "%d", startCount);
	std::string str = temp;
	textCountDown->setString(str);

	if (startCount <= 0)
	{
		unschedule(schedule_selector(HelloWorld::countDown));
		imageCountDown->removeFromParent();
	}
}

void HelloWorld::initCrystalMine()// 初始化矿物资源
{
	//测试水晶矿代码,有6块左右的水晶矿
	auto instance = GameManager::getInstance();
	//水晶矿1
	auto crystal1 = CrystalMine::create();
	crystal1->setAnchorPoint(Point(0, 0));	
	map->addChild(crystal1, 5);
	crystal1->setPosition(32*4, 32*58-16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos1 = convertTotileCoord(crystal1->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos1, 3);//3代表该地形式水晶矿，不能行走
	instance->crystalMineVector.pushBack(crystal1);

	//水晶矿2
	auto crystal2 = CrystalMine::create();
	crystal2->setAnchorPoint(Point(0, 0));
	map->addChild(crystal2, 5);
	crystal2->setPosition(32 * 2, 32 * 60 - 16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos2 = convertTotileCoord(crystal2->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos2, 3);
	instance->crystalMineVector.pushBack(crystal2);

	//水晶矿3
	auto crystal3 = CrystalMine::create();
	crystal3->setAnchorPoint(Point(0, 0));
	map->addChild(crystal3, 5);
	crystal3->setPosition(32 * 2, 32 * 63 - 16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos3 = convertTotileCoord(crystal3->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos3, 3);
	instance->crystalMineVector.pushBack(crystal3);

	//水晶矿4
	auto crystal4 = CrystalMine::create();
	crystal4->setAnchorPoint(Point(0, 0));
	map->addChild(crystal4, 5);
	crystal4->setPosition(32 * 7, 32 * 58 - 16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos4 = convertTotileCoord(crystal4->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos4, 3);
	instance->crystalMineVector.pushBack(crystal4);

	//水晶矿5
	auto crystal5 = CrystalMine::create();
	crystal5->setAnchorPoint(Point(0, 0));
	map->addChild(crystal5, 5);
	crystal5->setPosition(32 * 10, 32 * 57 - 16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos5 = convertTotileCoord(crystal5->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos5, 3);
	instance->crystalMineVector.pushBack(crystal5);

	//水晶矿6
	auto crystal6 = CrystalMine::create();
	crystal6->setAnchorPoint(Point(0, 0));
	map->addChild(crystal6, 5);
	crystal6->setPosition(32 * 1, 32 * 66 - 16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos6 = convertTotileCoord(crystal6->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos6, 3);
	//log("pos %f %f", crystalSpriteCenterPos6.x, crystalSpriteCenterPos6.y);
	instance->crystalMineVector.pushBack(crystal6);
		
	//水晶矿7
	auto crystal7 = CrystalMine::create();
	crystal7->setAnchorPoint(Point(0, 0));
	map->addChild(crystal7, 5);
	crystal7->setPosition(32 * 13, 32 * 58 - 16); //这里要减去16才能对齐
	Point crystalSpriteCenterPos7 = convertTotileCoord(crystal7->getPosition());
	removeOrAddLandOccupation(10, crystalSpriteCenterPos7, 3);
	instance->crystalMineVector.pushBack(crystal7);
}

void HelloWorld::SoldierPollutionGasDamage(float dt)//士兵部署时产生的毒气攻击效果
{
	for (int i = 0; i < instance1->soldierVector.size(); i++)
	{
		auto tempSoldier = instance1->soldierVector.at(i);
		if (tempSoldier->getIsDeployed())//如果该士兵处于部署状态,才造成伤害
		{
			auto tempSoldierPos = tempSoldier->getPosition();
			for (int j = 0; j < instance1->enemyVector.size(); j++)
			{
				auto tempEnemy = instance1->enemyVector.at(j);
				auto tempEnemyPos = tempEnemy->getPosition();
				if (tempSoldierPos.getDistance(tempEnemyPos) <= tempSoldier->getPollutionRange())//如果该敌人在士兵的毒气作用范围内
				{
					if (tempEnemy->getCurrHp() >= 1) //毒气只会减血到1，不会杀死敌人
					{
						tempEnemy->setCurrHp(tempEnemy->getCurrHp() - 1);
						auto hpPercent = (float)tempEnemy->getCurrHp() / (float)tempEnemy->getMaxHp();
						tempEnemy->getHpBar()->setPercentage(hpPercent * 100);
					}
				}
			}
		}
	}
}