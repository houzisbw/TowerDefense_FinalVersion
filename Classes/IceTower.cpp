#include "IceTower.h"
#include "IceBullet.h"
#include "EnemyTest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;
USING_NS_CC;

#define EFFECT_NUM 4
#define EXPLOSION_NUM 9
#define ICE_EFFECT_RANGE 100  //寒冰减速子弹影响范围

IceTower::IceTower()
{
}


IceTower::~IceTower()
{
}

bool IceTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}

	instance = GameManager::getInstance();

	setScope(300);
	setAttack(0);
	setRate(4);//4秒钟开火一次
	setTowerType(2);
	setTowerSize(Size(1, 3)); //占地1 * 3大小
	setIsSelected(false);
	setGrade(0);//开始没有等级
	setPower(10);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(true);//该塔耗电
	setFrozeTime(20);//冻结时间

	towerSprite = Sprite::create("towerItem/Item3.png");
	addChild(towerSprite, 5);

	gradeSprite = Sprite::create("level1.png");
	gradeSprite->setAnchorPoint(Point(0, 0));
	gradeSprite->setPosition(this->getPosition().x + 10, -towerSprite->getBoundingBox().size.height / 2);
	gradeSprite->setOpacity(0);//开始让其不可见
	addChild(gradeSprite, 6);

	noPowerSprite = Sprite::create("noPower.png");
	noPowerSprite->setAnchorPoint(Point(0.5, 0));
	noPowerSprite->setScale(1.4);
	noPowerSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 - 80, towerSprite->getBoundingBox().size.height - 70);
	noPowerSprite->setVisible(false);
	addChild(noPowerSprite, 7);

	//沉默图片
	noAttackSprite = Sprite::create("noSpeak.png");
	noAttackSprite->setScale(0.8);
	noAttackSprite->setAnchorPoint(Point(0.5, 0));
	noAttackSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 - 85, towerSprite->getBoundingBox().size.height - 90);
	addChild(noAttackSprite, 8);
	noAttackSprite->setVisible(false);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(noPowerSprite->getPosition()-Point(5,0));
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
	
	//闪电特效
	effectSprite = Sprite::create("towerItem/Item3_Effect/effect1.png");
	effectSprite->setPosition(towerSprite->getPosition()+Point(0,90));
	this->addChild(effectSprite,6);

	//初始化闪电特效
	for (int i = 1; i <= EFFECT_NUM; i++)
	{
		char buf[50];
		sprintf(buf, "towerItem/Item3_Effect/effect%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 100, 100 }); //直接从res里取出
		effectSpriteVector.pushBack(frame);
	}
	animationEffect = Animation::createWithSpriteFrames(effectSpriteVector, 0.2);
	effectSprite->runAction(RepeatForever::create(Animate::create(animationEffect))); 

	schedule(schedule_selector(IceTower::checkNearestEnemy), 0.2);//找到最近敌人
	schedule(schedule_selector(IceTower::checkPowerIsEnough), 0.2);//检测电力是否足够
	scheduleShoot(rate);//开始执行射击函数

	addTouch();//添加触摸事件 

	return true;
}

void IceTower::shoot(float dt)
{
	
	if (nearestEnemy != nullptr && nearestEnemy->getCurrHp() > 0 && isPowerEnough && nearestEnemy->getIsVisible() && !isSilenced) //电力足够才能攻击
	{
		auto currBullet = IceBullet::create();
		
		currBullet->setPosition(this->getPosition()+Point(0,70));
		instance->map->addChild(currBullet, 30);//zOrder需要调整

		auto nearestEnemyPos = nearestEnemy->getPosition();
		auto mt = MoveTo::create(1, nearestEnemyPos);
		auto deleteBullet = CallFunc::create([=](){
			currBullet->removeFromParent();
			instance->bulletVector.eraseObject(currBullet);
			//删除爆炸特效，否则会一直播放
			exp->stopAllActions();
			exp->removeFromParent();
		});
		//寒冰子弹爆炸效果
		auto explosion = CallFunc::create([=](){
			//最近敌人的位置
			auto nearestPos = nearestEnemyPos;
			//敌人被减速效果
			for (int i = 0; i < instance->enemyVector.size(); i++)
			{
				auto enemy = instance->enemyVector.at(i);
				auto enemyPos = enemy->getPosition(); //获得敌人在map上的位置
				if ((enemyPos.x - nearestPos.x)*(enemyPos.x - nearestPos.x) + (enemyPos.y - nearestPos.y)*(enemyPos.y - nearestPos.y) <= ICE_EFFECT_RANGE*ICE_EFFECT_RANGE) //如果敌人在寒冰减速范围内
				{
					
					enemy->setIsFrozen(true);//敌人被冻住
					enemy->iceSprite->setVisible(true);
					enemy->setTotalFrozeTime(frozeTime); //设置敌人的冻结时间，注意 这个是总的冻结时间，不变的值
				}

			}

			exp = CSLoader::createNode("IceBulletExplosion.csb");
			instance->map->addChild(exp, 31);
			exp->setPosition(nearestPos);

			auto timeLine = CSLoader::createTimeline("IceBulletExplosion.csb");
			timeLine->gotoFrameAndPlay(0);

			exp->runAction(timeLine);
		});
		auto delay = DelayTime::create(0.6);
		
		auto seq = Sequence::create(mt, explosion, delay,deleteBullet,NULL);
		currBullet->runAction(seq);
		

	}
}

void IceTower::checkPowerIsEnough(float dt)//检测电力是否足够
{
	if (instance->getRedPower() <= instance->getGreenPower()) //电力足够
	{
		effectSprite->setVisible(true);
	}
	else
	{
		effectSprite->setVisible(false);
	}
}

void IceTower::unscheduleShoot()
{
	this->unschedule(schedule_selector(IceTower::shoot));
}

void IceTower::scheduleShoot(float rate)
{
	this->schedule(schedule_selector(IceTower::shoot), rate);
}

