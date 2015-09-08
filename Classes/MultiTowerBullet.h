#ifndef _MULTITOWERBULLET_H_
#define _MULTITOWERBULLET_H_
#include "BaseBullet.h"
#include "cocos2d.h"
#include "GameManager.h"
class MultiTowerBullet :
	public BaseBullet
{
public:
	MultiTowerBullet();
	~MultiTowerBullet();
	virtual bool init();
	CREATE_FUNC(MultiTowerBullet);
private:
	GameManager * instance;

};
#endif

