#include "GroupEnemy.h"
USING_NS_CC;

GroupEnemy::GroupEnemy()
{
}


GroupEnemy::~GroupEnemy()
{
}

GroupEnemy* GroupEnemy::initGroupEnemy(int type1Total, int type1Hp, int type2Total, int type2Hp, int type3Total, int type3Hp, int type4Total, int type4Hp, int type5Total, int type5Hp)
{
	this->type1Total = type1Total;
	this->type2Total = type2Total;
	this->type3Total = type3Total;
	this->type4Total = type4Total;
	this->type5Total = type5Total;
	this->type1Hp = type1Hp;
	this->type2Hp = type2Hp;
	this->type3Hp = type3Hp;
	this->type4Hp = type4Hp;
	this->type5Hp = type5Hp;
	this->enemyTotal = type1Total + type2Total + type3Total+type4Total+type5Total;
	this->isFinishedAddGroup = false; //判断本波敌人是否全部添加到场景中去了
	return this;
}

bool GroupEnemy::init()
{
	if (!Node::init())
	{
		return false;
	}


	return true;
}