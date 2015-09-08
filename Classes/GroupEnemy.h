#ifndef _GROUPENEMY_H_
#define _GROUPENEMY_H_
#include "cocos2d.h"
class GroupEnemy :public cocos2d::Node
{
public:
	GroupEnemy();
	~GroupEnemy();
	virtual bool init();
	//5种敌人的血量和个数
	GroupEnemy* initGroupEnemy(int type1Total, int type1Hp, int type2Total, int type2Hp, int type3Total, int type3Hp, int type4Total, int type4Hp, int type5Total, int type5Hp);
	CREATE_FUNC(GroupEnemy);

	CC_SYNTHESIZE(int, type1Total, Type1Total);
	CC_SYNTHESIZE(int, type2Total, Type2Total);
	CC_SYNTHESIZE(int, type3Total, Type3Total);
	CC_SYNTHESIZE(int, type4Total, Type4Total);
	CC_SYNTHESIZE(int, type5Total, Type5Total);
	CC_SYNTHESIZE(int, type1Hp, Type1Hp);
	CC_SYNTHESIZE(int, type2Hp, Type2Hp);
	CC_SYNTHESIZE(int, type3Hp, Type3Hp);
	CC_SYNTHESIZE(int, type4Hp, Type4Hp);
	CC_SYNTHESIZE(int, type5Hp, Type5Hp);
	CC_SYNTHESIZE(int, enemyTotal, EnemyTotal);
	CC_SYNTHESIZE(bool, isFinishedAddGroup, IsFinishedAddGroup);
};
#endif
