#ifndef _ICETOWER_H_
#define _ICETOWER_H_
#include "cocos2d.h"
#include "BaseTower.h"
#include "GameManager.h"
class IceTower : public BaseTower
{
public:
	IceTower();
	~IceTower(); 
	virtual bool init();
	CREATE_FUNC(IceTower);
	void shoot(float dt); //真正发射子弹
	void checkPowerIsEnough(float dt);//检测电力是否足够
	CC_SYNTHESIZE(int, frozeTime, FrozeTime);//冰塔的冻结时间

	//取消或者回复调度射击函数
	void unscheduleShoot();
	void scheduleShoot(float rate);
private:
	cocos2d::Sprite * effectSprite;
	cocos2d::Vector<cocos2d::SpriteFrame*> effectSpriteVector;//闪电特效动画vector
	cocos2d::Animation * animationEffect;
	GameManager * instance;

	cocos2d::Node * exp;//爆炸特效node

};
#endif

