#ifndef _LOADLEVELINFO_H_
#define _LOADLEVELINFO_H_
#include "cocos2d.h"
class LoadLevelInfo : public cocos2d::Ref
{
public:
	LoadLevelInfo();
	~LoadLevelInfo();
	static LoadLevelInfo* createLoadLevelInfo(const std::string &plistPath);
	bool initPlist(const std::string &plistPath);
	void readLevelInfo();
	void clearAll();
private:
	cocos2d::ValueMap resource;
	cocos2d::ValueMap levelInfo;
};
#endif

