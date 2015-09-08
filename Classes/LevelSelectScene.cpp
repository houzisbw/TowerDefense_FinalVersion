#include "LevelSelectScene.h"
#include "WelcomeScene.h"
#include "LoadLevelInfo.h"
USING_NS_CC;
using namespace cocostudio::timeline;



LevelSelectScene::LevelSelectScene()
{
}


LevelSelectScene::~LevelSelectScene()
{
}

bool LevelSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	instance = GameManager::getInstance();

	//初始化玩家
	player = Sprite::create("22-w.png");
	player->setScale(0.5);

	cloud = Sprite::create("cloud.png");
	player->addChild(cloud);
	cloud->setPosition(player->getBoundingBox().size.width / 2+30, 0);
	cloud->setVisible(false);
	instance->cloud = cloud;

	//玩家动画
	for (int i = 1; i <= 4; i++)
	{
		char buf[30];
		sprintf(buf, "22-w%d.png", i);
		std::string str = buf;
		auto frame = SpriteFrame::create(str, Rect{ 0, 0, 150, 150 }); //直接从res里取出
		playerVec.pushBack(frame);
	}
	Animation * animation = Animation::createWithSpriteFrames(playerVec, 0.3);
	Animate * animate = Animate::create(animation);
	player->runAction(RepeatForever::create(animate));


	instance->clear(); //防止其中vector重复添加
	instance->player = player;

	auto rootNode = CSLoader::createNode("LevelSelectScene.csb");
	scrollMap = rootNode->getChildByName<ui::ScrollView*>("ScrollView_Bg");
	walkingMap = dynamic_cast<ui::ImageView*>(scrollMap->getChildByName("Image")); //要把强制转换放前面才行
	addChild(rootNode);
	walkingMap->addChild(player,5);

	backButton = rootNode->getChildByName<ui::Button*>("Button_back");
	backButton->addClickEventListener([=](Ref * ref){
		Director::getInstance()->replaceScene(TransitionFade::create(1,WelcomeScene::createScene()));
	
	});



	//log("map size %f %f", walkingMap->getContentSize().width, walkingMap->getContentSize().height);
	//log("map bounding box size %f %f", walkingMap->getBoundingBox().size.width, walkingMap->getBoundingBox().size.height);

	addCity(); //添加城市
	player->setPosition(city1->getPosition()); //设置玩家初始位置
	player->setTag(1);//表示玩家现在正处于哪个城市
	
	scheduleUpdate();

	return true;
}

Scene * LevelSelectScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelSelectScene::create();
	scene->addChild(layer);
	return scene;
}

void LevelSelectScene::addCity()
{
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	city1 = city::create();
	city1->setPosition(250, 50);
	walkingMap->addChild(city1);
	instance->cityPoint.push_back(city1->getPosition());
	cityPoint.push_back(city1->getPosition());
	instance->cityVector.push_back(city1);
	city1->setTag(1);
	const char *str_1 = ((String*)dic->objectForKey("levelName1"))->_string.c_str();
	std::string str1 = str_1;
	city1->initLevelName(str1);

	city2 = city::create();
	city2->setPosition(102, 300);
	walkingMap->addChild(city2);
	instance->cityPoint.push_back(city2->getPosition());
	cityPoint.push_back(city2->getPosition());
	instance->cityVector.push_back(city2);
	city2->setTag(2);
	const char *str_2 = ((String*)dic->objectForKey("levelName2"))->_string.c_str();
	std::string str2 = str_2;
	city2->initLevelName(str2);

	city3 = city::create();
	city3->setPosition(485, 200);
	walkingMap->addChild(city3);
	instance->cityPoint.push_back(city3->getPosition());
	cityPoint.push_back(city3->getPosition());
	instance->cityVector.push_back(city3);
	city3->setTag(3);
	const char *str_3 = ((String*)dic->objectForKey("levelName3"))->_string.c_str();
	std::string str3 = str_3;
	city3->initLevelName(str3);

	city4 = city::create();
	city4->setPosition(340, 320);
	walkingMap->addChild(city4);
	instance->cityPoint.push_back(city4->getPosition());
	cityPoint.push_back(city4->getPosition());
	instance->cityVector.push_back(city4);
	city4->setTag(4);
	const char *str_4 = ((String*)dic->objectForKey("levelName4"))->_string.c_str();
	std::string str4 = str_4;
	city4->initLevelName(str4);

	city5 = city::create();
	city5->setPosition(560, 410);
	walkingMap->addChild(city5);
	instance->cityPoint.push_back(city5->getPosition());
	cityPoint.push_back(city5->getPosition());
	instance->cityVector.push_back(city5);
	city5->setTag(5);
	const char *str_5 = ((String*)dic->objectForKey("levelName5"))->_string.c_str();
	std::string str5 = str_5;
	city5->initLevelName(str5);

	city6 = city::create();
	city6->setPosition(816, 176);
	walkingMap->addChild(city6);
	instance->cityPoint.push_back(city6->getPosition());
	cityPoint.push_back(city6->getPosition());
	instance->cityVector.push_back(city6);
	city6->setTag(6);
	const char *str_6 = ((String*)dic->objectForKey("levelName6"))->_string.c_str();
	std::string str6 = str_6;
	city6->initLevelName(str6);

	city7 = city::create();
	city7->setPosition(790, 330);
	walkingMap->addChild(city7);
	instance->cityPoint.push_back(city7->getPosition());
	cityPoint.push_back(city7->getPosition());
	instance->cityVector.push_back(city7);
	city7->setTag(7);
	const char *str_7 = ((String*)dic->objectForKey("levelName7"))->_string.c_str();
	std::string str7 = str_7;
	city7->initLevelName(str7);

	city8 = city::create();
	city8->setPosition(887, 467);
	walkingMap->addChild(city8);
	instance->cityPoint.push_back(city8->getPosition());
	cityPoint.push_back(city8->getPosition());
	instance->cityVector.push_back(city8);
	city8->setTag(8);
	const char *str_8 = ((String*)dic->objectForKey("levelName8"))->_string.c_str();
	std::string str8= str_8;
	city8->initLevelName(str8);

	city9 = city::create();
	city9->setPosition(838, 682);
	walkingMap->addChild(city9);
	instance->cityPoint.push_back(city9->getPosition());
	cityPoint.push_back(city9->getPosition());
	instance->cityVector.push_back(city9);
	city9->setTag(9);
	const char *str_9 = ((String*)dic->objectForKey("levelName9"))->_string.c_str();
	std::string str9 = str_9;
	city9->initLevelName(str9);

	city10 = city::create();
	city10->setPosition(677, 756);
	walkingMap->addChild(city10);
	instance->cityPoint.push_back(city10->getPosition());
	cityPoint.push_back(city10->getPosition());
	instance->cityVector.push_back(city10);
	city10->setTag(10);
	const char *str_10 = ((String*)dic->objectForKey("levelName10"))->_string.c_str();
	std::string str10 = str_10;
	city10->initLevelName(str10);

	city11 = city::create();
	city11->setPosition(563, 600);
	walkingMap->addChild(city11);
	instance->cityPoint.push_back(city11->getPosition());
	cityPoint.push_back(city11->getPosition());
	instance->cityVector.push_back(city11);
	city11->setTag(11);
	const char *str_11 = ((String*)dic->objectForKey("levelName11"))->_string.c_str();
	std::string str11 = str_11;
	city11->initLevelName(str11);

	city12 = city::create();
	city12->setPosition(443, 765);
	walkingMap->addChild(city12);
	instance->cityPoint.push_back(city12->getPosition());
	cityPoint.push_back(city12->getPosition());
	instance->cityVector.push_back(city12);
	city12->setTag(12);
	const char *str_12 = ((String*)dic->objectForKey("levelName12"))->_string.c_str();
	std::string str12 = str_12;
	city12->initLevelName(str12);

	city13 = city::create();
	city13->setPosition(474, 495);
	walkingMap->addChild(city13);
	instance->cityPoint.push_back(city13->getPosition());
	cityPoint.push_back(city13->getPosition());
	instance->cityVector.push_back(city13);
	city13->setTag(13);
	const char *str_13 = ((String*)dic->objectForKey("levelName13"))->_string.c_str();
	std::string str13 = str_13;
	city13->initLevelName(str13);

	city14 = city::create();
	city14->setPosition(250, 580);
	walkingMap->addChild(city14);
	instance->cityPoint.push_back(city14->getPosition());
	cityPoint.push_back(city14->getPosition());
	instance->cityVector.push_back(city14);
	city14->setTag(14);
	const char *str_14 = ((String*)dic->objectForKey("levelName14"))->_string.c_str();
	std::string str14 = str_14;
	city14->initLevelName(str14);

	city15 = city::create();
	city15->setPosition(111, 480);
	walkingMap->addChild(city15);
	instance->cityPoint.push_back(city15->getPosition());
	cityPoint.push_back(city15->getPosition());
	instance->cityVector.push_back(city15);
	city15->setTag(15);
	const char *str_15 = ((String*)dic->objectForKey("levelName15"))->_string.c_str();
	std::string str15 = str_15;
	city15->initLevelName(str15);

	city16 = city::create();
	city16->setPosition(86, 696);
	walkingMap->addChild(city16);
	instance->cityPoint.push_back(city16->getPosition());
	cityPoint.push_back(city16->getPosition());
	instance->cityVector.push_back(city16);
	city16->setTag(16);
	const char *str_16 = ((String*)dic->objectForKey("levelName16"))->_string.c_str();
	std::string str16 = str_16;
	city16->initLevelName(str16);

	city17 = city::create();
	city17->setPosition(186, 815);
	walkingMap->addChild(city17);
	instance->cityPoint.push_back(city17->getPosition());
	cityPoint.push_back(city17->getPosition());
	instance->cityVector.push_back(city17);
	city17->setTag(17);
	const char *str_17 = ((String*)dic->objectForKey("levelName17"))->_string.c_str();
	std::string str17 = str_17;
	city17->initLevelName(str17);



}

bool LevelSelectScene::checkWetherPlayerIsArrived()
{
	for (int i = 0; i < cityPoint.size(); i++)
	{
		auto tempPoint = cityPoint.at(i);
		if (instance->player->getPosition() == tempPoint)
		{
			return true;
		}
	}

	return false;
}

void  LevelSelectScene::update(float dt)
{
	if (checkWetherPlayerIsArrived())
	{
		instance->isWalking = false;
	}
	else
	{
		instance->isWalking = true;
	}
	//log("%d", instance->isWalking);
}
