#include "city.h"
#include <set>
#include <queue>
#include <vector>
#include "HelloWorldScene.h"
#include "LoadLevelInfo.h"
using namespace std;
USING_NS_CC;

#define CITY_NUM 17

//行走地图的邻接矩阵
float mapAdj[CITY_NUM][CITY_NUM] = {

	0,   0,   0,   2.85, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   2.4,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   1.9,  2.1, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	2.85,2.4, 1.9,  0,   2.6, 0,   0,   0,   0,   0,   0,   0,   2.2, 0,   0,   0,   0,
	0,   0,   2.1, 2.6,  0,   0,   1.9, 0,   0,   0,   2.3, 0,   1.8, 0,   0,   0,   0,
	0,   0,   0,   0,    0,   0,   1.5, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,    1.9, 1.5, 0,   1.7, 0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,    0,   0,   1.7, 0,   2.2, 0,   3.5, 0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,    0,   0,   0,   2.2, 0,   1.8, 2.9, 0,   0,   0,   0,   0,   0,  
	0,   0,   0,   0,    0,   0,   0,   0,   1.8, 0,   2,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,    2.3, 0,   0,   3.5, 2.9, 2,   0,   0,   1.3, 0,   0,   0,   0,
	0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   2.7, 0,   0,   0,   0,
	0,   0,   0,   2.2,  1.8, 0,   0,   0,   0,   0,   1.3, 2.7, 0,   2.3, 0,   0,   0,
	0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   2.3, 0,   1.7, 2,   2.5,
	0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   1.7, 0,   2.2, 0,
	0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2.2, 0,   1.6,
	0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   2.5, 0,   1.6, 0


};

city::city()
{
}


city::~city()
{
}

bool city::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	citySprite = Sprite::create("city.png");
	citySprite->setScale(0.2);
	addChild(citySprite);
	setIsSelected(false);

	enterBg = Sprite::create("enterBg.png");
	//enterBg->setScale(0.3);
	enterBg->setPosition(citySprite->getPosition() + Point(0, citySprite->getBoundingBox().size.height / 2 + 40));
	addChild(enterBg, 2);
	enterBg->setVisible(false);

	levelName = Label::createWithTTF("", "b.ttf", 10);//显示不清楚
	enterBg->addChild(levelName, 2);
	levelName->setPosition(enterBg->getBoundingBox().size.width/2, 14);

	fight = Sprite::create("fight.png");
	fight->setScale(0.5);
	enterBg->addChild(fight, 3);
	fight->setPosition(enterBg->getBoundingBox().size.width / 2, enterBg->getBoundingBox().size.height / 2+10);

	instance = GameManager::getInstance();

	addTouch();
	addTouchOnEnterBg();

	return true;
}

void city::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create(); 
	listener->onTouchBegan = CC_CALLBACK_2(city::onTouchBegan, this);
	
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->citySprite);
}
bool city::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());

	// 获取当前点击点所在相对按钮的位置坐标   
	Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());          //一定要这样做
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// 点击范围判断检测
	if (rect.containsPoint(locationInNode))
	{
		//log("city touched");
		
		if (!instance->isWalking) //如果玩家不在行走过程，才能行走
		{

			//获取目的城市节点和玩家城市节点
			int destCity = this->getTag()-1;
			int srcCity = instance->player->getTag()-1;

			if (destCity != srcCity) //必须让2者不同
			{
				//隐藏说明面板
				instance->cityVector[srcCity]->enterBg->setVisible(false);
				//渐隐动画
				auto fo = FadeOut::create(1.0);
				auto seq = Sequence::create(fo, fo->reverse(), NULL);
				auto rep = RepeatForever::create(seq);
				this->citySprite->runAction(rep);
				instance->destCity = this;

				cityOnPath.clear();
				float distance = dijkstra(srcCity, destCity);
				//log("city%d ---- city%d : %f", srcCity + 1, destCity + 1, distance);

				cityNum = cityOnPath.size() - 1;	

				instance->cloud->setVisible(true);
				runFollowCity(destCity);
			}

			return true;
		}
	}
	return false;
}

float city::dijkstra(int src, int dest)
{

	bool visit[CITY_NUM];//每个节点是否访问过的数组
	float distance[CITY_NUM]; //每个节点距离源节点的距离
	int prev[CITY_NUM];//每个节点的前一个节点;
	set<int> s;
	for (int i = 0; i < CITY_NUM; i++)
	{
		visit[i] = false;
		distance[i] = 100000; //代替无穷大
		s.insert(i);
	}

	//初始化
	s.erase(src);
	distance[src] = 0;
	visit[src] = true;

	int temp = src;
	while (!s.empty())
	{
		
		//update过程
		for (int i = 0; i < CITY_NUM; i++)
		{
			if (mapAdj[temp][i] != 0 && visit[i]==false) //temp节点和i节点有线段相连 并且该点没访问过
			{
				if (distance[temp] + mapAdj[temp][i] < distance[i])
				{
					distance[i] = distance[temp] + mapAdj[temp][i];
					prev[i] = temp;//更新i节点的前级节点
				}
			}
		}

		//find min过程
		set<int>::iterator it;
		float max = 10000;
		int cursor; //要找出的节点(距离最小)
		for (it = s.begin(); it != s.end(); it++)
		{
			if (distance[*it] < max)
			{
				max = distance[*it];
				cursor = *it;
			}
		}

		visit[cursor] = true;
		temp = cursor;
		s.erase(cursor); //删去该节点，已经访问过了
	}

	//处理最短路
	int t = dest;
	cityOnPath.push_back(instance->cityPoint[t]);
	while (prev[t] != src)
	{
		t = prev[t];
		cityOnPath.push_back(instance->cityPoint[t]);
	}
	cityOnPath.push_back(instance->cityPoint[src]);  //此vector中存放了从目的节点到源节点的最短路  dest---------src

	//输出最短路城市坐标:从源输出至目的地
	//vector<Vec2>::iterator it_vec;
	//for (it_vec = cityOnPath.end() - 1; it_vec != cityOnPath.begin() - 1; it_vec--)
	//{
		//log("cityPos: %f %f", (*it_vec).x, (*it_vec).y);
	//}

	return distance[dest];

}


void city::runFollowCity(int dest)
{
	//递归调用该函数以达到不断行走的目的

	if (cityNum != 0)
	{
		
		instance->player->runAction(Sequence::create(MoveTo::create(1, cityOnPath[--cityNum])  //注意是--cityNum
			, CallFuncN::create(CC_CALLBACK_0(city::runFollowCity, this,dest))
			, NULL));
	}
	else // 玩家已经到达目的地城市
	{
		instance->destCity->citySprite->stopAllActions();
		instance->destCity->citySprite->setOpacity(255);
		instance->player->setTag(dest+1);  //注意tag表示城市的标号(1~CITY_NUM) 从1开始，不是从0开始，所以要加1
		instance->destCity->enterBg->setVisible(true);
		//log("retrurn");
		instance->cloud->setVisible(false);
		return;
	}

	
		
	
}

void city::initLevelName(std::string name)//初始化关卡名字
{
	levelName->setString(name);
}

void city::addTouchOnEnterBg()//点击图片进入具体关卡
{
	auto dispatcher = this->getEventDispatcher();
	EnterBgListener = EventListenerTouchOneByOne::create();
	EnterBgListener->onTouchBegan = [=](Touch* pTouch, Event* pEvent){
	
		auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());

		// 获取当前点击点所在相对按钮的位置坐标   
		Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());          //一定要这样做
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		// 点击范围判断检测 必须加visible判断才行
		if (rect.containsPoint(locationInNode) && target->isVisible())
		{
			int tag = this->getTag();
			//读入本关初始数据 ,暂时都是第一关
			auto info = LoadLevelInfo::createLoadLevelInfo("levelInfo/levelInfo_0.plist");
			info->readLevelInfo(); //在这个函数里面初始化了每一关的地图，敌人数据等，不用在helloWorld里再说明

			Director::getInstance()->replaceScene(TransitionFade::create(1.0, HelloWorld::createScene()));
			return true;
		}
		return false;

	};
	dispatcher->addEventListenerWithSceneGraphPriority(EnterBgListener, this->enterBg);
}