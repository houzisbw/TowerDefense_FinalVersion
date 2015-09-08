#ifndef _ARTILLERY_H_
#define _ARTILLERY_H_
#include "BaseTower.h"
#include "cocos2d.h"
class ArtilleryTower :
	public BaseTower
{
public:
	ArtilleryTower();
	~ArtilleryTower();
	virtual bool init();
	CREATE_FUNC(ArtilleryTower);
public:
	void rotateAndShoot(float dt);
	void shoot(float dt); //真正发射子弹
	void removeBullet(cocos2d::Node* pSender);//销毁子弹，有参数注意
	cocos2d::Sprite*  ArtilleryTowerBullet(); //创造子弹，不用单独一个类
private:
	cocos2d::Sprite * basePlate;
	cocos2d::Sprite * rotateArtillery;
	cocos2d::Vector<cocos2d::SpriteFrame*> bulletAnimationVec;
	
};
#endif