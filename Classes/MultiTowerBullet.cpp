#include "MultiTowerBullet.h"
USING_NS_CC;

MultiTowerBullet::MultiTowerBullet()
{
}


MultiTowerBullet::~MultiTowerBullet()
{
}

bool MultiTowerBullet::init()
{

	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}

	srand(time(0));
	instance = GameManager::getInstance();

	char temp[40];
	int color = rand() % 6 + 1;//获取1-6的随机数
	sprintf(temp, "bullet/multiTowerBullet_%d.png", color);
	std::string str = temp;

	bulletSprite = Sprite::create(str);
	bulletSprite->setTag(1);//攻击力1 

	bulletSprite->setName("multiTowerBullet");

	bulletSprite->setScale(1);
	addChild(bulletSprite, 2);

	return true;
}