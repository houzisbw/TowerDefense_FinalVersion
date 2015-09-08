#ifndef _LEVELSELECTSCENE_H_
#define _LEVELSELECTSCENE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "city.h"
#include "GameManager.h"
#include "Hero.h"
class LevelSelectScene : public cocos2d::Layer
{
public:
	LevelSelectScene();
	~LevelSelectScene();
	virtual bool init();
	CREATE_FUNC(LevelSelectScene);
	static cocos2d::Scene * createScene();
	void addCity();
	bool checkWetherPlayerIsArrived();
	void update(float dt);
private:
	cocos2d::ui::Button* backButton;

	cocos2d::ui::ScrollView * scrollMap;
	cocos2d::ui::ImageView  * walkingMap;
	cocos2d::Sprite * player;
	cocos2d::Sprite * cloud;
	cocos2d::Vector<cocos2d::SpriteFrame*> playerVec;
	cocos2d::ui::ImageView * title;
	city * city1; //每个城市的节点
	city * city2; //每个城市的节点
	city * city3; //每个城市的节点
	city * city4; //每个城市的节点
	city * city5; //每个城市的节点

	city * city6; //每个城市的节点
	city * city7; //每个城市的节点
	city * city8; //每个城市的节点
	city * city9; //每个城市的节点
	city * city10; //每个城市的节点

	city * city11; //每个城市的节点
	city * city12; //每个城市的节点
	city * city13; //每个城市的节点
	city * city14; //每个城市的节点
	city * city15; //每个城市的节点
	city * city16; //每个城市的节点
	city * city17; //每个城市的节点

	cocos2d::Sprite * bgMap;
	GameManager * instance;
	std::vector<cocos2d::Vec2> cityPoint;

	Hero * hero;
	

	

};
#endif

