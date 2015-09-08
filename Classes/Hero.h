#ifndef _HERO_H_
#define _HERO_H_
#include "cocos2d.h"

#define MAX_MANA 100


class Hero : public cocos2d::Sprite
{
public:
	Hero();
	~Hero();
	virtual bool init();
	CREATE_FUNC(Hero);
	void runRingAction();
	void stopRingAction();
	void setAnimation(int dir);//设置英雄的行走动画,表示英雄行走方向的int变量dir:0表示左上角，1表示正上方..........7表示正左方
	void stopHeroAction(int dir); //英雄停止时正确显示其站位

	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent); //点击英雄弹出面板
	void addTouch();

	CC_SYNTHESIZE(int, mana, Mana);//魔法值
	CC_SYNTHESIZE(cocos2d::ProgressTimer*, manaBar, ManaBar);
	CC_SYNTHESIZE(cocos2d::Label*, manaLabel, ManaLabel);//英雄魔法文本

	void recoverMana(float dt);//回蓝函数

	int ability2Range;
	
private:
	cocos2d::Sprite * heroSprite;
	//cocos2d::Vector<cocos2d::SpriteFrame*> animFrams; //每一帧动作图片的vector
	cocos2d::Sprite * ring;//选中英雄时产生的光圈

	cocos2d::Vector<cocos2d::SpriteFrame*> walkDown;//向下走的动画vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkUp;//向上走的动画vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeft;//向左走的动画vector
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRight;//向左走的动画vector

	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeftUp;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkLeftDown;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRightUp;
	cocos2d::Vector<cocos2d::SpriteFrame*> walkRightDown;

	cocos2d::Animation * animationWalkDown;
	cocos2d::Animation * animationWalkUp;
	cocos2d::Animation * animationWalkLeft;
	cocos2d::Animation * animationWalkRight;

	cocos2d::Animation * animationWalkLeftUp;
	cocos2d::Animation * animationWalkRightUp;
	cocos2d::Animation * animationWalkLeftDown;
	cocos2d::Animation * animationWalkRightDown;

	
	//cocos2d::ProgressTimer * manaBar;//魔力值条
	cocos2d::Sprite * manaBarBg;//魔力值条的背景

	cocos2d::Size visibleSize;
	cocos2d::EventListenerTouchOneByOne * listener;

	
};

#endif

