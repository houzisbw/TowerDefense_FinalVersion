#include "PrismTower.h"
#include "EnemyRage.h"
#include "GameManager.h"
#include "EnemyThrow_son.h"
USING_NS_CC;

PrismTower::PrismTower():
nearbyTower(0)
{
}


PrismTower::~PrismTower()
{
}

bool PrismTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}
	
	

	setScope(300);
	setAttack(5);
	setRate(2.5);//2.5秒钟开火一次
	setTowerType(0);
	setTowerSize(Size(1, 3)); //占地1 * 3大小
	setIsSelected(false);
	setGrade(0);//开始没有等级
	setPower(5);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(true);//该塔耗电

	towerSprite = Sprite::create("towerItem/Item1.png");
	towerSprite->setScale(0.2);
	addChild(towerSprite,5);

	gradeSprite = Sprite::create("level1.png");
	gradeSprite->setAnchorPoint(Point(0, 0));
	gradeSprite->setPosition(this->getPosition().x+10, -towerSprite->getBoundingBox().size.height/2);
	gradeSprite->setOpacity(0);//开始让其不可见
	addChild(gradeSprite, 6);

	noPowerSprite = Sprite::create("noPower.png");
	noPowerSprite->setAnchorPoint(Point(0.5, 0));
	noPowerSprite->setScale(0.8);
	noPowerSprite->setPosition(towerSprite->getBoundingBox().size.width / 2-15, towerSprite->getBoundingBox().size.height-40);
	noPowerSprite->setVisible(false);
	addChild(noPowerSprite, 7);

	//沉默图片
	noAttackSprite = Sprite::create("noSpeak.png");
	noAttackSprite->setScale(0.5);
	noAttackSprite->setAnchorPoint(Point(0.5, 0));
	noAttackSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 -19, towerSprite->getBoundingBox().size.height - 40);
	addChild(noAttackSprite, 8);
	noAttackSprite->setVisible(false);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(noPowerSprite->getPosition() - Point(5, -10));
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

	
	schedule(schedule_selector(PrismTower::checkNearestEnemy),0.2);
	//schedule(schedule_selector(ArtilleryTower::rotateAndShoot), 0.5f); //别忘了调用函数，时间间隔越小，炮塔旋转越平滑
	schedule(schedule_selector(PrismTower::shoot), rate);

	addTouch();//添加触摸事件

	return true;
}

void PrismTower::shoot(float dt)
{

	auto instance = GameManager::getInstance();
	auto bulletVector = instance->bulletVector;
	if (nearestEnemy != nullptr && getMove() == false && isPowerEnough && nearestEnemy->getCurrHp() > 0 && nearestEnemy->getIsVisible() && !isSilenced) //当该塔不在移动状态时才能攻击
	{
		
		towerEnergyGathering();
		
		auto PosPrismTower = this->getPosition()+Point(0,40);
		auto PosEnemy = nearestEnemy->getPosition();
		//log(" worldPosPrismTower: %f %f", PosPrismTower.x, PosPrismTower.y);   

		//通过贴图来达到激光的效果
		auto laser = Sprite::create("laser.png");
		Point enemyAndTowerVector = PosEnemy - PosPrismTower;                          //得到敌人与本炮塔之间的向量
		float rotateRadians = enemyAndTowerVector.getAngle();                          //得到该向量与x轴的夹角 弧度制
		float rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

		float length = sqrt((PosEnemy.x - PosPrismTower.x)*(PosEnemy.x - PosPrismTower.x) + (PosEnemy.y - PosPrismTower.y+30)*(PosEnemy.y - PosPrismTower.y+30));  //改变30达到长度效果
		float scale = length / (float)laser->getContentSize().width;
		//log("sclae : %f", scale);
		laser->setScaleX(scale);
		if (nearbyTower >= 5)
		{
			nearbyTower = 5; //设置最大值，防止光束无限变粗,最多5个塔参与聚能
		}
		if (nearbyTower != 0)
		{
			laser->setScaleY(laser->getScaleY()+0.2*nearbyTower);
			//log("nearby %d", nearbyTower);
		}

		laser->setRotation(rotateDegrees + 180);          
		laser->setAnchorPoint(Point(1, 0.5));
		laser->setPosition(0, 40);
		addChild(laser, 4);
		laser->setVisible(false);
		laser->setOpacity(200);

		auto action1 = CallFunc::create([=](){

			laser->setVisible(true);
		});

		auto action2 = CallFunc::create([=](){

			laser->removeFromParent();
			
		});

		auto fadeOut = FadeOut::create(0.3);

		
		ActionInterval *delayTime = DelayTime::create(0.3);
		

		laser->runAction(Sequence::create(action1,delayTime,fadeOut,action2,NULL));

		//如果是狂怒敌人的话，增加怒气
		int enemyType = nearestEnemy->getType();
		if (enemyType == 4)
		{
			increaseRageIfHitEnemyWithRage(nearestEnemy);
		}

		//敌人掉血
		auto currHp = nearestEnemy->getCurrHp();
		//将tag作为攻击力减去
		currHp -= getTotalDamage(nearbyTower);
		nearestEnemy->setCurrHp(currHp);  //别忘了设置hp，否则下次检查hp不会变化
		auto hpPercent = (float)currHp / (float)nearestEnemy->getMaxHp();
		nearestEnemy->getHpBar()->setPercentage(hpPercent * 100);

		if (currHp <= 0)
		{
			//如果敌人是投掷型敌人,则要投掷出蘑菇攻击玩家防御塔
			if (enemyType == 5)
			{
				EnemyThrowMushroomToTower(nearestEnemy);
			}

			instance->enemyVector.eraseObject(nearestEnemy); //不能用enemyVector，必须用instance的，因为只是一个复制过程	
			//小地图处理 万万不能忘记，这个bug找了好几天
			auto tempSmallRed = instance->smallMap->map->getChildByTag(nearestEnemy->getTag());
			instance->smallMap->redVector.eraseObject(static_cast<Sprite*>(tempSmallRed));
			tempSmallRed->removeFromParent();

			nearestEnemy->enemyExplosionAnimation();//爆炸动画顺带移除敌人

		}

		nearbyTower = 0;
	
	}
}

void PrismTower::towerEnergyGathering() //光棱塔聚能效果
{
	//找到自己范围内所有的光棱塔
	GameManager * instance = GameManager::getInstance();
	for (int i = 0; i < instance->prismTowerVector.size(); i++)
	{
		auto tempTower = instance->prismTowerVector.at(i);
		auto towerPos = tempTower->getPosition();

		//如果其他塔在自己的射程内并且其他塔没有最近敌人（目标为空）,才能作为聚能塔，否则各自攻击各自目标
		if ((this->getPosition().x - towerPos.x)*(this->getPosition().x - towerPos.x) + (this->getPosition().y - towerPos.y)*(this->getPosition().y - towerPos.y) <= scope * scope && tempTower->getNearestEnemy()==nullptr)
		{
			nearbyTower++;
			//在2个塔之间画激光
			auto laser = Sprite::create("laser.png");
			Point towerAndTowerVector = towerPos - this->getPosition();                    
			float rotateRadians = towerAndTowerVector.getAngle();                          //得到该向量与x轴的夹角 弧度制
			float rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

			auto thisTowerPos = this->getPosition() + Point(0, 40);

			float length = sqrt((towerPos.x - thisTowerPos.x)*(towerPos.x - thisTowerPos.x) + (towerPos.y - thisTowerPos.y + 40)*(towerPos.y - thisTowerPos.y + 40));  //改变30达到长度效果
			float scale = length / (float)laser->getContentSize().width;
			log("sclae : %f", scale);
			laser->setScaleX(scale);

			laser->setRotation(rotateDegrees + 180);           //log("enemy anchor : %f %f", nearestEnemy->getAnchorPoint().x, nearestEnemy->getAnchorPoint().y);
			laser->setAnchorPoint(Point(1, 0.5));
			laser->setPosition(0, 40);
			laser->setOpacity(200);
			addChild(laser, 4);

			laser->setVisible(false);

			auto action1 = CallFunc::create([=](){

				laser->setVisible(true);
			});

			auto action2 = CallFunc::create([=](){

				laser->removeFromParent();

			});

			auto fadeOut = FadeOut::create(0.3);


			ActionInterval *delayTime = DelayTime::create(0.3);


			laser->runAction(Sequence::create(action1, delayTime, fadeOut, action2, NULL));
		}
	}
}

int PrismTower::getTotalDamage(int towerNum)
{
	int tempAttack = attack;
	for (int i = 1; i <= towerNum; i++)
	{
		tempAttack *= 2;
	}
	return tempAttack;

}

void PrismTower::increaseRageIfHitEnemyWithRage(EnemyBase* enemy)//如果子弹击中怒气型敌人，则敌人增加怒气
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
void PrismTower::EnemyThrowMushroomToTower(EnemyBase* enemy)//投掷型敌人投掷出蘑菇来攻击玩家防御塔
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
		auto enemyMushroom = EnemyThrow_son::createEnemyThrow_son(vec, 10, timeToDelay + i*0.1);//10能量值,所有蘑菇不是同一时间开始训寻路
		enemyMushroom->setPosition(pos);
		instance->map->addChild(enemyMushroom, 30);

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