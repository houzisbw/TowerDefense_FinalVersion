#include "EnemyThrow_son.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio::timeline;

#define EFFECT_NUM 10
#define ATTACK_RANGE_TAG 10001

enum
{
	PRISM_TOWER = 0,
	ARTILLERY_TOWER = 1,
	ICE_TOWER = 2,
	ANTI_TOWER = 3,
	POWER_PLANT = 4,
	FORETELLING_TOWER = 5
};

EnemyThrow_son::EnemyThrow_son()
{
	aStar = new AStarPath;   //有bug这里，待排查
}


EnemyThrow_son::~EnemyThrow_son()
{
	log("delete sonnnnnnnnnnnnnn");
	delete aStar;
}

bool EnemyThrow_son::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	instance = GameManager::getInstance();

	sprite = Sprite::create("monster/monster_throw/son/left1.png"); //怪物本身的图片，初始化显
	addChild(sprite);

	iceSprite = Sprite::create("ice.png"); //冰块图片
	iceSprite->setPosition(sprite->getPosition());
	addChild(iceSprite, 2);
	iceSprite->setVisible(false);


	setType(6);//这是第6种敌人
	setIsVisible(true);//不是隐形单位
	setRunSpeed(4);
	setHpPercentage(100);
	setMoney(8);//只值4元钱
	setIsReachable(false);//初始化不可达最近炮塔

	animationRight = createAnimation("monster/monster_throw/son/left", 4, 0.1f, 50, 57);
	AnimationCache::getInstance()->addAnimation(animationRight, "monster_throw_son_right");  //这个cache里存有各种做好的动画，重要，以便后来取出用动画
	animationLeft = createAnimation("monster/monster_throw/son/left", 4, 0.1f, 50, 57);
	AnimationCache::getInstance()->addAnimation(animationLeft, "monster_throw_son_left");



	createAndSetHpBar();

	schedule(schedule_selector(EnemyThrow_son::deFroze), 0.2f); //每隔0.2秒执行一次检测是否减速函数
	schedule(schedule_selector(EnemyThrow_son::changeDirection), 0.4f);
	schedule(schedule_selector(EnemyThrow_son::decreaseEnergy), 1.0f);//每隔一秒执行一次能量衰减函数

	


	return true;
}

//敌人冻住之后的解冻计时函数
void EnemyThrow_son::deFroze(float dt)
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

void EnemyThrow_son::changeDirection(float dt)
{
	
	sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("monster_throw_son_right"))); //从cache里取出动画播放		
	
}

EnemyThrow_son* EnemyThrow_son::createEnemyThrow_son(cocos2d::Vector<cocos2d::Node*> points, int hp,float time)
{
	auto enemy = new EnemyThrow_son();
	if (enemy && enemy->init())
	{
		enemy->scheduleOnceToWalk(time);
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

void EnemyThrow_son::createAndSetHpBar()
{
	//血条背景
	hpBgSprite = Sprite::create("monsterBloodBg.png");
	hpBgSprite->setScaleX(0.5);
	//log("%f,%f", this->getContentSize().width, this->getContentSize().height); //此时this的大小为0，因为还没有创建实体
	hpBgSprite->setPosition(Point(/*sprite->getBoundingBox().size.width / 2*/0, sprite->getBoundingBox().size.height - 10)); //sprite在子类里被初始化
	addChild(hpBgSprite, 4);

	//血条
	hpBar = ProgressTimer::create(Sprite::create("monster_rage_bar.png")); //参数是一个图片sprite
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Point(0, 0.5f));
	hpBar->setBarChangeRate(Point(1, 0));
	hpBar->setPercentage(hpPercentage);
	hpBar->setPosition(Point(hpBgSprite->getContentSize().width / 2, hpBgSprite->getContentSize().height / 3 * 2 - 4));
	hpBgSprite->addChild(hpBar, 5);

}

//敌人能量衰减函数
void EnemyThrow_son::decreaseEnergy(float dt)
{
	currHp--;
	float percentage = float(currHp) / float(maxHp) * 100;
	hpBar->setPercentage(percentage);
	if (currHp <= 0)
	{
		//让怪物本身不可见
		sprite->setVisible(false);
		hpBar->setVisible(false);
		hpBgSprite->setVisible(false);

		dieNode = CSLoader::createNode("MushroomDie.csb");
		dieNode->setPosition(this->getPosition());
		instance->map->addChild(dieNode, 41);

		auto timeLine = CSLoader::createTimeline("MushroomDie.csb");
		timeLine->gotoFrameAndPlay(0);

		auto delay = DelayTime::create(0.7);
		auto del = CallFunc::create([=](){
			dieNode->removeFromParent();
			this->removeFromParent();
		});

		dieNode->runAction(timeLine);
		this->runAction(Sequence::create(delay, del, NULL));
	}
}

//冲向离自己最近的炮塔
void EnemyThrow_son::waitToRunToNearestTower(float dt)
{
	//清空列表
	openListClear();
	//搜寻附近最近的炮塔
	int maxDistance = 10000;
	nearestTower = nullptr;
	for (int i = 0; i < instance->towerVector.size(); i++)
	{
		auto tower = instance->towerVector.at(i);
		int towerType = tower->getTowerType();
		//只有炮塔才能收到攻击
		if (sqrt((this->getPosition().x - tower->getPosition().x)*(this->getPosition().x - tower->getPosition().x) + (this->getPosition().y - tower->getPosition().y)*(this->getPosition().y - tower->getPosition().y)) < maxDistance && towerType<=4)
		{
			maxDistance = sqrt((this->getPosition().x - tower->getPosition().x)*(this->getPosition().x - tower->getPosition().x) + (this->getPosition().y - tower->getPosition().y)*(this->getPosition().y - tower->getPosition().y));
			nearestTower = tower;
		}
	}
	//如果最近炮塔非空
	if (nearestTower)
	{
		auto enemyTilePos = convertTotileCoord(this->getPosition()); //得到敌人自己的tile坐标
		auto towerTilePos = convertTotileCoord(nearestTower->getPosition());//得到最近炮塔的坐标

		//获取炮塔种类
		towerType = nearestTower->getTowerType();
		//清空炮塔占地
		int towerType = nearestTower->getTowerType();
		removeOrAddLandOccupation(towerType, towerTilePos, 0);

		runToNearestTower(enemyTilePos.x ,enemyTilePos.y, towerTilePos.x, towerTilePos.y);//寻路过程

		//恢复炮塔占地
		removeOrAddLandOccupation(towerType, towerTilePos, 1);
	}
}

//开始冲向离自己最近的防御塔
void EnemyThrow_son::runToNearestTower(int srcX, int srcY, int destX, int destY)
{

	aStar->AStar(srcX, srcY, destX, destY);
	isReachable = aStar->getIsReachable();//判断是否可达目的地
	//如果可达最近炮塔才能向炮塔行走
	

	if (isReachable)
	{
		roadPoint = aStar->roadPoint;
		roadLength = roadPoint.size() - 1;
		runFollowShortestRoad();//具体开始行走了
	}
}

Vec2 EnemyThrow_son::convertTotileCoord(Vec2 position)
{
	auto mapSize = instance->map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// 把position转换为瓦片坐标，确保得到的是整数
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}

//沿着最短路vector行走
void EnemyThrow_son::runFollowShortestRoad()
{

	//递归调用该函数以达到不断行走的目的
	if (roadLength != 0)
	{
		//获取当前坐标和下一个坐标：用来判断玩家行走方向
		Point nowPoint = roadPoint[roadLength];
		roadLength--;
		Point nextPoint = roadPoint[roadLength];

		Point temp = Point(roadPoint[roadLength].x * 32, 80 * 32 - roadPoint[roadLength].y * 32);
		this->runAction(Sequence::create(MoveTo::create(0.2, temp)
			, CallFuncN::create(CC_CALLBACK_0(EnemyThrow_son::runFollowShortestRoad, this))
			, NULL));
	}
	else // 蘑菇已经到达目的地(炮塔) 产生爆炸动画
	{
		//检测目的炮塔是否存在,这一步不可少,否则出错
		int maxDistance = 10000;
		BaseTower* TempNearestTower = nullptr;
		for (int i = 0; i < instance->towerVector.size(); i++)
		{
			auto tower = instance->towerVector.at(i);
			//只有炮塔才能收到攻击
			if (sqrt((this->getPosition().x - tower->getPosition().x)*(this->getPosition().x - tower->getPosition().x) + (this->getPosition().y - tower->getPosition().y)*(this->getPosition().y - tower->getPosition().y)) < maxDistance && towerType <= 4)
			{
				maxDistance = sqrt((this->getPosition().x - tower->getPosition().x)*(this->getPosition().x - tower->getPosition().x) + (this->getPosition().y - tower->getPosition().y)*(this->getPosition().y - tower->getPosition().y));
				TempNearestTower = tower;
			}
		}
		//炮塔掉血	
		if (TempNearestTower && nearestTower && towerType <= 4 && TempNearestTower == nearestTower && !nearestTower->getTowerHp()->isVisible()) //只有炮塔(5种)才受伤害 且 目的地炮塔和当前算出的最近炮塔一样才能继续，否则nearestTower指针不为空，但是其指向的内容已经被释放了，所以要出错
		{                                                                                                             //炮塔选中状态(血条可见)不掉血，无敌状态
			auto towerHp = nearestTower->getHp();
			nearestTower->setHp(towerHp - 1);

			float percentage = (float)(towerHp - 1) / (float)(nearestTower->getMaxHp()) * 100;
			nearestTower->getTowerHp()->setPercentage(percentage);

			if (nearestTower->getHp() <= 0)
			{
				removeTower();
				nearestTower->removeFromParent();//移除炮塔
				nearestTower = nullptr;		

			}
		
		}

		//让怪物本身不可见
		sprite->setVisible(false);
		hpBar->setVisible(false);
		hpBgSprite->setVisible(false);

		dieNode2 = CSLoader::createNode("MushroomDie.csb");
		dieNode2->setPosition(this->getPosition());
		instance->map->addChild(dieNode2, 41);

		auto timeLine = CSLoader::createTimeline("MushroomDie.csb");
		timeLine->gotoFrameAndPlay(0); 

		auto delay = DelayTime::create(0.7);
		auto del = CallFunc::create([=](){
			dieNode2->removeFromParent();
			this->removeFromParent();	
		});

		dieNode2->runAction(timeLine);
		this->runAction(Sequence::create(delay, del, NULL));

		return;
	}
}

void EnemyThrow_son::removeTower()
{
	//获取tower的type，判断属于哪一种塔
	int towerTag = nearestTower->getTag();
	int towerType = nearestTower->getTowerType();
	//从prismTowerVector中移除
	if (towerType == PRISM_TOWER)
	{
		instance->prismTowerVector.eraseObject(dynamic_cast<PrismTower*>(nearestTower));
	}
	//从反隐塔vector中移除
	if (towerType == ANTI_TOWER)
	{
		instance->antiVisibleTowerVector.eraseObject(dynamic_cast<AntiVisibleTower*>(nearestTower));
	}


	//去除占地面积
	Point towerSpriteCenterPos = convertTotileCoord(nearestTower->getPosition());
	int centerX = (int)towerSpriteCenterPos.x;
	int centerY = (int)towerSpriteCenterPos.y;
	removeOrAddLandOccupation(towerType, Point(centerX, centerY), 0);//0表示去除

	//小地图处理
	auto tempSmall = instance->smallMap->map->getChildByTag(instance->towerVector.size()); //得到的node父亲类，要转化为sprite类才行 ,注意移除哪一个绿色方块是根据tag决定的，tower的tag和绿色方块的tag一一对应，是一样的	
	//log("temp small tag %d", indicatorTower->getTag());
	instance->smallMap->greenVector.eraseObject(dynamic_cast<Sprite*>(tempSmall));
	tempSmall->removeFromParent();


	//电量恢复 
	if (nearestTower->getIsPowerConsumption())
	{
		instance->setRedPower(instance->getRedPower() - nearestTower->getPower());
	}
	char temp[10];
	sprintf(temp, "%d/%d", instance->getRedPower(), instance->getGreenPower());
	std::string str = temp;
	instance->powerText->setText(str);

	//检测电力
	//if (instance->getRedPower() <= instance->getGreenPower())
	//{

	//	powerRedGreen->loadTexture("powerGreen.png");
	//	for (int i = 0; i < instance->towerVector.size(); i++)
	//	{
	//		auto tempTower = instance->towerVector.at(i);
	//		//如果该塔耗电
	//		if (tempTower->getIsPowerConsumption())
	//		{
	//			tempTower->noPowerSprite->setVisible(false);
	//			tempTower->setIsPowerEnough(true);
	//		}
	//	}
	//}
	//else //电力不足
	//{
	//	powerRedGreen->loadTexture("powerRed.png");
	//	for (int i = 0; i < instance->towerVector.size(); i++)
	//	{
	//		auto tempTower = instance->towerVector.at(i);
	//		//如果该塔耗电
	//		if (tempTower->getIsPowerConsumption())
	//		{
	//			tempTower->noPowerSprite->setVisible(true);
	//			tempTower->setIsPowerEnough(false);
	//		}
	//	}
	//	//小地图退出
	//	if (isForeTellingTowerComeIn && foreTellingTowerCount>0)
	//	{
	//		smallMapComeInAnimation(false);
	//		isForeTellingTowerComeIn = false;
	//	}
	//}


	//vector中也要做同样的事,注意2个vector中都要移除  这句话得放最后面才行，否则vector越界
	instance->towerVector.eraseObject(nearestTower);
}

void EnemyThrow_son::scheduleOnceToWalk(float time)
{
	scheduleOnce(schedule_selector(EnemyThrow_son::waitToRunToNearestTower), time);//延时3.3秒开始执行函数,只执行一次该函数
	
}

//开启列表清空
void EnemyThrow_son::openListClear()
{
	while (!aStar->open.empty()) //不能少！！
	{
		aStar->open.pop();
	}
}

//暂时移除炮塔占地面积:让蘑菇能够寻路，否则无法寻路
void  EnemyThrow_son::removeOrAddLandOccupation(int type, cocos2d::Point pos, int removeOrAdd)
{
	int centerX = (int)pos.x;
	int centerY = (int)pos.y;
	switch (type)
	{
	case 5: //发电厂
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY - 1] = removeOrAdd;
		break;
	case 1: //一般炮塔
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY - 1] = removeOrAdd;
		break;
	case 0: //聚能塔
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		break;
	case 2: //冰塔
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		break;
	case 3: //反隐塔
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		break;
	case 4: //多方向攻击塔
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		break;
	case 6://预言圣坛
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY - 1] = removeOrAdd;
		break;
	case 7://训练中心
		instance->walkMap[centerX][centerY] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY - 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY] = removeOrAdd;
		instance->walkMap[centerX][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX - 1][centerY + 1] = removeOrAdd;
		instance->walkMap[centerX + 1][centerY - 1] = removeOrAdd;
		break;

	}
}

//碰撞检测过程:如果检测到蘑菇与炮塔碰撞则移除蘑菇
void EnemyThrow_son::collisionDetection(float dt)
{
	//auto thisRect = Rect{ this->getPosition().x, this->getPosition().y, sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height};
	//for (int i = 0; i < instance->towerVector.size(); i++)
	//{
	//	auto tower = instance->towerVector.at(i);
	//	auto towerPos = tower->getPosition();
	//	auto towerRect = Rect{ towerPos.x, towerPos.y, tower->towerSprite->getBoundingBox().size.width, tower->towerSprite->getBoundingBox().size.height };
	//	if (thisRect.intersectsRect(towerRect))
	//	{
	//		this->removeFromParent();//移除蘑菇
	//	}

	//}
}

