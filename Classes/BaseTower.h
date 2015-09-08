#ifndef _BASETOWER_H_
#define _BASETOWER_H_
#include "cocos2d.h"
#include "EnemyBase.h"
//struct TowerCornerPoint   //tower图片的四个角坐标
//{
//	cocos2d::Point leftUp;
//	cocos2d::Point rightUp;
//	cocos2d::Point rightDown;
//	cocos2d::Point leftDown;
//};
class BaseTower : public cocos2d::Sprite
{
public:
	BaseTower();
	virtual ~BaseTower();
	virtual bool init() = 0;
	void checkNearestEnemy(float dt);
	virtual void shoot(float dt) = 0;

	CC_SYNTHESIZE(float, scope, Scope); //塔的射程
	CC_SYNTHESIZE(int, attack, Attack);//塔的攻击力
	CC_SYNTHESIZE(float, rate, Rate);//塔的设计速率
	CC_SYNTHESIZE(int, grade, Grade);//塔的等级
	CC_SYNTHESIZE(int, power, Power);//塔的消耗电力
	CC_SYNTHESIZE(bool, isPowerEnough, IsPowerEnough);//电力是否足够
	CC_SYNTHESIZE(bool, isPowerConsumption, IsPowerConsumption);//该塔是否耗电
	CC_SYNTHESIZE(int, towerType, TowerType);//塔的种类,从0开始递增

	CC_SYNTHESIZE(int, towerSequence, TowerSequence);//塔的序号，从1开始增加的整数

	EnemyBase * nearestEnemy;//最近的敌人
	EnemyBase * getNearestEnemy(); //获取敌人

	CC_SYNTHESIZE(cocos2d::Size, towerSize, TowerSize); //塔的尺寸：长 * 高 以tiledmap的格子为单位，也就是占地大小
	CC_SYNTHESIZE(bool, isSelected, IsSelected);//是否选中该炮塔
	CC_SYNTHESIZE(bool, move, Move);//是否选中该炮塔

	cocos2d::Sprite * towerSprite;//炮塔图片
	cocos2d::Sprite * gradeSprite;//等级标志
	cocos2d::Sprite * noPowerSprite;//耗电炮塔没电标志
	cocos2d::Sprite * noAttackSprite;//炮塔被敌人沉默无法攻击的图片

	cocos2d::Sprite * towerHpSprite;//炮塔血量
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, towerHp, TowerHp);//炮塔血量进度条
	CC_SYNTHESIZE(int, hp, Hp);//炮塔当前血量
	CC_SYNTHESIZE(int, maxHp, MaxHp);//炮塔最大血量

	virtual void addTouch();//添加触摸事件

	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	void moveTower(cocos2d::Vec2 pos);
	void addBuildIcon();
	bool isIntersect();//是否和其他建筑物或者地形有交集
	//坐标转换公式:将屏幕坐标转换为tilemap坐标
	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position);
	//获得towerSPrite4个角的map坐标，便于计算占地面积
	//TowerCornerPoint getTowerSpriteCornerPoint();

	//判断炮塔是否处于任何一个沉默敌人的范围内
	void isTowerInAnyDiableEnemyRange(float dt);
	//炮塔是否被沉默
	CC_SYNTHESIZE(bool, isSilenced, IsSilenced);

private:	
	cocos2d::Sprite * buildIcon;
	cocos2d::Vec2 OriginPos;
	bool canPutDown;//是否可以放下炮塔
protected:
	cocos2d::EventListenerTouchOneByOne *  listener;
	
};
#endif

