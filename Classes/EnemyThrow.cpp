#include "EnemyThrow.h"

USING_NS_CC;

EnemyThrow::EnemyThrow()
{
}


EnemyThrow::~EnemyThrow()
{
}

bool EnemyThrow::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	instance = GameManager::getInstance();

	sprite = Sprite::create("monster/monster_throw/right1.png"); //怪物本身的图片，初始化显
	addChild(sprite);

	iceSprite = Sprite::create("ice.png"); //冰块图片
	iceSprite->setPosition(sprite->getPosition());
	addChild(iceSprite, 2);
	iceSprite->setVisible(false);


	setType(5);//这是第5种敌人
	setIsVisible(true);//不是隐形单位
	setRunSpeed(2);
	setHpPercentage(100);
	setMoney(8);//只值4元钱
	animationRight = createAnimation("monster/monster_throw/right", 4, 0.1f, 120, 100);
	AnimationCache::getInstance()->addAnimation(animationRight, "monster_throw_right");  //这个cache里存有各种做好的动画，重要，以便后来取出用动画
	animationLeft = createAnimation("monster/monster_throw/left", 4, 0.1f, 120, 100);
	AnimationCache::getInstance()->addAnimation(animationLeft, "monster_throw_left");


	createAndSetHpBar();

	schedule(schedule_selector(EnemyThrow::changeDirection), 0.4f);
	schedule(schedule_selector(EnemyThrow::deFroze), 0.2f); //每隔0.2秒执行一次检测是否减速函数



	return true;
}
void EnemyThrow::changeDirection(float dt)
{
	auto curr = currPoint();
	if (curr->getPositionX() > this->getPosition().x)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_throw_right"))); //从cache里取出动画播放
	}
	else{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_throw_left")));
	}
}

EnemyThrow* EnemyThrow::createEnemyThrow(Vector<cocos2d::Node*> points, int hp)
{
	auto enemy = new EnemyThrow();
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

void EnemyThrow::unscheduleChangeDirection()
{
	unschedule(schedule_selector(EnemyThrow::changeDirection));
}

void EnemyThrow::deFroze(float dt)
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