#include "EnemyTest.h"
USING_NS_CC;

EnemyTest::EnemyTest()
{
}


EnemyTest::~EnemyTest()
{
}

bool EnemyTest::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	instance = GameManager::getInstance();

	sprite = Sprite::create("monster/monster1/right1.png"); //怪物本身的图片，初始化显示
	addChild(sprite);
	
	iceSprite = Sprite::create("ice.png"); //冰块图片
	iceSprite->setPosition(sprite->getPosition());
	addChild(iceSprite, 2);
	iceSprite->setVisible(false);

	setType(1);//这是第一种敌人
	setIsVisible(true);
	setRunSpeed(3);
	setHpPercentage(100);
	setMoney(2);//只值2元钱
	animationRight = createAnimation("monster/monster1/right", 4, 0.1f,95,84);
	AnimationCache::getInstance()->addAnimation(animationRight, "monster1_right");  //这个cache里存有各种做好的动画，重要，以便后来取出用动画
	animationLeft = createAnimation("monster/monster1/left", 4, 0.1f,95,84);
	AnimationCache::getInstance()->addAnimation(animationLeft, "monster1_left");
	

	createAndSetHpBar();

	schedule(schedule_selector(EnemyTest::changeDirection), 0.4f);
	schedule(schedule_selector(EnemyTest::deFroze), 0.2f); //每隔0.2秒执行一次检测是否减速函数

	////设置schduler来减缓敌人速度
	//CCScheduler *defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	//// Create a new scheduler, and link it to the main scheduler
	//sched1 = new CCScheduler();
	//defaultScheduler->scheduleUpdateForTarget(sched1, 0, false);
	//// Create a new ActionManager, and link it to the new scheudler
	//actionManager1 = new CCActionManager();
	//sched1->scheduleUpdateForTarget(actionManager1, 0, false);
	//// Replace the default ActionManager with the new one.
	//this->setActionManager(actionManager1);

	return true;
}

EnemyTest* EnemyTest::createEnemyTest(Vector<Node*> points, int hp)
{
	auto test = new EnemyTest();
	if (test && test->init())
	{
		test->setPointsVector(points);
		test->runFollowPoint();  //这里就开始执行行走了
		test->autorelease();
		test->setMaxHp(hp);
		test->setCurrHp(hp);
		return test;
	}
	else
	{
		delete test;
		test = nullptr;
		return nullptr;
	}

}

void  EnemyTest::changeDirection(float dt)
{
	auto curr = currPoint();
	if (curr->getPositionX() > this->getPosition().x)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster1_right"))); //从cache里取出动画播放
	}
	else{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster1_left")));
	}
}


void EnemyTest::decreaseHp(float dt)//测试自残函数
{
	this->currHp -= 1;
	if (currHp <= 0)
	{
		this->unschedule(schedule_selector(EnemyTest::decreaseHp));
		instance->enemyVector.eraseObject(this);	
		instance->map->removeChild(this, true);
	}
}

void EnemyTest::unscheduleChangeDirection()
{
	unschedule(schedule_selector(EnemyTest::changeDirection));
}

//检测敌人是否冻住的函数,每隔一秒检测一次
void EnemyTest::deFroze(float dt)
{
	if (isFrozen && frozeTime==totalFrozeTime && !isStopped) //被冻住 且没有被封印才行
	{
		frozeTime = totalFrozeTime;
		sched1->setTimeScale(0.2); //减缓敌人速度为原来1/5,0的话为静止不动
		
	}
	if (frozeTime>0 && isFrozen && !isStopped)//开始解冻啦
	{
		frozeTime--;
	}

	if (frozeTime == 0 && !isStopped)
	{
		isFrozen = false;
		iceSprite->setVisible(false);
		frozeTime = totalFrozeTime;
		sched1->setTimeScale(1); //恢复速度
	}
}
