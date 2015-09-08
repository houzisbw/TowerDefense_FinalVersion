#include "EnemyInvisible.h"
USING_NS_CC;

EnemyInvisible::EnemyInvisible()
{
}


EnemyInvisible::~EnemyInvisible()
{
}

bool EnemyInvisible::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	instance = GameManager::getInstance();

	sprite = Sprite::create("monster/monster_invisible/right1.png"); //怪物本身的图片，初始化显示
	sprite->setVisible(false);
	addChild(sprite);

	iceSprite = Sprite::create("ice.png"); //冰块图片
	iceSprite->setPosition(sprite->getPosition());
	addChild(iceSprite, 2);
	iceSprite->setVisible(false);

	setType(2);//这是第2种敌人
	setIsVisible(false);//是隐形单位
	setRunSpeed(3);
	setHpPercentage(100);
	setMoney(4);//只值4元钱
	animationRight = createAnimation("monster/monster_invisible/right", 4, 0.1f, 100, 100);
	AnimationCache::getInstance()->addAnimation(animationRight, "monster_invisible_right");  //这个cache里存有各种做好的动画，重要，以便后来取出用动画
	animationLeft = createAnimation("monster/monster_invisible/left", 4, 0.1f,100, 100);
	AnimationCache::getInstance()->addAnimation(animationLeft, "monster_invisible_left");


	createAndSetHpBar();

	schedule(schedule_selector(EnemyInvisible::changeDirection), 0.4f);
	schedule(schedule_selector(EnemyInvisible::deFroze), 0.2f); //每隔0.2秒执行一次检测是否减速函数


	return true;
}

void  EnemyInvisible::changeDirection(float dt)
{
	auto curr = currPoint();
	if (curr->getPositionX() > this->getPosition().x)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_invisible_right"))); //从cache里取出动画播放
	}
	else{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_invisible_left")));
	}
}

//敌人冻住之后的解冻计时函数
void EnemyInvisible::deFroze(float dt)
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

void  EnemyInvisible::unscheduleChangeDirection()
{
	unschedule(schedule_selector(EnemyInvisible::changeDirection));
}

EnemyInvisible* EnemyInvisible::createEnemyInvisible(Vector<Node*> points, int hp)
{
	auto invisible = new EnemyInvisible();
	if (invisible && invisible->init())
	{
		invisible->setPointsVector(points);
		invisible->runFollowPoint();  //这里就开始执行行走了
		invisible->autorelease();
		invisible->setMaxHp(hp);
		invisible->setCurrHp(hp);
		return invisible;
	}
	else
	{
		delete invisible; 
		invisible = nullptr;
		return nullptr;
	}
}
