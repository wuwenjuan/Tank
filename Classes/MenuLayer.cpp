#include "MenuLayer.h"
#include "SimpleAudioEngine.h"
#include <string.h>
#include "MusicLayer.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
using namespace cocos2d;
//实现MenuLayer类中的init方法，初始化布景
bool MenuLayer::init(){
	//调用父类的初始化
    if ( !CCLayer::init()){
        return false;
    }
	//开启返回键监听
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
	//创建开始按钮的精灵
	CCMenuItemImage* startgame = CCMenuItemImage::create
	(
			"pic/start_game.png",
			"pic/start_game_select.png",
			this,
			menu_selector(MenuLayer::goToGameLayer)
	);
	//创建帮助按钮的精灵
	CCMenuItemImage* help = CCMenuItemImage::create
	(
			"pic/help.png",
			"pic/help_select.png",
			this,
			menu_selector(MenuLayer::goToHelpLayer)
	);
	//创建声音按钮的精灵
	CCMenuItemImage* soundset = CCMenuItemImage::create
	(
			"pic/soundset.png",
			"pic/soundset_select.png",
			this,
			menu_selector(MenuLayer::musicLayer)
	);
	//创建退出按钮的精灵
	CCMenuItemImage* exit = CCMenuItemImage::create
	(
			"pic/exit_game.png",
			"pic/exit_game_select.png",
			this,
			menu_selector(MenuLayer::exitGame)
	);
	pMenu = CCMenu::create(startgame,help,soundset,exit,NULL);
	//设置菜单位置
	pMenu->setPosition(CCPointZero);
	//设置为菜单不可用
	pMenu->setEnabled(false);
	//将菜单添加到布景中
	this->addChild(pMenu, 4);
	//声明CCNode
	CCNode* child = NULL;
	//获得pMenu中的按钮精灵并存入CCArray
	CCArray * pArray = pMenu->getChildren();
	//得到CCArray中元素的数量
	int count = pArray->count();
	//遍历CCArray
	for(int m=0;m<count;m++)
	{
		//获取menu中的按钮
		child = (CCNode*)(pArray->objectAtIndex(m));
		//设置按钮的初始位置
		child->setPosition(ccp(650,520));
		//创建到最下面的动作
		CCActionInterval* moveTo1 = CCMoveTo::create(0.2f,ccp(650,-40));
		//创建往上走的动作
		CCActionInterval* moveTo2 = CCMoveTo::create(0.25f*(count-m),ccp(650,80*(count-m)));
		//将动作进行封装
		CCActionInterval* move_ease_in2 = CCEaseElasticOut::create(moveTo2);
		//时间延长动作
		CCDelayTime *delay = CCDelayTime::create(1.5+0.2f*(count-m-1));
		if(m!=0)
		{
			//创建动作序列
			CCSequence* seq = CCSequence::create(delay,moveTo1,move_ease_in2, NULL);
			//执行动作
			child->runAction(seq);
		}
		else
		{
			//创建动作序列
			CCSequence* seq0 = CCSequence::create
			(
				delay,
				moveTo1,
				move_ease_in2,
				CCCallFuncND::create(this, callfuncND_selector(MenuLayer::setMenuEnabled), NULL),
				NULL
			);
			//在执行动作
			child->runAction(seq0);
		}
	}
    return true;
}
//菜单中按钮执行完动作的回调方法
void MenuLayer::setMenuEnabled(CCObject* pSender)
{
	//设置菜单可用
	pMenu->setEnabled(true);
}
//进入游戏界面
void MenuLayer::goToGameLayer(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//设置菜单按钮的可用性
	pMenu->setEnabled(false);
	//进入游戏界面
	tsm->goGameLayer();
}
//退出的回调方法
void MenuLayer::musicLayer(CCObject* pSender)
{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
		}
		//关闭返回键监听
		this->setKeypadEnabled(false);
		//设置菜单按钮的可用性
		pMenu->setEnabled(false);
		//创建半透明层
		MusicLayer *musicLayer = MusicLayer::create();
		//设置半透明层的位置
		musicLayer->setPosition(ccp(0,0));
		//添加到父类层
		this->addChild(musicLayer,DASHBOARD_LEVEL_DQX);
}
//进入帮助界面
void MenuLayer::goToHelpLayer(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//设置菜单按钮的可用性
	pMenu->setEnabled(false);
	//进入游戏界面
	tsm->goHelpLayer();
}
//退出应用程序
void MenuLayer::exitGame(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//设置菜单按钮的可用性
	pMenu->setEnabled(false);
	//退出
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		 exit(0);
	}
	#endif
}


