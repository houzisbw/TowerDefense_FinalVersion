#include "LoadLevelInfo.h"
#include "GameManager.h"
USING_NS_CC;

//读取出的文件存入gameManager中统一管理


LoadLevelInfo::LoadLevelInfo()
{
}


LoadLevelInfo::~LoadLevelInfo()
{
}

LoadLevelInfo* LoadLevelInfo::createLoadLevelInfo(const std::string &plistPath)
{
	auto loadLevelInfo = new LoadLevelInfo();
	if (loadLevelInfo && loadLevelInfo->initPlist(plistPath))
	{
		loadLevelInfo->autorelease();
		return loadLevelInfo;
	}
	else
	{
		delete loadLevelInfo;
		loadLevelInfo = nullptr;
		return nullptr;
	}
}

bool LoadLevelInfo::initPlist(const std::string &plistPath)
{
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistPath);
	ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);

	resource = dict["resources"].asValueMap();  //这2个仍然是valuemap，原来plist文件里是map中含有map
	levelInfo = dict["levelInfo"].asValueMap();

	return true;
}

void LoadLevelInfo::readLevelInfo()
{
	GameManager * instance = GameManager::getInstance();

	auto money = levelInfo["money"].asInt(); //读取金钱
	instance->setMoney(money);

	auto currentLevel = levelInfo["currlevel"].asString();
	instance->setCurrLevelFile(currentLevel);

	/*auto nextlevel = levelInfo["nextlevel"].asString();
	instance->setNextLevelFile(nextlevel);*/

	ValueMap& groupDict = levelInfo["group"].asValueMap();
	auto groupTotle = groupDict.size();  //敌人总的波数
	instance->setGroupNum(groupTotle);

	//log("group size %d", groupTotle);

	for (auto it = groupDict.begin(); it != groupDict.end(); it++)
	{
		auto group = it->second.asValueMap(); //得到每一波敌人的valuemap

		auto type1Num = group["type1Num"].asInt();
		auto type2Num = group["type2Num"].asInt();
		auto type3Num = group["type3Num"].asInt();
		auto type4Num = group["type4Num"].asInt();
		auto type5Num = group["type5Num"].asInt();

		auto type1Hp = group["type1Hp"].asInt();
		auto type2Hp = group["type2Hp"].asInt();
		auto type3Hp = group["type3Hp"].asInt();
		auto type4Hp = group["type4Hp"].asInt();
		auto type5Hp = group["type5Hp"].asInt();

		GroupEnemy* groupEnemy = GroupEnemy::create();
		groupEnemy->initGroupEnemy(type1Num, type1Hp, type2Num, type2Hp, type3Num, type3Hp, type4Num, type4Hp, type5Num, type5Hp);
		instance->groupVector.pushBack(groupEnemy);//存入vector中
	}

	//log("groupVector size %d", instance->groupVector.size());
	/*for (int i = 0; i < instance->groupVector.size(); i++)
	{
		auto temp = instance->groupVector.at(i);
		log("enemy Num: %d %d %d %d %d", temp->getType1Total(), temp->getType2Total(), temp->getType3Total(), temp->getType4Total(), temp->getType5Total());
	}*/
	

	auto curMapName = resource["map"].asString();
	instance->setCurMapName(curMapName);
	//auto curBgName = resource["image"].asString();
	//instance->setCurBgName(curBgName);
}


