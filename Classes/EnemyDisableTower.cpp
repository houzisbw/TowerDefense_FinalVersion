#include "EnemyDisableTower.h"
USING_NS_CC;

#define DISABLE_RANGE 300

EnemyDisableTower::EnemyDisableTower()
{
}


EnemyDisableTower::~EnemyDisableTower()
{
}

bool EnemyDisableTower::init() /*override*/
{
	if (!Sprite::init())
	{
		return false;
	}
	instance = GameManager::getInstance();

	sprite = Sprite::create("monster/monster_disable_tower/right1.png"); //怪物本身的图片，初始化显
	addChild(sprite);

	iceSprite = Sprite::create("ice.png"); //冰块图片
	iceSprite->setPosition(sprite->getPosition());
	addChild(iceSprite, 2);
	iceSprite->setVisible(false);

	disableRing = Sprite::create("disableRing.png");
	disableRing->setScale(2);
	addChild(disableRing, 10);
	auto fo = FadeOut::create(1);
	disableRing->runAction(RepeatForever::create(Sequence::create(fo, fo->reverse(), NULL)));

	setType(3);//这是第3种敌人
	setIsVisible(true);//不是隐形单位
	setRunSpeed(4);
	setHpPercentage(100);
	setMoney(8);//只值4元钱
	animationRight = createAnimation("monster/monster_disable_tower/right", 4, 0.1f, 100, 100);
	AnimationCache::getInstance()->addAnimation(animationRight, "monster_disable_right");  //这个cache里存有各种做好的动画，重要，以便后来取出用动画
	animationLeft = createAnimation("monster/monster_disable_tower/left", 4, 0.1f, 100, 100);
	AnimationCache::getInstance()->addAnimation(animationLeft, "monster_disable_left");


	createAndSetHpBar();

	schedule(schedule_selector(EnemyDisableTower::changeDirection), 0.4f);
	schedule(schedule_selector(EnemyDisableTower::deFroze), 0.2f); //每隔0.2秒执行一次检测是否减速函数
	//schedule(schedule_selector(EnemyDisableTower::disableNearbyTower), 0.2f);//每隔0.2秒沉默敌人一次,有问题:不能在敌人这判断，要在炮塔那判断才行,否则会图片会闪烁


	return true;
}

void EnemyDisableTower::changeDirection(float dt)
{
	auto curr = currPoint();
	if (curr->getPositionX() > this->getPosition().x)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_disable_right"))); //从cache里取出动画播放
	}
	else{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_disable_left")));
	}
}

void  EnemyDisableTower::unscheduleChangeDirection()
{
	unschedule(schedule_selector(EnemyDisableTower::changeDirection));
}

EnemyDisableTower* EnemyDisableTower::createEnemyDisableTower(Vector<Node*> points, int hp)
{
	auto dis = new EnemyDisableTower();
	if (dis && dis->init())
	{
		dis->setPointsVector(points);
		dis->runFollowPoint();  //这里就开始执行行走了
		dis->autorelease();
		dis->setMaxHp(hp);
		dis->setCurrHp(hp);
		return dis;
	}
	else
	{
		delete dis;
		dis = nullptr;
		return nullptr;
	}
}

//敌人冻住之后的解冻计时函数
void EnemyDisableTower::deFroze(float dt)
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
//沉默附近炮塔:此函数不能在这用
void EnemyDisableTower::disableNearbyTower(float dt)
{
	//for (int i = 0; i < instance->towerVector.size(); i++)
	//{
	//	int type = instance->towerVector.at(i)->getTowerType();
	//	if (type <= 4 && type!=3) //只有炮塔能被沉默，且不能沉默反隐塔
	//	{

	//		auto tempTower = instance->towerVector.at(i);
	//		auto pos = tempTower->getPosition();

	//		tempTower->noAttackSprite->setVisible(false);
	//		//如果敌人在自己的范围内
	//		if ((pos.x - this->getPosition().x)*(pos.x - this->getPosition().x) + (pos.y - this->getPosition().y)*(pos.y - this->getPosition().y) <= DISABLE_RANGE*DISABLE_RANGE)
	//		{
	//			tempTower->noAttackSprite->setVisible(true);
	//		}
	//	}
	//}
}