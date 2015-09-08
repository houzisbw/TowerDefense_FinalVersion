#include "EnemyBase.h"
#include "GameManager.h"
USING_NS_CC;

EnemyBase::EnemyBase():
pointCounter(0),
runSpeed(0),
isFrozen(false),
frozeTime(20), //冰冻时间
totalFrozeTime(20),
isStopped(false),
enemySuccessful(false)
{
	//设置schduler来减缓敌人速度
	CCScheduler *defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	// Create a new scheduler, and link it to the main scheduler
	sched1 = new CCScheduler();
	defaultScheduler->scheduleUpdateForTarget(sched1, 0, false);
	// Create a new ActionManager, and link it to the new scheudler
	actionManager1 = new CCActionManager();
	sched1->scheduleUpdateForTarget(actionManager1, 0, false);
	// Replace the default ActionManager with the new one.
	this->setActionManager(actionManager1);

	
}


EnemyBase::~EnemyBase()
{
}

void EnemyBase::setPointsVector(Vector<Node*> points)
{
	/*for (int i = 0; i < points.size(); i++)
	{
		auto node = points.at(i);
		node->setPosition(node->getPosition() + Point(0, 40));
	}*/
	pointsVector = points;

}

Node* EnemyBase::currPoint()
{
	return this->pointsVector.at(pointCounter);
}
Node* EnemyBase::nextPoint()
{
	int maxCount = this->pointsVector.size();
	pointCounter++;
	if (pointCounter < maxCount)
	{
		auto node = this->pointsVector.at(pointCounter);
		return node;
	}
	else
	{
		pointCounter = maxCount - 1;
		return nullptr;
	}
}
void EnemyBase::runFollowPoint()
{
	auto point = currPoint();
	auto prevP = point;
	setPosition(point->getPosition());
	point = nextPoint();
	auto nextP = point;

	if (point != nullptr)
	{

		float distancePrevToNext;
		//计算2个点的距离差，保证匀速运动
		if (abs(prevP->getPosition().x - nextP->getPosition().x) <= abs(prevP->getPosition().y - nextP->getPosition().y))
		{
			distancePrevToNext = abs(prevP->getPosition().y - nextP->getPosition().y);
		}
		else
		{
			distancePrevToNext = abs(prevP->getPosition().x - nextP->getPosition().x);
		}

		float timeToMove = distancePrevToNext / (getRunSpeed()*20.0);

		this->runAction(Sequence::create(MoveTo::create(timeToMove, point->getPosition())
			, CallFuncN::create(CC_CALLBACK_0(EnemyBase::runFollowPoint, this))
			, NULL));
	}
	else //敌人到达目的地
	{
		setEnemySuccessful(true);

	}
}

Animation* EnemyBase::createAnimation(std::string prefixName, int framesNum, float delay,int width,int height)
{
	Vector<SpriteFrame*> animFrams; //每一帧动作图片的vector
	for (int i = 1; i <= framesNum; i++)
	{
		char buf[10];
		sprintf(buf, "%d.png", i);  
		std::string str = prefixName + buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, (float)width, (float)height });
		animFrams.pushBack(frame);
	}

	return Animation::createWithSpriteFrames(animFrams, delay);
}

void EnemyBase::changeDirection(float dt) //子类要重写
{
	
}

void EnemyBase::createAndSetHpBar()
{
	//血条背景
	hpBgSprite = Sprite::create("monsterBloodBg.png");
	hpBgSprite->setScaleX(0.7);
	//log("%f,%f", this->getContentSize().width, this->getContentSize().height); //此时this的大小为0，因为还没有创建实体
	hpBgSprite->setPosition(Point(/*sprite->getBoundingBox().size.width / 2*/0, sprite->getBoundingBox().size.height-30)); //sprite在子类里被初始化
	addChild(hpBgSprite,4);

	//血条
	hpBar = ProgressTimer::create(Sprite::create("monsterBloodBar.png")); //参数是一个图片sprite
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Point(0, 0.5f));
	hpBar->setBarChangeRate(Point(1, 0));
	hpBar->setPercentage(hpPercentage);
	hpBar->setPosition(Point(hpBgSprite->getContentSize().width / 2, hpBgSprite->getContentSize().height / 3 * 2-4));
	hpBgSprite->addChild(hpBar,5);


}

void EnemyBase::unscheduleChangeDirection()
{
	unschedule(schedule_selector(EnemyBase::changeDirection));
}

void EnemyBase::deFroze(float dt)
{

}

void EnemyBase::enemyExplosionAnimation()
{
	instance = GameManager::getInstance();

	unschedule(schedule_selector(EnemyBase::changeDirection));
	this->stopAllActions();
	hpBar->setVisible(false);
	hpBgSprite->setVisible(false);

	//金钱label显示：从下往上显示
	auto moneySprite = Sprite::create("towerCostImage.png");
	moneySprite->setScale(0.6);
	moneySprite->setPosition(hpBgSprite->getPosition() + Point(0, 10));
	addChild(moneySprite, 10);
	auto mt = MoveTo::create(0.6, hpBgSprite->getPosition() + Point(0, 60));
	auto deleteMoney = CallFunc::create([=](){
		moneySprite->removeFromParent();
	});
	auto seq = Sequence::create(mt, deleteMoney, NULL);
	moneySprite->runAction(seq);

	char tempChar[10];
	sprintf(tempChar, "%d", money);
	std::string str = tempChar;
	auto moneyLabel = Label::createWithTTF(str, "HANGTHEDJ.ttf", 35);
	moneyLabel->setColor(Color3B(255, 255, 255));
	moneyLabel->setPosition(hpBgSprite->getPosition() + Point(50, 10));
	addChild(moneyLabel, 10);
	auto mt1 = MoveTo::create(0.6, moneyLabel->getPosition() + Point(0, 50));
	auto deleteLabel = CallFunc::create([=](){
		moneyLabel->removeFromParent();
	});
	auto seq1 = Sequence::create(mt1, deleteLabel, NULL);
	moneyLabel->runAction(seq1);

	//玩家金钱数增加
	char m[20];
	sprintf(m, "%d", instance->getMoney()+money);
	std::string mStr = m;
	instance->moneyLabel->setString(mStr);
	instance->setMoney(instance->getMoney() + money);

	auto st = ScaleTo::create(0.6, 1.5);
	auto fo = FadeOut::create(0.6);
	auto sp = Spawn::createWithTwoActions(st, fo);//如果用spawn::create()就不行,奇了怪了
	auto deleteThis = CallFunc::create([=](){
		this->removeFromParent();
	});
	sprite->runAction(Sequence::create(sp, deleteThis, NULL));
}