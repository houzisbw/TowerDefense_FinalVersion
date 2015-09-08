#ifndef _OPTIONLAYER_H_
#define _OPTIONLAYER_H_
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using  namespace CocosDenshion;
class OptionLayer : public cocos2d::Layer
{
public:
	OptionLayer();
	~OptionLayer();
	virtual bool init();
	CREATE_FUNC(OptionLayer);
	void onEnter();
private:
	cocos2d::Node * rootNode;
	cocos2d::ui::ImageView* imageOption;
	cocos2d::ui::Button* returnButton;
	cocos2d::ui::Button* resumeButton;
	cocos2d::ui::ImageView* soundImage;


};
#endif

