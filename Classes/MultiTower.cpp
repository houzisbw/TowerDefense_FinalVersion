#include "MultiTower.h"
#include "MultiTowerBullet.h"
USING_NS_CC;

MultiTower::MultiTower()
{
}


MultiTower::~MultiTower()
{
}

bool MultiTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}

	instance = GameManager::getInstance();

	setScope(450);
	setAttack(15);
	setRate(5);//2.5秒钟开火一次
	setTowerType(4);
	setIsSelected(false);
	setGrade(0);//开始没有等级
	setPower(40);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(true);//该塔耗电

	towerSprite = Sprite::create("towerItem/Item5.png");
	towerSprite->setScale(1);
	addChild(towerSprite, 5);

	gradeSprite = Sprite::create("level1.png");
	gradeSprite->setAnchorPoint(Point(0, 0));
	gradeSprite->setPosition(this->getPosition().x + 10, -towerSprite->getBoundingBox().size.height / 2);
	gradeSprite->setOpacity(0);//开始让其不可见
	addChild(gradeSprite, 6);

	noPowerSprite = Sprite::create("noPower.png");
	noPowerSprite->setAnchorPoint(Point(0.5, 0));
	noPowerSprite->setScale(1);
	noPowerSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 - 50, towerSprite->getBoundingBox().size.height - 30);
	noPowerSprite->setVisible(false);
	addChild(noPowerSprite, 7);

	//沉默图片
	noAttackSprite = Sprite::create("noSpeak.png");
	noAttackSprite->setScale(0.7);
	noAttackSprite->setAnchorPoint(Point(0.5, 0));
	noAttackSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 - 58, towerSprite->getBoundingBox().size.height - 60);
	addChild(noAttackSprite, 8);
	noAttackSprite->setVisible(false);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(noPowerSprite->getPosition()-Point(10,20));
	towerHpSprite->setScale(0.6);
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


	schedule(schedule_selector(MultiTower::checkNearestEnemy), 0.2);
	schedule(schedule_selector(MultiTower::shoot), rate);

	addTouch();//添加触摸事件

	return true;
}

void MultiTower::shoot(float dt) //真正发射子弹
{
	
	if (nearestEnemy != nullptr && getMove() == false && isPowerEnough && nearestEnemy->getCurrHp() > 0 && nearestEnemy->getIsVisible() && !isSilenced) //当该塔不在移动状态时才能攻击
	{	
		int dirTotal = 10;	
		for (int i = 0; i < dirTotal; i++)
		{
			auto multiBullet = MultiTowerBullet::create();
			instance->bulletVector.pushBack(multiBullet->getBulletSprite()); //这里注意 ： 子弹vector是sprite类型的，不是bullet类型的
			instance->map->addChild(multiBullet, 40);
			multiBullet->setPosition(this->getPosition() + Point(0, 40));
			multiBullet->getBulletSprite()->setTag(attack);
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
			float farthestDistance = 450;
			Point overshotVector = normalizedShootVector * farthestDistance;
			Point offscreenPoint = (multiBullet->getPosition() - overshotVector);

			multiBullet->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint),
				CallFunc::create([=](){
				instance->bulletVector.eraseObject(multiBullet);
				multiBullet->removeFromParent();
			}),NULL));
		}
		
		

	}
}

