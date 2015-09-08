#ifndef _CRYSTALMINE_H_
#define _CRYSTALMINE_H_
#include "cocos2d.h"
class CrystalMine : public cocos2d::Sprite
{
public:
	CrystalMine();
	~CrystalMine();
	virtual bool init();
	CREATE_FUNC(CrystalMine);

	CC_SYNTHESIZE(int, crystalMaxNum, CrystalMaxNum);//水晶矿最大数量
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, crystalBar, CrystalBar);//水晶矿数量进度条
	CC_SYNTHESIZE(int, crystalCurrentNum, crystalCurrentNum);//水晶矿当前数量

	CC_SYNTHESIZE(std::vector<cocos2d::Point> , vacantMiningPos,VacantMiningPos); //每一个水晶矿都有一个vector，里面存储了空闲的采矿位置
	void updateVacantMiningPos(cocos2d::Point minePos);//更新空闲的采矿位置，当点击农民采矿时，触发此函数 ,参数表示自己在map中的位置
	cocos2d::Vec2 convertTotileCoord(cocos2d::Vec2 position);  //将map坐标转化为tile坐标

	virtual void addTouch();//添加触摸事件
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	//virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	//virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
private:
	cocos2d::Sprite * crystalSprite;//水晶矿图片
	cocos2d::Sprite * crystalBarSprite;//水晶矿进度条背景图片
protected:
	cocos2d::EventListenerTouchOneByOne *  listener; //触摸侦听器

};
#endif

