#include "IceBullet.h"
USING_NS_CC;

IceBullet::IceBullet()
{
}


IceBullet::~IceBullet()
{
}

bool IceBullet::init()
{
	if (!Sprite::init()) //如果忘记了这句话则会在runApplication报错
	{
		return false;
	}
	instance = GameManager::getInstance();

	setDamage(0);
	bulletSprite = Sprite::create("bullet/IceBullet.png");
	bulletSprite->setScale(0.8);
	addChild(bulletSprite, 2);

	return true;
}
