#include "Soldier.h"
#include "GameManager.h"
#include "EnemyRage.h"
USING_NS_CC;
#define FRAME_NUM 3
#define FRAME_WALK 8
#define MAP_SIZE 80



Soldier::Soldier()
{
	aStar = new AStarPath;
}


Soldier::~Soldier()
{
	delete aStar;
}

bool Soldier::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	instance = GameManager::getInstance();

	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	soldierSprite = Sprite::create("soldier/down_1.png");
	addChild(soldierSprite,4);

	//士兵目标地点指示器
	soldierDestPointer = Sprite::create("pointer.png");
	soldierDestPointer->setScale(1.5);
	instance->map->addChild(soldierDestPointer, 14);
	soldierDestPointer->setPosition(-400, -400); //让其不可见
	soldierDestPointer->setVisible(false);

	isReachable = true;
	isDeployed = false;
	pollutionRange = 250;//毒气作用范围 
	attackRange = 300;
	attackDamage = 4;//攻击力2

	time = 100;//初始化生存时间为100
	timeBarBg = Sprite::create("manaBarBg.png");
	timeBarBg->setScale(0.45);
	timeBarBg->setPosition(soldierSprite->getPosition() + Point(0, 70));
	addChild(timeBarBg, 11);


	//英雄蓝条初始化
	timeBar = ProgressTimer::create(Sprite::create("time.png"));
	timeBar->setScaleX(0.9);
	timeBar->setScaleY(2.2);
	timeBar->setType(ProgressTimer::Type::BAR);
	timeBar->setMidpoint(Point(0, 0.5));
	timeBar->setBarChangeRate(Point(1, 0));
	timeBar->setPercentage(100);
	timeBar->setAnchorPoint(Point(0, 0.5));
	timeBar->setPosition(timeBarBg->getPosition() - Point(timeBarBg->getBoundingBox().size.width / 2 - 5, 0));
	addChild(timeBar, 10);

	//士兵属性条不可见
	timeBar->setVisible(false);
	timeBarBg->setVisible(false);

	//毒气图片初始化
	deployedSprite = Sprite::create("toxicFog.png");
	addChild(deployedSprite, 5);
	deployedSprite->setVisible(false);
	deployedSprite->setScale(1.5);
	auto rb = RotateBy::create(6, 90);
	auto rp = RepeatForever::create(rb);
	deployedSprite->runAction(rp);


	//初始化英雄走路的动画:向下走
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/down_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkDown.pushBack(frame);
	}
	animationWalkDown = Animation::createWithSpriteFrames(walkDown, 0.1);

	//初始化英雄走路的动画:向上走
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/up_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkUp.pushBack(frame);
	}
	animationWalkUp = Animation::createWithSpriteFrames(walkUp, 0.1);

	//初始化英雄走路的动画:向左
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/left_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeft.pushBack(frame);
	}
	animationWalkLeft = Animation::createWithSpriteFrames(walkLeft, 0.1);

	//初始化英雄走路的动画:向右
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/right_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRight.pushBack(frame);
	}
	animationWalkRight = Animation::createWithSpriteFrames(walkRight, 0.1);


	//初始化英雄走路的动画:向左上
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/leftup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeftUp.pushBack(frame);
	}
	animationWalkLeftUp = Animation::createWithSpriteFrames(walkLeftUp, 0.1);

	//初始化英雄走路的动画:向左下
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/leftdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeftDown.pushBack(frame);
	}
	animationWalkLeftDown = Animation::createWithSpriteFrames(walkLeftDown, 0.1);

	//初始化英雄走路的动画:向右上
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/rightup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRightUp.pushBack(frame);
	}
	animationWalkRightUp = Animation::createWithSpriteFrames(walkRightUp, 0.1);

	//初始化英雄走路的动画:向右下
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "soldier/rightdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRightDown.pushBack(frame);
	}
	animationWalkRightDown = Animation::createWithSpriteFrames(walkRightDown, 0.1);


	//将animation加入到cache中去,必须将animation加入到cache中去，用的时候取出来,注意名字要有区分，因为是单例，所以动画名字不能冲突
	AnimationCache::getInstance()->addAnimation(animationWalkDown, "soldier_down");
	AnimationCache::getInstance()->addAnimation(animationWalkUp, "soldier_up");
	AnimationCache::getInstance()->addAnimation(animationWalkLeft, "soldier_left");
	AnimationCache::getInstance()->addAnimation(animationWalkRight, "soldier_right");

	AnimationCache::getInstance()->addAnimation(animationWalkLeftUp, "soldier_leftup");
	AnimationCache::getInstance()->addAnimation(animationWalkLeftDown, "soldier_leftdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightDown, "soldier_rightdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightUp, "soldier_rightup");

	this->schedule(schedule_selector(Soldier::decreaseSurviveTime), 2);
	this->schedule(schedule_selector(Soldier::checkNearestEnemy), 0.2);//开始不断搜寻最近的敌人
	this->schedule(schedule_selector(Soldier::shoot), 2);//开始射击

	return true;
}

void Soldier::setAnimation(int dir)//设置英雄的行走动画,表示英雄行走方向的int变量dir:0表示左上角，1表示正上方..........7表示正左方
{
	soldierSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_leftup"))));
		break;
	case 1:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_up"))));
		break;
	case 2:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_rightup"))));
		break;
	case 3:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_right"))));
		break;
	case 4:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_rightdown"))));
		break;
	case 5:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_down"))));
		break;
	case 6:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_leftdown"))));
		break;
	case 7:
		soldierSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("soldier_left"))));
		break;
	}
}
void Soldier::stopSoldierAction(int dir)//英雄停止时正确显示其站位
{
	soldierSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		soldierSprite->setTexture("soldier/leftup_1.png");
		break;
	case 1:
		soldierSprite->setTexture("soldier/up_1.png");
		break;
	case 2:
		soldierSprite->setTexture("soldier/rightup_5.png");
		break;
	case 3:
		soldierSprite->setTexture("soldier/right_5.png");
		break;
	case 4:
		soldierSprite->setTexture("soldier/rightdown_5.png");
		break;
	case 5:
		soldierSprite->setTexture("soldier/down_1.png");
		break;
	case 6:
		soldierSprite->setTexture("soldier/leftdown_1.png");
		break;
	case 7:
		soldierSprite->setTexture("soldier/left_5.png");
		break;
	}
}

void Soldier::runFollowShortestRoad()//沿着最短路行走
{
	//取消占地
	auto tempPos = this->getPosition();
	auto tempTilePos = tempPos / 32.0;
	instance->walkMap[(int)tempTilePos.x][MAP_SIZE-(int)tempTilePos.y] = 0;//取消占地

	//log("raod point %f %f", roadPoint[0].x, roadPoint[0].y);

	//递归调用该函数以达到不断行走的目的
	if (roadLength != 0)
	{
		//获取当前坐标和下一个坐标：用来判断玩家行走方向
		Point nowPoint = roadPoint[roadLength];
		roadLength--;
		Point nextPoint = roadPoint[roadLength];

		nowDir = getHeroDirection(nowPoint, nextPoint);

		if (prevDir != nowDir)
		{
			this->setAnimation(nowDir);
		}
		prevDir = nowDir;

		Point temp = Point(roadPoint[roadLength].x * 32, 80 * 32 - roadPoint[roadLength].y * 32);
		this->runAction(Sequence::create(MoveTo::create(0.2, temp)
			, CallFuncN::create(CC_CALLBACK_0(Soldier::runFollowShortestRoad, this))
			, NULL));
	}
	else // 玩家已经到达目的地
	{
		//heroDestPointer->setVisible(false);
		this->stopSoldierAction(nowDir);
		//确定占地面积
		auto mapPos = this->getPosition();
		auto tilePos = mapPos / 32.0;
		//log("tile pos end %f %f", tilePos.x, tilePos.y);
		instance->walkMap[(int)tilePos.x][MAP_SIZE - (int)tilePos.y] = 1;//占地了,注意要用80减才行
		soldierDestPointer->setVisible(false);

		return;
	}
}
int Soldier::getHeroDirection(cocos2d::Point now, cocos2d::Point next) //获取士兵行走方向的函数
{
	int tempX = next.x - now.x;
	int tempY = next.y - now.y;

	if (tempX == 0 && tempY == 1) //向正下方走
	{
		return 5;
	}
	else if (tempX == 0 && tempY == -1)//正上方走
	{
		return 1;
	}
	else if (tempX == 1 && tempY == 0)//正右方走
	{
		return 3;
	}
	else if (tempX == -1 && tempY == 0)//正左方走
	{
		return 7;
	}
	else if (tempX == 1 && tempY == -1)//右上方走
	{
		return 2;
	}
	else if (tempX == -1 && tempY == 1)//左下方走
	{
		return 6;
	}
	else if (tempX == -1 && tempY == -1)//左上方走
	{
		return 0;
	}
	else if (tempX == 1 && tempY == 1)//右下方走
	{
		return 4;
	}

}

void Soldier::pathFinding(int srcX, int srcY, int destX, int destY)
{
	//log("源目的地坐标：%d %d %d %d", srcX, srcY, destX, destY);
	aStar->AStar(srcX,srcY,destX,destY);
	isReachable = aStar->getIsReachable();//判断是否可达目的地
    //复制路径数组 和 最短路长度
	roadPoint = aStar->roadPoint;
	roadLength = roadPoint.size() - 1;
	//log("soldier road lenght %d", roadLength);


}

void Soldier::openListClear()//清空openlist列表
{
	while (!aStar->open.empty()) //不能少！！
	{
		aStar->open.pop();
	}
}

void Soldier::setTimeBarVisibleOrNot(bool is)//设置其属性条可见与否
{
	
	timeBar->setVisible(is);
	timeBarBg->setVisible(is);
}

void Soldier::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Soldier::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Soldier::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Soldier::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->soldierSprite);
}

bool Soldier::onTouchBegan(Touch* pTouch, Event* pEvent)
{

	return true;
}
void Soldier::onTouchMoved(Touch* pTouch, Event* pEvent)
{

}
void Soldier::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	
}
void Soldier::decreaseSurviveTime(float dt)//生存时间减少函数 1分钟后消失
{
	if (isDeployed) //只有部署状态才减少生存时间
	{

		time--;
		timeBar->setPercentage(time);
		if (time <= 0)
		{

			//从2个容器里删除
			for (std::vector<Soldier*>::iterator iter = instance->soldierInRectVector.begin(); iter != instance->soldierInRectVector.end();)
			{
				if (*iter == this)
					iter = instance->soldierInRectVector.erase(iter);
				else
					iter++;
			}

			for (std::vector<Soldier*>::iterator iter = instance->soldierVector.begin(); iter != instance->soldierVector.end();)
			{
				if (*iter == this)
					iter = instance->soldierVector.erase(iter);
				else
					iter++;
			}

			soldierDestPointer->removeFromParent();//别忘了去除指示器

			instance->totalSoldierNum--;//士兵总数减少1

			this->removeFromParent();
		}
	}

}

void Soldier::checkNearestEnemy(float dt)//搜寻附近的最近敌人
{
	instance = GameManager::getInstance();
	auto currMinDistance = attackRange;
	EnemyBase * enemyTemp = nullptr;
	for (int i = 0; i < instance->enemyVector.size(); i++)
	{
		auto enemy = instance->enemyVector.at(i);
		float distance = this->getPosition().getDistance(enemy->getPosition()); //注意这里没有sprite。可能出问题
		if (distance < currMinDistance)
		{
			currMinDistance = distance;
			enemyTemp = enemy;
		}
	}

	nearestEnemy = enemyTemp;
}

void Soldier::shoot(float dt) //射击函数
{
	if (nearestEnemy != nullptr && nearestEnemy->getCurrHp() > 0 && nearestEnemy->getIsVisible() && isDeployed==false) //当该塔不在移动状态 且 未部署时才能攻击
	{


		auto PosSoldier = this->getPosition();
		auto PosEnemy = nearestEnemy->getPosition()+Point(20,20);
		//log(" worldPosPrismTower: %f %f", PosPrismTower.x, PosPrismTower.y);   

		//通过贴图来达到激光的效果
		auto laser = Sprite::create("soldier_laser.png");
		Point enemyAndSoldierVector = PosEnemy - PosSoldier;                          //得到敌人与本炮塔之间的向量
		float rotateRadians = enemyAndSoldierVector.getAngle();                          //得到该向量与x轴的夹角 弧度制
		float rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);

		float length = sqrt((PosEnemy.x - PosSoldier.x)*(PosEnemy.x - PosSoldier.x) + (PosEnemy.y - PosSoldier.y + 30)*(PosEnemy.y - PosSoldier.y + 30));  //改变30达到长度效果
		float scale = length / (float)laser->getContentSize().width;
		//log("sclae : %f", scale);
		laser->setScaleX(scale+0.5);
		laser->setScaleY(2);
		laser->setRotation(rotateDegrees + 180);
		laser->setAnchorPoint(Point(1, 0.5));
		laser->setPosition(0, 20);
		addChild(laser, 3);
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


		laser->runAction(Sequence::create(action1, delayTime, fadeOut, action2, NULL));

		//如果是狂怒敌人的话，增加怒气
		int enemyType = nearestEnemy->getType();
		if (enemyType == 4)
		{
			increaseRageIfHitEnemyWithRage(nearestEnemy);
		}

		//敌人掉血
		auto currHp = nearestEnemy->getCurrHp();
		//将tag作为攻击力减去
		currHp -= attackDamage;
		nearestEnemy->setCurrHp(currHp);  //别忘了设置hp，否则下次检查hp不会变化
		auto hpPercent = (float)currHp / (float)nearestEnemy->getMaxHp();
		nearestEnemy->getHpBar()->setPercentage(hpPercent * 100);

		if (currHp <= 0)
		{
			//如果敌人是投掷型敌人,则要投掷出蘑菇攻击玩家防御塔
			/*if (enemyType == 5)
			{
				EnemyThrowMushroomToTower(nearestEnemy);
			}*/

			instance->enemyVector.eraseObject(nearestEnemy); //不能用enemyVector，必须用instance的，因为只是一个复制过程	
			//小地图处理 万万不能忘记，这个bug找了好几天
			auto tempSmallRed = instance->smallMap->map->getChildByTag(nearestEnemy->getTag());
			instance->smallMap->redVector.eraseObject(static_cast<Sprite*>(tempSmallRed));
			tempSmallRed->removeFromParent();

			nearestEnemy->enemyExplosionAnimation();//爆炸动画顺带移除敌人

		}


	}
}

void Soldier::increaseRageIfHitEnemyWithRage(EnemyBase* enemy)//如果子弹击中怒气型敌人，则敌人增加怒气
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
