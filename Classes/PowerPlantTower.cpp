#include "PowerPlantTower.h"
USING_NS_CC;

PowerPlantTower::PowerPlantTower()
{
}


PowerPlantTower::~PowerPlantTower()
{
}

bool PowerPlantTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}

	
	setTowerType(5);
	setIsSelected(false);
	setGrade(0);//开始没有等级
	setPower(0);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(false);//该塔不耗电
	setTowerSize(Size(3, 3));//占地面积3*3

	towerSprite = Sprite::create("towerItem/Item6.png");
	towerSprite->setScale(0.4);
	addChild(towerSprite, 5);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(towerSprite->getPosition()+Point(0,50));
	towerHpSprite->setScale(0.3);
	addChild(towerHpSprite, 10);
	//炮塔血量
	hp = 4;
	//炮塔血量进度条
	towerHp = ProgressTimer::create(Sprite::create("soldierProduceTimeBar.png")); //参数是一个图片sprite
	towerHp->setScaleX(2);
	towerHp->setScaleY(5.2);
	towerHp->setType(ProgressTimer::Type::BAR);
	towerHp->setMidpoint(Point(0, 0.5f));
	towerHp->setBarChangeRate(Point(1, 0));
	towerHp->setPercentage(100);
	towerHp->setPosition(Point(towerHpSprite->getContentSize().width / 2, towerHpSprite->getContentSize().height / 3 * 2 - 10));
	towerHpSprite->addChild(towerHp, 5);
	//初始化不可见
	towerHp->setVisible(false);
	towerHpSprite->setVisible(false);

	addTouch();//添加触摸事件

	return true;
}

void PowerPlantTower::shoot(float dt)
{
	// do nothing
}
