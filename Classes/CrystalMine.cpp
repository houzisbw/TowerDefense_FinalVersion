#include "CrystalMine.h"
#include "GameManager.h"
USING_NS_CC;


CrystalMine::CrystalMine()
{
}


CrystalMine::~CrystalMine()
{
}

bool CrystalMine::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	//初始化
	crystalSprite = Sprite::create("mine.png"); //水晶矿图片
	//crystalSprite->setAnchorPoint(Point(0, 0));
	addChild(crystalSprite, 3);

	setCrystalMaxNum(100);
	setcrystalCurrentNum(100);
	//水晶矿进度条背景图片
	crystalBarSprite = Sprite::create("manaBarBg.png");
	crystalBarSprite->setPosition(crystalSprite->getPosition() + Point(0, 50));
	crystalBarSprite->setScale(0.4);
	addChild(crystalBarSprite, 10);
	//水晶矿存储量进度条初始化
	crystalBar = ProgressTimer::create(Sprite::create("soldierProduceTimeBar.png")); //参数是一个图片sprite
	crystalBar->setScaleX(2);
	crystalBar->setScaleY(5.2);
	crystalBar->setType(ProgressTimer::Type::BAR);
	crystalBar->setMidpoint(Point(0, 0.5f));
	crystalBar->setBarChangeRate(Point(1, 0));
	crystalBar->setPercentage(100);
	crystalBar->setPosition(Point(crystalBarSprite->getContentSize().width / 2, crystalBarSprite->getContentSize().height / 3 * 2 - 10));
	crystalBarSprite->addChild(crystalBar, 5);


	addTouch();

	//Point tempPos = convertTotileCoord(this->getPosition());
	//updateVacantMiningPos(tempPos);


	return true;
}

bool CrystalMine::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());
	// 获取当前点击点所在相对按钮的位置坐标   
	Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());          //一定要这样做
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// 点击范围判断检测
	if (rect.containsPoint(locationInNode))
	{
		crystalBarSprite->setVisible(true);
		crystalBar->setVisible(true);
		return true;
	}
	else
	{
		crystalBarSprite->setVisible(false);
		crystalBar->setVisible(false);
	}

	return true;
}

void CrystalMine::addTouch()//添加触摸事件
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CrystalMine::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->crystalSprite); //针对水晶矿设置侦听器
}

void CrystalMine::updateVacantMiningPos(Point minePos)//更新空闲的采矿位置，当点击农民采矿时，触发此函数,得到最新的空闲位置vector
{
	int centerX = minePos.x;
	int centerY = minePos.y;
	//清空vector
	vacantMiningPos.clear();
	//获取单例
	auto instance = GameManager::getInstance();
	//判断周围6个位置是否合法且空闲，如果是的话就加入vector
	//水晶矿上面一侧
	if (centerX - 1 >= 0 && centerY - 2 >= 0 && instance->walkMap[centerX - 1][centerY - 2] == 0)
	{
		auto tempPoint = Point(centerX - 1, centerY - 2);
		vacantMiningPos.push_back(tempPoint);
	}
	if (centerX  >= 0 && centerY - 2 >= 0 && instance->walkMap[centerX ][centerY - 2] == 0)
	{
		auto tempPoint = Point(centerX , centerY - 2);
		vacantMiningPos.push_back(tempPoint);
	}
	//水晶矿右侧
	if (centerX +1>= 0 && centerY  >= 0 && instance->walkMap[centerX+1][centerY] == 0)
	{
		auto tempPoint = Point(centerX+1, centerY);
		vacantMiningPos.push_back(tempPoint);
	}
	//水晶矿下侧
	if (centerX - 1 >= 0 && centerY + 2>= 0 && instance->walkMap[centerX -1][centerY+2] == 0)
	{
		auto tempPoint = Point(centerX - 1, centerY+2);
		vacantMiningPos.push_back(tempPoint);
	}
	if (centerX  >= 0 && centerY + 2 >= 0 && instance->walkMap[centerX ][centerY + 2] == 0)
	{
		auto tempPoint = Point(centerX , centerY + 2);
		vacantMiningPos.push_back(tempPoint);
	}
	//水晶矿左侧
	if (centerX-2 >= 0 && centerY  >= 0 && instance->walkMap[centerX-2][centerY ] == 0)
	{
		auto tempPoint = Point(centerX-2, centerY);
		vacantMiningPos.push_back(tempPoint);
	}

	//log("vacant size %d", vacantMiningPos.size());
	/*for (int i = 0; i < vacantMiningPos.size(); i++)
	{
		auto temp = vacantMiningPos.at(i);
		log("pos %f %f", temp.x, temp.y);
	}*/
}

Vec2 CrystalMine::convertTotileCoord(Vec2 position)  //将map坐标转化为tile坐标
{
	auto instance = GameManager::getInstance();

	auto mapSize = instance->map->getMapSize();
	// 计算当前缩放下，每块瓦片的长宽
	auto tileWidth = instance->map->getBoundingBox().size.width / instance->map->getMapSize().width;
	auto tileHeight = instance->map->getBoundingBox().size.height / instance->map->getMapSize().height;
	// 把position转换为瓦片坐标，确保得到的是整数
	int posX = position.x / tileWidth;
	int posY = instance->map->getMapSize().height - position.y / tileHeight;

	return Point(posX, posY);
}