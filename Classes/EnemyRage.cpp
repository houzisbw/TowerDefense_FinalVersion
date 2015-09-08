#include "EnemyRage.h"
USING_NS_CC;

EnemyRage::EnemyRage():
rage(0)
{
}


EnemyRage::~EnemyRage()
{
}

bool EnemyRage::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	instance = GameManager::getInstance();

	sprite = Sprite::create("monster/monster_rage/right1.png"); //怪物本身的图片，初始化显
	addChild(sprite);

	iceSprite = Sprite::create("ice.png"); //冰块图片
	iceSprite->setPosition(sprite->getPosition());
	addChild(iceSprite, 2);
	iceSprite->setVisible(false);

	rageIcon = Sprite::create("rageIcon.png");
	rageIcon->setScale(0.9);
	rageIcon->setPosition(sprite->getPosition() + Point(0, 150));
	rageIcon->setVisible(false);
	addChild(rageIcon, 10);

	setType(4);//这是第4种敌人
	setIsVisible(true);//不是隐形单位
	setRunSpeed(2);
	setHpPercentage(100);
	setMoney(8);//只值4元钱
	animationRight = createAnimation("monster/monster_rage/right", 4, 0.1f, 100, 100);
	AnimationCache::getInstance()->addAnimation(animationRight, "monster_rage_right");  //这个cache里存有各种做好的动画，重要，以便后来取出用动画
	animationLeft = createAnimation("monster/monster_rage/left", 4, 0.1f, 100, 100);
	AnimationCache::getInstance()->addAnimation(animationLeft, "monster_rage_left");


	createAndSetHpBar();
	createRageBar();

	schedule(schedule_selector(EnemyRage::changeDirection), 0.4f);
	schedule(schedule_selector(EnemyRage::deFroze), 0.2f); //每隔0.2秒执行一次检测是否减速函数
	schedule(schedule_selector(EnemyRage::rageDecrease), 0.2f);//怒气衰减函数
	


	return true;
}

void EnemyRage::changeDirection(float dt)
{
	auto curr = currPoint();
	if (curr->getPositionX() > this->getPosition().x)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_rage_right"))); //从cache里取出动画播放
	}
	else{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_rage_left")));
	}
}

void EnemyRage::unscheduleChangeDirection()
{
	unschedule(schedule_selector(EnemyRage::changeDirection));
}

//敌人冻住之后的解冻计时函数
void EnemyRage::deFroze(float dt)
{
	if (isFrozen && frozeTime == totalFrozeTime && !isStopped) //被冻住 且没有被封印才行
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

//创建狂怒条
void  EnemyRage::createRageBar()
{
	//怒气条背景
	rageBgSprite = Sprite::create("monsterBloodBg.png");
	rageBgSprite->setScaleX(0.7);
	//log("%f,%f", this->getContentSize().width, this->getContentSize().height); //此时this的大小为0，因为还没有创建实体
	rageBgSprite->setPosition(Point(/*sprite->getBoundingBox().size.width / 2*/0, sprite->getBoundingBox().size.height-10)); //sprite在子类里被初始化
	addChild(rageBgSprite, 4);

	//怒气条，开始百分比为0
	rageBar = ProgressTimer::create(Sprite::create("monster_rage_bar.png")); //参数是一个图片sprite
	rageBar->setType(ProgressTimer::Type::BAR);
	rageBar->setMidpoint(Point(0, 0.5f));
	rageBar->setBarChangeRate(Point(1, 0));
	rageBar->setPercentage(0);
	rageBar->setPosition(Point(rageBgSprite->getContentSize().width / 2, rageBgSprite->getContentSize().height / 3 * 2 - 4));
	rageBgSprite->addChild(rageBar, 5);
}

EnemyRage* EnemyRage::createEnemyRage(Vector<cocos2d::Node*> points, int hp)
{
	auto enemy = new EnemyRage();
	if (enemy && enemy->init())
	{
		enemy->setPointsVector(points);
		enemy->runFollowPoint();  //这里就开始执行行走了
		enemy->autorelease();
		enemy->setMaxHp(hp);
		enemy->setCurrHp(hp);
		return enemy;
	}
	else
	{
		delete enemy;
		enemy = nullptr;
		return nullptr;
	}
}

//该敌人加速前进一段时间
void EnemyRage::runFastInPeroid()
{
	this->sched1->setTimeScale(5);
}
//敌人怒气值衰减函数
void EnemyRage::rageDecrease(float dt)
{	
	auto tempRage = rage - 3;
	if (tempRage <= 0 && !isFrozen && !isStopped) //必须不是在时间结界冻结状态下才行
	{
		tempRage = 0;
		this->sched1->setTimeScale(1);//恢复正常速度
		rageIcon->setVisible(false);
	}
	this->setRage(tempRage);
	this->rageBar->setPercentage(rage);
	
}
