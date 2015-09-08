#include "WelcomeScene.h"
#include "LevelSelectScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

WelcomeScene::WelcomeScene()
{
}


WelcomeScene::~WelcomeScene()
{
}

bool WelcomeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//初始化加载背景音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("SwordLand.OGG");

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.6);



	auto visibleSize = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("welcomeBg.jpg");
	bg->setPosition(visibleSize / 2);
	addChild(bg, 1);

	root = CSLoader::createNode("WelcomeScene.csb");
	addChild(root, 3);

	start = root->getChildByName<ui::ImageView*>("Image_start");
	start->addClickEventListener([=](Ref* ref){
		
		clickEffect(start);
		Director::getInstance()->replaceScene(TransitionFade::create(1.5, LevelSelectScene::createScene()));
	    
	});
	auto st = ScaleBy::create(1, 1.1);
	auto seq = Sequence::create(st, st->reverse(), NULL);
	auto rep = RepeatForever::create(seq);
	start->runAction(rep);


	return true;
}


Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}

void WelcomeScene::particleEffect(Point pos)
{

	ParticleExplosion* effect = ParticleExplosion::create();
	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("victoryMenu.png"));
	effect->setTotalParticles(10);

	//让其为图片本身的颜色
	effect->setStartColor(Color4F(255, 255, 255, 255));
	effect->setEndColor(Color4F(255, 255, 255, 0));

	effect->setStartSize(50.0f);

	effect->setLife(2.6f);
	effect->setSpeed(200);
	effect->setSpeedVar(10);
	effect->setPosition(pos);
	start->addChild(effect, 15);
}
void WelcomeScene::clickEffect(cocos2d::ui::ImageView* image)
{
	auto st = ScaleTo::create(1, 4);
	auto fo = FadeOut::create(1);
	auto sp = Spawn::createWithTwoActions(st, fo);
	image->runAction(sp);
}