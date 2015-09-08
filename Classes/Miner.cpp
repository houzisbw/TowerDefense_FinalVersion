#include "Miner.h"
#include "CampTower.h"
USING_NS_CC;
#define FRAME_NUM 3
#define FRAME_WALK 8
#define MAP_SIZE 80
#define MINING_TIME 5
#define MINING_GOLD 10 //每次采矿得到10金币

Miner::Miner()
{
	aStar = new AStarPath;
}


Miner::~Miner()
{
}

bool Miner::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	nearestCamp = nullptr;
	isReached = false;
	isMiningFinished = false;
	miningTimer = 0;//计时器初始化为0
	instance = GameManager::getInstance();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	minerSprite = Sprite::create("miner/down_1.png");
	addChild(minerSprite);

	//士兵目标地点指示器
	minerDestPointer = Sprite::create("pointer.png");
	minerDestPointer->setScale(1.5);
	instance->map->addChild(minerDestPointer, 14);
	minerDestPointer->setPosition(-400, -400); //让其不可见
	minerDestPointer->setVisible(false);

	isReachable = true;


	time = 100;//初始化生存时间为100
	timeBarBg = Sprite::create("manaBarBg.png");
	timeBarBg->setScale(0.45);
	timeBarBg->setPosition(minerSprite->getPosition() + Point(0, 70));
	addChild(timeBarBg, 11);


	//矿工生命值初始化
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

	//初始化矿工状态标识
	stateSprite = Sprite::create("miner_idle.png");
	addChild(stateSprite, 12);
	stateSprite->setPosition(timeBar->getPosition()+Point(50,60));


	//初始化英雄走路的动画:向下走
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/down_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkDown.pushBack(frame);
	}
	animationWalkDown = Animation::createWithSpriteFrames(walkDown, 0.1);

	//初始化英雄走路的动画:向上走
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/up_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkUp.pushBack(frame);
	}
	animationWalkUp = Animation::createWithSpriteFrames(walkUp, 0.1);

	//初始化英雄走路的动画:向左
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/left_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeft.pushBack(frame);
	}
	animationWalkLeft = Animation::createWithSpriteFrames(walkLeft, 0.1);

	//初始化英雄走路的动画:向右
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/right_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRight.pushBack(frame);
	}
	animationWalkRight = Animation::createWithSpriteFrames(walkRight, 0.1);


	//初始化英雄走路的动画:向左上
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/leftup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeftUp.pushBack(frame);
	}
	animationWalkLeftUp = Animation::createWithSpriteFrames(walkLeftUp, 0.1);

	//初始化英雄走路的动画:向左下
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/leftdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeftDown.pushBack(frame);
	}
	animationWalkLeftDown = Animation::createWithSpriteFrames(walkLeftDown, 0.1);

	//初始化英雄走路的动画:向右上
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/rightup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRightUp.pushBack(frame);
	}
	animationWalkRightUp = Animation::createWithSpriteFrames(walkRightUp, 0.1);

	//初始化英雄走路的动画:向右下
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "miner/rightdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRightDown.pushBack(frame);
	}
	animationWalkRightDown = Animation::createWithSpriteFrames(walkRightDown, 0.1);


	//将animation加入到cache中去,必须将animation加入到cache中去，用的时候取出来,注意名字要有区分，因为是单例，所以动画名字不能冲突
	AnimationCache::getInstance()->addAnimation(animationWalkDown, "miner_down");
	AnimationCache::getInstance()->addAnimation(animationWalkUp, "miner_up");
	AnimationCache::getInstance()->addAnimation(animationWalkLeft, "miner_left");
	AnimationCache::getInstance()->addAnimation(animationWalkRight, "miner_right");

	AnimationCache::getInstance()->addAnimation(animationWalkLeftUp, "miner_leftup");
	AnimationCache::getInstance()->addAnimation(animationWalkLeftDown, "miner_leftdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightDown, "miner_rightdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightUp, "miner_rightup");

	//this->schedule(schedule_selector(Miner::decreaseSurviveTime), 2);
	curState = stateIdle;//初始为空闲状态 
	scheduleUpdate();//有限状态机开始运转

	return true;
}

//设置采矿者状态
void Miner::setState(minerState nextState)
{
	curState = nextState;
}

void Miner::update(float dt)//每一帧更新有限状态机
{
	switch (curState) 
	{
		case stateIdle: //空闲状态
		{
							  //清空所有数据
							  miningTimer = 0;
							  isReached = false;
							  this->aStar->roadPoint.clear(); 
							  this->openListClear();
							  nearestCamp = nullptr;
							  this->unschedule(schedule_selector(Miner::miningTimerIncrease));
							  this->getStateSprite()->setTexture("miner_idle.png");
		}
		break;

		case stateMining://采矿状态
		{
							if (miningTimer == MINING_TIME)//采矿完成
							{
								miningTimer = 0;
								this->unschedule(schedule_selector(Miner::miningTimerIncrease));

								//玩家行走到该采矿位置,计算最短路
								this->aStar->roadPoint.clear(); //清空的是astar对象的roadpoint
								this->openListClear();
								this->stopAllActions();
								//寻找采矿点和卸货点之间最短路
								this->pathFinding(miningPoint.x, miningPoint.y, unloadPoint.x, unloadPoint.y);
								//log("pathfingd");
								//向大本营行走
								this->runFollowShortestRoad();

								//改变状态,开始行走到大本营 
								curState = stateWalkToCamp;
								this->getStateSprite()->setTexture("miner_unload.png");
							}
			 

		}
		break;

		case stateWalkToCamp://行走至大本营
		{
			
								if (isReached) //如果走完了路程
								{
									isReached = false;
									//清理数据万万不能忘记
									this->aStar->roadPoint.clear(); //清空的是astar对象的roadpoint
									this->openListClear();
									this->stopAllActions();
									//寻找采矿点和卸货点之间最短路
									this->pathFinding(unloadPoint.x, unloadPoint.y, miningPoint.x, miningPoint.y);
									//开始行走
									this->runFollowShortestRoad();
									//改变状态
									curState = stateWalkToMine;
									this->getStateSprite()->setTexture("miner_readyToWork.png");

									//玩家金钱增加
									instance->setMoney(instance->getMoney() + MINING_GOLD);
									char temp[20];
									sprintf(temp,"%d", instance->getMoney());
									std::string str = temp;
									instance->moneyLabel->setString(str);
								}

		}
		break;

		case stateWalkToMine://行走至矿石处 
		{

								if (isReached)
								{
									//寻找最近的大本营
									searchNearestCamp();
									if (nearestCamp)
									{
										//寻找最近的卸货地点
										searchNearestUnloadPos(nearestCamp);
									}
									//开始采矿计时
									this->schedule(schedule_selector(Miner::miningTimerIncrease), 1);
									//log("mining point %f %f", miningPoint.x, miningPoint.y);
											
									//改变状态
									curState = stateMining;
									this->getStateSprite()->setTexture("miner_working.png");
									isReached = false;
								}

		}
		break;
	}
}


void Miner::setAnimation(int dir)//设置英雄的行走动画,表示英雄行走方向的int变量dir:0表示左上角，1表示正上方..........7表示正左方
{
	minerSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_leftup"))));
		break;
	case 1:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_up"))));
		break;
	case 2:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_rightup"))));
		break;
	case 3:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_right"))));
		break;
	case 4:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_rightdown"))));
		break;
	case 5:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_down"))));
		break;
	case 6:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_leftdown"))));
		break;
	case 7:
		minerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("miner_left"))));
		break;
	}
}
void Miner::stopMinerAction(int dir)//英雄停止时正确显示其站位
{
	minerSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		minerSprite->setTexture("miner/leftup_1.png");
		break;
	case 1:
		minerSprite->setTexture("miner/up_1.png");
		break;
	case 2:
		minerSprite->setTexture("miner/rightup_5.png");
		break;
	case 3:
		minerSprite->setTexture("miner/right_5.png");
		break;
	case 4:
		minerSprite->setTexture("miner/rightdown_5.png");
		break;
	case 5:
		minerSprite->setTexture("miner/down_1.png");
		break;
	case 6:
		minerSprite->setTexture("miner/leftdown_1.png");
		break;
	case 7:
		minerSprite->setTexture("miner/left_5.png");
		break;
	}
}

void Miner::runFollowShortestRoad()//沿着最短路行走
{
	//取消占地
	auto tempPos = this->getPosition();
	auto tempTilePos = tempPos / 32.0;
	instance->walkMap[(int)tempTilePos.x][MAP_SIZE - (int)tempTilePos.y] = 0;//取消占地

	//log("raod point %f %f", roadPoint[0].x, roadPoint[0].y);

	//递归调用该函数以达到不断行走的目的
	if (roadLength != 0)
	{
		//获取当前坐标和下一个坐标：用来判断玩家行走方向
		Point nowPoint = roadPoint[roadLength];
		roadLength--;
		Point nextPoint = roadPoint[roadLength];

		nowDir = getMinerDirection(nowPoint, nextPoint);

		if (prevDir != nowDir)
		{
			this->setAnimation(nowDir);
		}
		prevDir = nowDir;

		Point temp = Point(roadPoint[roadLength].x * 32, 80 * 32 - roadPoint[roadLength].y * 32);
		this->runAction(Sequence::create(MoveTo::create(0.4, temp)
			, CallFuncN::create(CC_CALLBACK_0(Miner::runFollowShortestRoad, this))
			, NULL));
	}
	else // 玩家已经到达目的地
	{
		//log("hereeeeeeeeeee");
		//heroDestPointer->setVisible(false);
		this->stopMinerAction(nowDir);
		//确定占地面积
		auto mapPos = this->getPosition();
		auto tilePos = mapPos / 32.0;
		//log("tile pos end %f %f", tilePos.x, tilePos.y);
		instance->walkMap[(int)tilePos.x][MAP_SIZE - (int)tilePos.y] = 1;//占地了,注意要用80减才行
		minerDestPointer->setVisible(false);
		
		isReached = true;//到达目的地
		
		return;
	}
}

int Miner::getMinerDirection(cocos2d::Point now, cocos2d::Point next) //获取士兵行走方向的函数
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
void Miner::pathFinding(int srcX, int srcY, int destX, int destY)
{
	//log("源目的地坐标：%d %d %d %d", srcX, srcY, destX, destY);
	aStar->AStar(srcX, srcY, destX, destY);
	isReachable = aStar->getIsReachable();//判断是否可达目的地
	//复制路径数组 和 最短路长度
	roadPoint = aStar->roadPoint;
	roadLength = roadPoint.size() - 1;
	//log("soldier road lenght %d", roadLength);


}

void Miner::openListClear()//清空openlist列表
{
	while (!aStar->open.empty()) //不能少！！
	{
		aStar->open.pop();
	}
}

void Miner::setTimeBarVisibleOrNot(bool is)//设置其属性条可见与否
{

	timeBar->setVisible(is);
	timeBarBg->setVisible(is);
}

void Miner::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Miner::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Miner::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Miner::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->minerSprite);
}

bool Miner::onTouchBegan(Touch* pTouch, Event* pEvent)
{

	return true;
}
void Miner::onTouchMoved(Touch* pTouch, Event* pEvent)
{

}
void Miner::onTouchEnded(Touch* pTouch, Event* pEvent)
{

}

void Miner::decreaseSurviveTime(float dt)//生存时间减少函数 1分钟后消失
{

	time--;
	timeBar->setPercentage(time);
	if (time <= 0)
	{

		//从2个容器里删除
		for (std::vector<Miner*>::iterator iter = instance->minerInRectVector.begin(); iter != instance->minerInRectVector.end();)
		{
			if (*iter == this)
				iter = instance->minerInRectVector.erase(iter);
			else
				iter++;
		}

		for (std::vector<Miner*>::iterator iter = instance->minerVector.begin(); iter != instance->minerVector.end();)
		{
			if (*iter == this)
				iter = instance->minerVector.erase(iter);
			else
				iter++;
		}

		minerDestPointer->removeFromParent();//别忘了去除指示器

		instance->setTotalMinerNum(instance->getTotalMinerNum()-1);//采矿者总数减少1

		this->removeFromParent();
	}

}
//搜寻最近的大本营
void Miner::searchNearestCamp()
{
	float distance = 100000;
	nearestCamp = nullptr;
	for (int i = 0; i < instance->campTowerVector.size(); i++)
	{
		auto tempCamp = instance->campTowerVector.at(i);
		auto tempCampPos = tempCamp->getPosition();
		if ((tempCampPos.x - this->getPosition().x)*(tempCampPos.x - this->getPosition().x) + (tempCampPos.y - this->getPosition().y)*(tempCampPos.y - this->getPosition().y)<distance*distance)
		{
			distance = (tempCampPos.x - this->getPosition().x)*(tempCampPos.x - this->getPosition().x) + (tempCampPos.y - this->getPosition().y)*(tempCampPos.y - this->getPosition().y);
			nearestCamp = tempCamp;
		}
	}
	
}

//找到离自己最近的空闲卸货位置
void Miner::searchNearestUnloadPos(CampTower* campTower)
{
	float distance = 10000;
	auto campTowerTilePos = convertToTileCoord(campTower->getPosition());
	campTower->updateVacantUnloadPos(campTowerTilePos);
	for (int i = 0; i < campTower->getVacantUnloadPos().size(); i++)
	{
		auto tempPos = campTower->getVacantUnloadPos().at(i);
		if ((tempPos.x - this->getPosition().x)*(tempPos.x - this->getPosition().x) + (tempPos.y - this->getPosition().y)*(tempPos.y - this->getPosition().y)< distance*distance)
		{
			distance = (tempPos.x - this->getPosition().x)*(tempPos.x - this->getPosition().x) + (tempPos.y - this->getPosition().y)*(tempPos.y - this->getPosition().y);
			unloadPoint = tempPos;
		}
	}

	//log("vacant unload size %d", campTower->getVacantUnloadPos().size());
}

Vec2 Miner::convertToTileCoord(Vec2 position)//将map坐标转化为tile坐标
{
	GameManager * instance = GameManager::getInstance();
	auto mapSize = instance->map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// 把position转换为瓦片坐标，确保得到的是整数
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}

void Miner::miningTimerIncrease(float dt)//采矿计时器函数
{
	miningTimer++;
}