#include "Hero.h"
USING_NS_CC;
#define FRAME_NUM 3
#define FRAME_WALK 8
//#include "HelloWorldScene.h"
#include "GameManager.h"

Hero::Hero()
{
}


Hero::~Hero()
{
}

bool Hero::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	heroSprite = Sprite::create("Hero/hero_down_1.png");
	addChild(heroSprite);

	/*ring = Sprite::create("attackRange_noEnemy.png");
	ring->setScale(0.3);
	ring->setVisible(false);
	addChild(ring,2);*/

	mana = 100;//初始化魔法值为100
	ability2Range = 210;//技能2初始化范围

	manaLabel = Label::createWithTTF("100/100", "HANGTHEDJ.ttf", 20);
	manaLabel->setPosition(heroSprite->getPosition() + Point(0, 95));
	addChild(manaLabel, 11);

	manaBarBg = Sprite::create("manaBarBg.png");
	manaBarBg->setScale(0.45);
	manaBarBg->setPosition(heroSprite->getPosition() + Point(0, 70));
	addChild(manaBarBg, 11);


	//英雄蓝条初始化
	manaBar = ProgressTimer::create(Sprite::create("magic.png"));
	manaBar->setScaleX(0.9);
	manaBar->setScaleY(2.2);
	manaBar->setType(ProgressTimer::Type::BAR);
	manaBar->setMidpoint(Point(0, 0.5));
	manaBar->setBarChangeRate(Point(1, 0));
	manaBar->setPercentage(100);
	manaBar->setAnchorPoint(Point(0, 0.5));
	manaBar->setPosition(manaBarBg->getPosition()-Point(manaBarBg->getBoundingBox().size.width/2-5,0));
	addChild(manaBar, 10);



	//初始化英雄走路的动画:向下走
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_down_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkDown.pushBack(frame);
	}
	animationWalkDown = Animation::createWithSpriteFrames(walkDown, 0.1);

	//初始化英雄走路的动画:向上走
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_up_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkUp.pushBack(frame);
	}
	animationWalkUp = Animation::createWithSpriteFrames(walkUp, 0.1);

	//初始化英雄走路的动画:向左
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_left_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeft.pushBack(frame);
	}
	animationWalkLeft = Animation::createWithSpriteFrames(walkLeft, 0.1);

	//初始化英雄走路的动画:向右
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_right_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRight.pushBack(frame);
	}
	animationWalkRight = Animation::createWithSpriteFrames(walkRight, 0.1);
	

	//初始化英雄走路的动画:向左上
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_leftup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeftUp.pushBack(frame);
	}
	animationWalkLeftUp = Animation::createWithSpriteFrames(walkLeftUp, 0.1);

	//初始化英雄走路的动画:向左下
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_leftdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkLeftDown.pushBack(frame);
	}
	animationWalkLeftDown = Animation::createWithSpriteFrames(walkLeftDown, 0.1);

	//初始化英雄走路的动画:向右上
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_rightup_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRightUp.pushBack(frame);
	}
	animationWalkRightUp = Animation::createWithSpriteFrames(walkRightUp, 0.1);

	//初始化英雄走路的动画:向右下
	for (int i = 1; i <= FRAME_WALK; i++)
	{
		char buf[30];
		sprintf(buf, "Hero/hero_rightdown_%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 50, 100 }); //直接从res里取出
		walkRightDown.pushBack(frame);
	}
	animationWalkRightDown = Animation::createWithSpriteFrames(walkRightDown, 0.1);

	
	//将animation加入到cache中去,必须将animation加入到cache中去，用的时候取出来
	AnimationCache::getInstance()->addAnimation(animationWalkDown, "down");
	AnimationCache::getInstance()->addAnimation(animationWalkUp, "up");
	AnimationCache::getInstance()->addAnimation(animationWalkLeft, "left");
	AnimationCache::getInstance()->addAnimation(animationWalkRight, "right");

	AnimationCache::getInstance()->addAnimation(animationWalkLeftUp, "leftup");
	AnimationCache::getInstance()->addAnimation(animationWalkLeftDown, "leftdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightDown, "rightdown");
	AnimationCache::getInstance()->addAnimation(animationWalkRightUp, "rightup");

	//添加触摸事件
	addTouch();
	//添加回蓝函数
	schedule(schedule_selector(Hero::recoverMana), 1);


	return true;
}

void Hero::runRingAction()
{
	/*ring->setVisible(true);
	auto rb = RotateBy::create(1,90);
	auto rep = RepeatForever::create(rb);
	ring->runAction(rep);*/
}
void Hero::stopRingAction()
{
	/*ring->setVisible(false);
	ring->stopAllActions();*/
}

void Hero::setAnimation(int dir)
{
	heroSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("leftup")))); 
		break;
	case 1:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("up"))));
		break;
	case 2:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("rightup"))));
		break;
	case 3:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("right"))));
		break;
	case 4:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("rightdown"))));
		break;
	case 5:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("down"))));
		break;
	case 6:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("leftdown"))));
		break;
	case 7:
		heroSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("left"))));
		break;
	}

}

void Hero::stopHeroAction(int dir) 
{
	heroSprite->stopAllActions();
	switch (dir)
	{
	case 0:
		heroSprite->setTexture("Hero/hero_leftup_1.png");
		break;
	case 1:
		heroSprite->setTexture("Hero/hero_up_1.png");
		break;
	case 2:
		heroSprite->setTexture("Hero/hero_rightup_5.png");
		break;
	case 3:
		heroSprite->setTexture("Hero/hero_right_5.png");
		break;
	case 4:
		heroSprite->setTexture("Hero/hero_rightdown_5.png");
		break;
	case 5:
		heroSprite->setTexture("Hero/hero_down_1.png");
		break;
	case 6:
		heroSprite->setTexture("Hero/hero_leftdown_1.png");
		break;
	case 7:
		heroSprite->setTexture("Hero/hero_left_5.png");
		break;
	}
}
bool Hero::onTouchBegan(Touch* pTouch, Event* pEvent) //点击英雄弹出面板
{
	GameManager* instance = GameManager::getInstance();

	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());

	// 获取当前点击点所在相对按钮的位置坐标   
	Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());          //一定要这样做
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	log("%f %f", s.width, s.height);

	// 点击范围判断检测
	if (rect.containsPoint(locationInNode))
	{
		instance->heroPanel->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
		instance->heroPanel->setOpacity(0);
		//动画效果
		auto fi = FadeIn::create(0.5);
		instance->heroPanel->runAction(fi);
		return true;
	}
	return false;

	

}
void Hero::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Hero::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->heroSprite);
}

void Hero::recoverMana(float dt)//回蓝函数
{
	if (mana < 100)
	{
		mana += 1;
		manaBar->setPercentage(mana);

		//实时改变魔法值文本显示
		char t1[10];
		sprintf(t1, "%d/100", mana);
		std::string str1 = t1;

		manaLabel->setString(str1);

	}
}


