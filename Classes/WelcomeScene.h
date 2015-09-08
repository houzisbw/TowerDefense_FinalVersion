#ifndef _WELCOMESCENE_H_
#define _WELCOMESCENE_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class WelcomeScene : public cocos2d::Layer
{
public:
	WelcomeScene();
	~WelcomeScene();
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(WelcomeScene);
	void particleEffect(cocos2d::Point pos);
	void clickEffect(cocos2d::ui::ImageView* image);
private:
	cocos2d::Sprite * bg;
	cocos2d::Node * root;
	cocos2d::ui::ImageView* start;//开始按钮
};
#endif

