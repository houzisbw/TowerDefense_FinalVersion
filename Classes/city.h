#ifndef _CITY_H_
#define _CITY_H_
#include "cocos2d.h"
#include "GameManager.h"
class city : public cocos2d::Sprite
{
public:
	city();
	~city();
	virtual bool init();
	CREATE_FUNC(city);
	void addTouch();
	void addTouchOnEnterBg();//点击图片进入具体关卡
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	CC_SYNTHESIZE(bool, isSelected, IsSelected);//该城市是否被选中
	float dijkstra(int src, int dest);//寻找最短路算法
	void runFollowCity(int dest);
	CC_SYNTHESIZE(cocos2d::Label*, levelName, LevelName);//关卡名字
	void initLevelName(std::string name);//初始化关卡名字
private:
	cocos2d::Sprite * citySprite;
	cocos2d::EventListenerTouchOneByOne * listener;
	cocos2d::EventListenerTouchOneByOne * EnterBgListener;
	GameManager * instance;
	std::vector<cocos2d::Vec2> cityOnPath;//最短路上的城市
	int cityNum;
	cocos2d::Sprite * enterBg;//进入关卡的对话框
	cocos2d::Sprite * fight;
	

	
	
};
#endif

