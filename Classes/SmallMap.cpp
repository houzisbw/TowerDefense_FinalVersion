#include "SmallMap.h"
#include "GameManager.h"
USING_NS_CC;

SmallMap::SmallMap()
{
}


SmallMap::~SmallMap()
{
}

bool SmallMap::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	map = Sprite::create("levelSmallMap/smallMap_1.png");
	map->setScale(1);
	addChild(map);

	window = Sprite::create("window.png");
	window->setAnchorPoint(Point(0, 0));
	map->addChild(window,2);

	//英雄头像
	heroIcon = Sprite::create("heroIcon.png");
	heroIcon->setScale(0.2);
	heroIcon->setPosition(100, 100);
	map->addChild(heroIcon);
	isHeroExisted = true;

	instance = GameManager::getInstance();

	//schedule(schedule_selector(SmallMap::updateTowerPosition, this),0.1);  //每隔0.3秒更新一次敌人和炮塔的位置

	return true;
}

Vec2 SmallMap::converToSmallMap(Vec2 pos)
{
	auto mapWidth = instance->map->getBoundingBox().size.width;
	auto mapHeight = instance->map->getBoundingBox().size.height;
	//log("map size : %f %f", mapWidth, mapHeight); 2560*2560  每一片瓦片32宽高
	auto scaleMapX = (float)pos.x / (float)mapWidth;
	auto scaleMapY = (float)pos.y / (float)mapHeight;
	//log("tower scale on map: %f %f", scaleMapX, scaleMapY);
	auto smallMapPosX = (float)(map->getBoundingBox().size.width * scaleMapX);
	auto smallMapPosY = (float)(map->getBoundingBox().size.height * scaleMapY);

	return Point(smallMapPosX, smallMapPosY);
}

void SmallMap::updateTowerPosition(float dt)
{
	//log("towerVector %d    greenVector %d", instance->towerVector.size(), greenVector.size());
	//log("enemyVector %d    redVector %d", instance->enemyVector.size(), redVector.size());
	//log("  ");

	//处理绿色炮塔方块
	for (int i = 0; i < instance->towerVector.size(); i++) //greenVector.size()
	{
		auto tempTower = instance->towerVector.at(i);
		auto tempPos = tempTower->getPosition();      //得到的pos是大地图上的pos
		auto smallMapPos = converToSmallMap(tempPos); //定时获得炮塔在小地图上的位置
		auto tempSmallMapGreen = greenVector.at(i);
		tempSmallMapGreen->setPosition(smallMapPos);
	}
	//处理红色敌人方块
	for (int i = 0; i < instance->enemyVector.size(); i++) //这里用enemyVector就可以了
	{
		auto tempEnemy = instance->enemyVector.at(i);
		auto tempPos = tempEnemy->getPosition();
		auto smallMapPos = converToSmallMap(tempPos);
		auto tempSmallMapRed = redVector.at(i);
		tempSmallMapRed->setPosition(smallMapPos);

	}
	//处理白色window:手机屏幕在大地图中的位置
	window->setPosition(windowPoint);
	//处理英雄头像的位置
	if (instance->hero)
	{
		auto tempHeroPos = instance->hero->getPosition();
		auto smallMapHeroPos = converToSmallMap(tempHeroPos);
		heroIcon->setPosition(smallMapHeroPos);
	}
	else //删除小地图上的英雄头像
	{
		if (isHeroExisted)//防止重复删除
		{
			heroIcon->removeFromParent();
			isHeroExisted = false;
		}
	}

}
