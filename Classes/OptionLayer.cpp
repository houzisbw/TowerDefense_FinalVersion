#include "OptionLayer.h"
#include "GameManager.h"
#include "LevelSelectScene.h"

USING_NS_CC;

OptionLayer::OptionLayer()
{
}


OptionLayer::~OptionLayer()
{
}
void OptionLayer::onEnter()
{
	Layer::onEnter();
	this->setSwallowsTouches(true);
}

bool OptionLayer::init()
{
	Layer::init();

	GameManager* instance = GameManager::getInstance();

	rootNode = CSLoader::createNode("OptionScene.csb");
	addChild(rootNode, 10); //顶层

	imageOption = rootNode->getChildByName<ui::ImageView*>("Image_option");

	resumeButton = imageOption->getChildByName<ui::Button*>("Button_resume");
	resumeButton->addClickEventListener([=](Ref * ref){
		this->removeFromParent();
	});

	returnButton = imageOption->getChildByName<ui::Button*>("Button_return");
	returnButton->addClickEventListener([=](Ref * ref){
		instance->clear();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LevelSelectScene::createScene()));

	});

	soundImage = rootNode->getChildByName<ui::ImageView*>("Image_sound");
	soundImage->addClickEventListener([=](Ref * ref){
		log("click");
		if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) //如果没有音乐播放，则开始播放音乐
		{
			log("no music");
			SimpleAudioEngine::getInstance()->playBackgroundMusic("SwordLand.OGG", true);
			soundImage->loadTexture("sound_on.png");
		}
		else
		{
			log("has music");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			soundImage->loadTexture("sound_off.png");
		}

	});


	auto listener = EventListenerTouchOneByOne::create();
	/*这里为了防止在该层点击能够点击到该层下面的精灵，所以要
	设置吞没函数，为true，而且ontouchbegin函数返要是true*/
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){

		return true;

	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;

}
