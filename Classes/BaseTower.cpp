#include "BaseTower.h"
#include "GameManager.h"
#include "Soldier.h"
#include "CampTower.h"
USING_NS_CC;

// 拖动过程中，选中项正常的纹理图，这个正常是能被添加在拖动位置
const char* move_texture[10] =
{
	"towerItem/moveItem1.png", "towerItem/moveItem2.png", "towerItem/moveItem3.png", "towerItem/moveItem4.png", "towerItem/moveItem5.png", "towerItem/moveItem6.png", "towerItem/moveItem7.png", "towerItem/moveItem8.png", "towerItem/moveItem9.png", "towerItem/moveItem2.png"
};

// 拖动过程中，选中项不正常的纹理图
const char* move_texture_en[10] =
{
	"towerItemDisable/Item1.png", "towerItemDisable/Item2.png", "towerItemDisable/Item3.png", "towerItemDisable/Item4.png", "towerItemDisable/Item5.png", "towerItemDisable/Item6.png", "towerItemDisable/Item7.png", "towerItemDisable/Item8.png", "towerItemDisable/Item9.png", "towerItemDisable/Item2.png"
};
//炮塔移动过程中勾差的大小系数
float moveScale[10] =
{
	0.3, 0.3, 0.5, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3
};


const int ROAD = 18;

BaseTower::BaseTower():
canPutDown(true)
{
	schedule(schedule_selector(BaseTower::isTowerInAnyDiableEnemyRange), 0.2);
}


BaseTower::~BaseTower()
{
}

void BaseTower::checkNearestEnemy(float dt)
{
	GameManager* instance = GameManager::getInstance();
	auto currMinDistance = this->scope;
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

EnemyBase* BaseTower::getNearestEnemy()
{
	return nearestEnemy;

}

bool BaseTower::isIntersect()
{
	GameManager * instance = GameManager::getInstance();
	bool isIntersect = false;
	
	//和炮塔比较
	for (int i = 0; i < instance->towerVector.size(); i++)
	{
		auto tempTower = instance->towerVector.at(i);
		if (tempTower != this) //不能和自己比较，我勒个去，想了半天,因为自己这个炮塔也在vector中
		{
			auto towerMapPos = instance->map->convertToNodeSpaceAR(this->getPosition());
			//log("towerMapPos %f %f", towerMapPos.x, towerMapPos.y);
			auto towerRect = Rect{ towerMapPos.x, towerMapPos.y, this->towerSprite->getBoundingBox().size.width, this->towerSprite->getBoundingBox().size.height };
			//log("rect size : %f %f", this->towerSprite->getBoundingBox().size.width, this->towerSprite->getBoundingBox().size.height);
			//log("tempTowerPos %f %f", tempTower->getPosition().x, tempTower->getPosition().y);
			auto tempTowerRect = Rect{ tempTower->getPosition().x, tempTower->getPosition().y, tempTower->towerSprite->getBoundingBox().size.width, tempTower->towerSprite->getBoundingBox().size.height };
			if (tempTowerRect.intersectsRect(towerRect))
			{
				//log("true");
				isIntersect = true;
				break;
			}
		}
	}
	//和士兵比较
	for (int i = 0; i < instance->soldierVector.size(); i++)
	{
		auto tempSoldier = instance->soldierVector.at(i);	

		auto towerMapPos = instance->map->convertToNodeSpaceAR(this->getPosition());
		//log("towerMapPos %f %f", towerMapPos.x, towerMapPos.y);
		auto towerRect = Rect{ towerMapPos.x, towerMapPos.y, this->towerSprite->getBoundingBox().size.width, this->towerSprite->getBoundingBox().size.height };
		//log("rect size : %f %f", this->towerSprite->getBoundingBox().size.width, this->towerSprite->getBoundingBox().size.height);
		//log("tempTowerPos %f %f", tempTower->getPosition().x, tempTower->getPosition().y);
		auto tempTowerRect = Rect{ tempSoldier->getPosition().x, tempSoldier->getPosition().y, tempSoldier->soldierSprite->getBoundingBox().size.width, tempSoldier->soldierSprite->getBoundingBox().size.height };
		if (tempTowerRect.intersectsRect(towerRect))
		{
			//log("true");
			isIntersect = true;
			break;
		}
		
	}


	return isIntersect;
}

void BaseTower::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(BaseTower::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(BaseTower::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(BaseTower::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->towerSprite);
}

void BaseTower::moveTower(Vec2 pos)
{
	//具体的移动
	this->setPosition(pos);
	//是否可以放置炮塔的位置check
	int type = this->getTowerType();


	bool is = isIntersect();

	GameManager * instance = GameManager::getInstance();
	//得到map的大小：以格子数计算
	auto mapSize = instance->map->getMapSize();
	//log("mapSize : %f %f", mapSize.width, mapSize.height);
	// 将position转化为地图坐标
	auto tilePos = this->convertTotileCoord(pos-Point(0,this->towerSprite->getBoundingBox().size.height/2)+Point(0,16)); //加上地图格子的一半长度，不是全部长度
	//log("tilePos : %f %f", tilePos.x, tilePos.y);
	// canBliud是用于判断是否可生成瓦片的变量

	int gid = instance->map->getLayer("bg")->getTileGIDAt(tilePos);
	auto p = instance->map->getPropertiesForGID(gid);

	if (!is && p.isNull()) //无交集，可以建造
	{

		this->towerSprite->setTexture(move_texture[type]);
		buildIcon->setTexture("canBuild.png");
		canPutDown = true;

	}
	else //有交集，显示红色炮塔
	{
		this->towerSprite->setTexture(move_texture_en[type]);
		buildIcon->setTexture("cannotBuild.png");
		canPutDown = false;

	}


}

void BaseTower::addBuildIcon()
{
	buildIcon = Sprite::create("canBuild.png");
	buildIcon->setScale(moveScale[towerType]);
	buildIcon->setPosition(this->towerSprite->getPosition() + Point(0, this->towerSprite->getBoundingBox().size.height / 2) + (towerType==8?Point(0, 80):Point(0,40))); //必须是towersprite的position,大本营要特殊处理
	this->addChild(buildIcon, 30);
}

bool BaseTower::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	OriginPos = this->getPosition();
	canPutDown = true;

	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());
	// 获取当前点击点所在相对按钮的位置坐标   
	Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());          //一定要这样做
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// 点击范围判断检测
	if (rect.containsPoint(locationInNode))
	{
		towerHpSprite->setVisible(true);
		towerHp->setVisible(true);
		return true;
	}
	else
	{
		towerHpSprite->setVisible(false);
		towerHp->setVisible(false);
	}
	
	return true;
}

void  BaseTower::onTouchMoved(Touch* pTouch, Event* pEvent)
{

	//停止炮塔的渐隐动画
	this->towerSprite->stopAllActions();
	this->towerSprite->setOpacity(255);
	Vec2 pos = this->getParent()->convertToNodeSpaceAR(pTouch->getLocation()); //map是parent
	//如果被选中,哨戒炮不能移动
	if (isSelected)
	{
		if (towerType == 8)
		{
			dynamic_cast<CampTower*>(this)->getProduceTimeBarBg()->setVisible(false);
			dynamic_cast<CampTower*>(this)->getTimeBar()->setVisible(false);
		}
		//血条隐藏
		this->towerHpSprite->setVisible(false);
		this->getTowerHp()->setVisible(false);
		//这里还需要误差处理
		listener->setSwallowTouches(true);//只移动炮塔,不移动地图
		moveTower(pos);
	}
}

void  BaseTower::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	if (isSelected)
	{
		this->setMove(false);
		buildIcon->removeFromParent();
		//生产条显现
		if (towerType == 8)
		{
			dynamic_cast<CampTower*>(this)->getProduceTimeBarBg()->setVisible(true);
			dynamic_cast<CampTower*>(this)->getTimeBar()->setVisible(true);
		}	
		//血条显现
		this->towerHpSprite->setVisible(true);
		this->getTowerHp()->setVisible(true);
		//如果不能放下，则炮塔回到原位
		if (!canPutDown)
		{
			this->setPosition(OriginPos);
			int tag = this->getTag();
			this->towerSprite->setTexture(move_texture[tag]);
		}
		else //移动点数减少1
		{
			GameManager* instance = GameManager::getInstance();
			instance->setMovePoint(instance->getMovePoint() - 1);

			char m1[20];
			sprintf(m1, "%d", instance->getMovePoint());
			std::string mStr1 = m1;
			instance->movePointLabel->setString(mStr1);
		}
	}
	
	isSelected = false;
	listener->setSwallowTouches(false);

}

//坐标转换公式:将屏幕坐标转换为tilemap坐标
Vec2 BaseTower::convertTotileCoord(Vec2 position)
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

//判断炮塔是否处于任何一个沉默敌人的范围内
void BaseTower::isTowerInAnyDiableEnemyRange(float dt)
{
	
	
	GameManager* instance = GameManager::getInstance();
	bool isInRange = false;
	for (int i = 0; i < instance->enemyVector.size(); i++)
	{
		auto enemy = instance->enemyVector.at(i);
		int type = enemy->getType();
		bool isStopped = enemy->getIsStopped();
		if (type == 3 && !isStopped)//如果敌人是沉默炮塔的敌人 并且 没有被禁锢
		{
			auto pos = enemy->getPosition();
			if ((pos.x - this->getPosition().x)*(pos.x - this->getPosition().x) + (pos.y - this->getPosition().y)*(pos.y - this->getPosition().y) <= 300 * 300)
			{
				isInRange = true;
				break;
			}
		}
	}
	int towerType = this->getTowerType();
	if (towerType < 5 && towerType!=3) //只有炮塔可以显示沉默图片
	{

		if (isInRange) //被沉默了
		{
			this->noAttackSprite->setVisible(true);
			isSilenced = true;
		}
		else
		{
			this->noAttackSprite->setVisible(false);
			isSilenced = false;
		}
	}

}

