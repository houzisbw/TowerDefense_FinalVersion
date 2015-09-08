#ifndef _BASEBULLET_H_
#define _BASEBULLET_H_
#include "cocos2d.h"
class BaseBullet : public cocos2d::Sprite
{
public:
	BaseBullet();
	virtual ~BaseBullet();
	virtual bool init() = 0;
	CC_SYNTHESIZE(int, damage, Damage);//子弹伤害
	CC_SYNTHESIZE(cocos2d::Sprite*, bulletSprite, BulletSprite);


};
#endif

