#include "ArtilleryTower.h"
#include "GameManager.h"
USING_NS_CC;

ArtilleryTower::ArtilleryTower()
{
}


ArtilleryTower::~ArtilleryTower()
{
}

bool ArtilleryTower::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}
	
	setScope(300);
	setAttack(10);
	setRate(2);
	setIsSelected(false);
	setTowerType(1);
	setGrade(0);//开始没有等级
	setPower(0);//塔消耗电力
	setMove(false);//开始不处于移动状态
	setIsPowerEnough(true);//开始状态电力足够
	setIsPowerConsumption(false);//该塔耗电

	basePlate = Sprite::createWithSpriteFrameName("fort1.png"); //塔的基座
	addChild(basePlate,2);
	//log("111111111111111");
	rotateArtillery = Sprite::createWithSpriteFrameName("fort2.png");  //塔的弓箭
	rotateArtillery->setPosition(0, 0);
	rotateArtillery->setRotation(270);
	addChild(rotateArtillery,3);

	//这个不能少，虽然不显示，这是因为这个炮塔比较特殊，是由2个sprite拼接而成的
	towerSprite = Sprite::create("towerItem/Item2.png");
	addChild(towerSprite);
	towerSprite->setVisible(false);

	gradeSprite = Sprite::create("level1.png");
	gradeSprite->setScale(2);
	gradeSprite->setAnchorPoint(Point(0, 0));
	gradeSprite->setPosition(this->getPosition().x + 60, -towerSprite->getBoundingBox().size.height / 2-10);
	gradeSprite->setOpacity(0);//开始让其不可见
	addChild(gradeSprite, 6);

	//沉默图片
	noAttackSprite = Sprite::create("noSpeak.png");
	noAttackSprite->setScale(0.9);
	noAttackSprite->setAnchorPoint(Point(0.5, 0));
	noAttackSprite->setPosition(towerSprite->getBoundingBox().size.width / 2 - 70, towerSprite->getBoundingBox().size.height - 60);
	addChild(noAttackSprite, 8);
	noAttackSprite->setVisible(false);

	//血量条背景图片
	towerHpSprite = Sprite::create("manaBarBg.png");
	towerHpSprite->setPosition(towerSprite->getPosition() + Point(0, 100));
	towerHpSprite->setScale(0.8);
	addChild(towerHpSprite, 10);
	//炮塔血量
	hp = 2;
	maxHp = 2;
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

    schedule(schedule_selector(ArtilleryTower::rotateAndShoot), 0.5f); //别忘了调用函数，时间间隔越小，炮塔旋转越平滑
	schedule(schedule_selector(ArtilleryTower::shoot), 2.5f);
	
	addTouch();

	return true;
}

void ArtilleryTower::rotateAndShoot(float dt)
{
	checkNearestEnemy(0.5);
	if (nearestEnemy != nullptr && nearestEnemy->getIsVisible()&&!isSilenced)  //找到射击范围内最近的敌人
	{
		Point enemyAndTowerVector = nearestEnemy->getPosition() - this->getPosition(); //得到敌人与本炮塔之间的向量
		float rotateRadians = enemyAndTowerVector.getAngle();                          //得到该向量与x轴的夹角 弧度制
		float rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

		float speed = 0.5 / M_PI;  //表示pi，是一个宏
		float rotateDuration = fabs(speed * rotateRadians);//得到炮塔旋转时间,注意绝对值

		//log("enemyPos: %f %f", nearestEnemy->getPosition().x, nearestEnemy->getPosition().y);
		//log("towerPos: %f %f", this->getPosition().x, this->getPosition().y);

		rotateArtillery->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees-90)/*, CallFuncN::create(CC_CALLBACK_0(ArtilleryTower::shoot, this))*/, NULL)); //先旋转再开火，注意角度要减90度才能对应上炮管
		                                                                                                                                                                            //分开使用，开火速率不同于旋转速率


	}
}

Sprite* ArtilleryTower::ArtilleryTowerBullet()
{
	
	auto bullet = Sprite::create("bullet/Artillerybullet1.png");
	bullet->setScale(1.3);

	bullet->setPosition(rotateArtillery->getPosition()); //单纯这样只是把子弹设置在了炮塔的正中心，而不是炮管末端
	bullet->setRotation(rotateArtillery->getRotation()+180);
	addChild(bullet,1);
	return bullet;

}

void ArtilleryTower::shoot(float dt)
{
	auto instance = GameManager::getInstance();
	auto bulletVector = instance->bulletVector;

	if (nearestEnemy != nullptr && nearestEnemy->getCurrHp() > 0 && nearestEnemy->getIsVisible() && !isSilenced) //敌人可见才能攻击
	{
		auto currBullet = ArtilleryTowerBullet();
		currBullet->setTag(attack);//将tag作为攻击力，碰撞检测时直接获取tag即可知道子弹的攻击力了
		(instance->bulletVector).pushBack(currBullet);

		auto moveDuration = getRate();
		Vec2 shootVector = nearestEnemy->getPosition() - this->getPosition();
		Vec2 normalizedShootVector = -shootVector.getNormalized();   //单位化向量，使其长度为1
		auto farthestDistance = Director::getInstance()->getWinSize().width;   //子弹射程最远为屏幕的宽
		Point overshotVector = normalizedShootVector * farthestDistance;
		Point offscreenPoint = (rotateArtillery->getPosition() - overshotVector);  //算出子弹在屏幕上消失的点

		currBullet->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint),  //不是恒定的速率飞过去我擦
			CallFuncN::create(CC_CALLBACK_1(ArtilleryTower::removeBullet, this)),
			NULL));
		currBullet = NULL;

	}
}

void ArtilleryTower::removeBullet(Node* pSender)
{
	GameManager *instance = GameManager::getInstance();

	Sprite *sprite = (Sprite *)pSender;
	instance->bulletVector.eraseObject(sprite);
	sprite->removeFromParent();
}