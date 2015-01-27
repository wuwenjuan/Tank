#include "MusicLayer.h"
#include "MenuLayer.h"
#include "JNIUtil.h"
#include "SimpleAudioEngine.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
MusicLayer::MusicLayer(){

}
MusicLayer::~MusicLayer(){
}
bool MusicLayer::init(){
	//返回类型
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayerGradient::initWithColor(ccc4(250, 250, 250, 0),ccc4(0, 0, 0, 0),ccp(0,1)));
		//条用initDialog方法
		this->initDialog();
		bRet = true;
	} while (0);
	return bRet;
}
void MusicLayer::initDialog(){

	setKeypadEnabled(true);
    //获取可见区域尺寸
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//获取可见区域原点坐标
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//创建背景精灵
	CCSprite* menuBackground = CCSprite::create("gameBackground.png");
	//设置背景精灵的位置
	menuBackground->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	//将背景精灵添加到布景
	this->addChild(menuBackground);
	//创建背景精灵
	CCSprite* title = CCSprite::create("title.png");
	//设置背景精灵的位置
	title->setPosition(ccp(origin.x+visibleSize.width/3,origin.y+visibleSize.height/6*5));
	//将背景精灵添加到布景
	this->addChild(title);
	//创建背景精灵
	CCSprite* decoration = CCSprite::create("decoration.png");
	//设置背景精灵的位置
	decoration->setPosition(ccp(origin.x+visibleSize.width/3,origin.y+visibleSize.height/3));
	//将背景精灵添加到布景
	this->addChild(decoration);
	soundon = CCMenuItemImage::create(
			"pic/open_music.png",
			"pic/open_music_select.png",
			CC_CALLBACK_0(MusicLayer::menuCallbacKOpenMusic,this)
	);
	soundon->setPosition(ccp(origin.x+visibleSize.width/4*3,origin.y+visibleSize.height/3*2));
	soundclose = CCMenuItemImage::create
	(
			"pic/close_music.png",
			"pic/close_music_select.png",
			CC_CALLBACK_0(MusicLayer::menuCallbackCloseMusic,this)
	);
	soundclose->setPosition(ccp(origin.x+visibleSize.width/4*3,origin.y+visibleSize.height/3*2));
	if(JNIUtil::getInt("musicFlag",1)==1)
	{
		soundon->setVisible(false);
		soundclose->setVisible(true);

	}
	else
	{
		soundon->setVisible(true);
		soundclose->setVisible(false);

	}
	effecton = CCMenuItemImage::create
	(
			"pic/open_effect.png",
			"pic/open_effect_select.png",
			CC_CALLBACK_0(MusicLayer::menuCallbackOpengEffect,this)
	);
	effecton->setPosition(ccp(origin.x+visibleSize.width/4*3,origin.y+visibleSize.height/3));
	effectclose = CCMenuItemImage::create
	(
			"pic/close_effect.png",
			"pic/close_effect_select.png",
			CC_CALLBACK_0(MusicLayer::menuCallbackCloseEffect,this)
	);
	effectclose->setPosition(ccp(origin.x+visibleSize.width/4*3,origin.y+visibleSize.height/3));
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		effecton->setVisible(false);
		effectclose->setVisible(true);

	}
	else
	{
		effecton->setVisible(true);
		effectclose->setVisible(false);

	}
	//创建游戏返回按钮
	CCMenuItemImage* back_menu = CCMenuItemImage::create
	(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		CC_CALLBACK_0(MusicLayer::menuCallbacKToMenuLayer,this)
	);
	//设置按钮位置
	back_menu->setPosition(ccp(origin.x+back_menu->getContentSize().width/2,origin.y+back_menu->getContentSize().height));
	pMenu = CCMenu::create(soundon,soundclose,effecton,effectclose,back_menu,NULL);
	//设置菜单位置
	pMenu->setPosition(CCPointZero);
	//设置为菜单不可用
	pMenu->setEnabled(false);
	//将菜单添加到布景中
	this->addChild(pMenu, 4);
	//颜色层执行动作
	this->runAction
	(
			CCSequence::create
			(
				(CCFadeOutBLTiles::create(1.0, CCSizeMake(16,12)))->reverse(),
				CallFuncN::create(CC_CALLBACK_0(MusicLayer::resumeMenu,this)),
				NULL
			)
	);
}


void MusicLayer::resumeMenu()
{
	//设置菜单为可用
	pMenu->setEnabled(true);
}

void MusicLayer::menuCallbacKToMenuLayer()
{

	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	this->setKeypadEnabled(false);
	//设置菜单按钮的可用性
	pMenu->setEnabled(false);
	//颜色层执行动作
	this->runAction
	(
		CCSequence::create
		(
			CCFadeOutBLTiles::create(1.0, CCSizeMake(16,12)),
			NULL
		)
	);
}
void MusicLayer::menuCallbackKeyBackClicked(cocos2d::CCObject *pSender)
{
	//获取父类按钮层的引用
	MenuLayer* menuLayer = (MenuLayer*)this->getParent();
	//设置按钮可用
	menuLayer->pMenu->setEnabled (true);
	//设开启父类层返回键监听
	menuLayer->setKeypadEnabled(true);
	//从父类中移除该层
	this->removeFromParentAndCleanup(false);

}
//打开声音
void  MusicLayer::menuCallbacKOpenMusic()
{

	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//preferencecaoz操作
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic
	(
		"sound/background.mp3",
		true
	);
	JNIUtil::saveInt("musicFlag",1);
	soundon->setVisible(false);
	soundclose->setVisible(true);


}
//关闭声音
void  MusicLayer::menuCallbackCloseMusic()
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	JNIUtil::saveInt("musicFlag",0);
	soundon->setVisible(true);
	soundclose->setVisible(false);
}
//打开音效
void  MusicLayer::menuCallbackOpengEffect()
{
	JNIUtil::saveInt("effectFlag",1);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	effecton->setVisible(false);
	effectclose->setVisible(true);
}
//关闭音效
void  MusicLayer::menuCallbackCloseEffect()
{
	JNIUtil::saveInt("effectFlag",0);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	effecton->setVisible(true);
	effectclose->setVisible(false);
}
