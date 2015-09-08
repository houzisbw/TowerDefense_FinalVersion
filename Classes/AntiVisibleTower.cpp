#include "AntiVisibleTower.h"
#include "GameManager.h"
USING_NS_CC;

AntiVisibleTower::AntiVisibleTower()
{
}


AntiVisibleTower::~AntiVisibleTower()
{
}

bool AntiVisibleTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}
	
	instance = GameManager::getInstance();   

	setScope(300);
	setAttack(0);
	setRate(5);//4秒钟开火一次
	setTowerType(3);
	setIsSelected(false);
	setGrade(0);//开始没有等级
	setPower(40);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(true);//该塔耗电
	nearestEnemy = nullptr;

	towerSprite = Sprite::create("towerItem/Item99.png");
	addChild(towerSprite, 5);

	gradeSprite = Sprite::create("level1.png");
	gradeSprite->setAnchorPoint(Point(0, 0));
	gradeSprite->setPosition(this->getPosition().x + 10, -towerSprite->getBoundingBox().size.height / 2);
	gradeSprite->setOpacity(0);//开始让其不可见
	addChild(gradeSprite, 6);

	noPowerSprite = Sprite::create("noPower.png");
	noPowerSprite->setAnchorPoint(Point(0.5, 0));
	noPowerSprite->setScale(1.4);
	noPowerSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 - 70, towerSprite->getBoundingBox().size.height - 40);
	noPowerSprite->setVisible(false);
	addChild(noPowerSprite, 7);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(noPowerSprite->getPosition()-Point(0,25));
	towerHpSprite->setScale(0.8);
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

	schedule(schedule_selector(AntiVisibleTower::shoot), 0.1f);
	schedule(schedule_selector(AntiVisibleTower::checkNearestEnemy), 0.2);
	addTouch();//添加触摸事件 

	return true;
}

void AntiVisibleTower::shoot(float dt)
{
	//电力足够的话 且没有移动
	if (getMove() == false && isPowerEnough)
	{
		//检测隐形敌人
		for (int i = 0; i < instance->enemyVector.size(); i++)
		{
			auto enemy = instance->enemyVector.at(i);
			auto enemyPos = enemy->getPosition();
			auto enemyType = enemy->getType();
			//只对第二种敌人(隐形敌人有效),否则所有敌人都要隐形
			if (enemyType == 2)
			{

				if (isEnemyInAnyAntiTowerRange(enemy)) //如果敌人在反隐塔vector中任意一个的射程内的话就显形，否则隐形
				{

					enemy->getEnemySprite()->setVisible(true); //图像上显形
					enemy->setIsVisible(true);//功能上显形
				}
				else
				{
					enemy->getEnemySprite()->setVisible(false);//隐形
					enemy->setIsVisible(false);

				}
			}
		}
	}
	
}

bool AntiVisibleTower::isEnemyInAnyAntiTowerRange(EnemyBase* enemy)//敌人是否处于任何一个反隐塔的范围内,做这个判断是因为如果不做的话，只有一个塔能反隐，其他的都不行
{
	for (int i = 0; i < instance->antiVisibleTowerVector.size(); i++)
	{
		auto towerPos = instance->antiVisibleTowerVector.at(i)->getPosition();
		if ((towerPos.x - enemy->getPosition().x)*(towerPos.x - enemy->getPosition().x) + (towerPos.y - enemy->getPosition().y)*(towerPos.y - enemy->getPosition().y) <= scope*scope)
		{
			return true;
		}
	}
	return false;
}
