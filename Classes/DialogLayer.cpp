#include "DialogLayer.h"
#include "ControlLayer.h"
#include "SimpleAudioEngine.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
DialogLayer::DialogLayer(){

}
DialogLayer::~DialogLayer(){
}
bool DialogLayer::init(){
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
void DialogLayer::initDialog(){
    //获取可见区域尺寸
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//获取可见区域原点坐标
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//创建圆角背景精灵
	CCSprite* background = CCSprite::create("pausebackground.png");
	//设置背景精灵的位置
	background->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	//精灵添加到布景
	this->addChild(background);
	//创建暂停按钮
	CCMenuItemImage* continue_game = CCMenuItemImage::create(
		"pic/continue_game.png",
		"pic/continue_game_select.png",
		this,
		menu_selector(DialogLayer::menuCallbackContinueGame)
	);
	//设置按钮的位置
	continue_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/3*2));
	//创建返回菜单按钮
	CCMenuItemImage* back_menu = CCMenuItemImage::create(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		this,
		menu_selector(DialogLayer::menuCallbackGoMenuLayer)
	);
	//设置按钮位置
	back_menu->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));

	//创建退出按钮
	CCMenuItemImage* exit_game = CCMenuItemImage::create(
		"pic/exit_game.png",
		"pic/exit_game_select.png",
		this,
		menu_selector(DialogLayer::menuCallbackExitGame)
	);
		//设置按钮位置
		exit_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/3));

	//创建菜单
	m_pMenu = CCMenu::create(continue_game, exit_game,back_menu, NULL);
	//设置菜单位置
	m_pMenu->setPosition(CCPointZero);
	//添加菜单到布景
	this->addChild(m_pMenu);
	//设置菜单按钮不可以用
	m_pMenu->setEnabled(false);
	//颜色层执行动作
	this->runAction
	(
			CCSequence::create
			(
				(CCFadeOutBLTiles::create(1.0, CCSizeMake(16,12)))->reverse(),
             cocos2d::CallFunc::create(CC_CALLBACK_0(DialogLayer::resumeMenu, this)),
				NULL
			)
	);
}
void DialogLayer::onEnter(){
    CCLayerColor::onEnter();
}
void DialogLayer::onExit(){
    CCLayerColor::onExit();
}

void DialogLayer::menuCallbackGoMenuLayer(cocos2d::CCObject *pSender){
	//设置菜单按钮不可以用
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//存一下分数
	//恢复场景
	CCDirector::sharedDirector()->resume();
	//获取父类层
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->tsm->goMenuLayer();


}
void DialogLayer::menuCallbackExitGame(cocos2d::CCObject *pSender){
	//设置菜单按钮不可以用
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//存一下分数
	//退出
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		exit(0);
	}
	#endif
}
void DialogLayer::menuCallbackContinueGame(cocos2d::CCObject *pSender){
	//设置菜单按钮不可以用
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}

	//暂停场景
	CCDirector::sharedDirector()->resume();
	this->runAction
	(
		CCSequence::create
		(
			CCFadeOutBLTiles::create(1.0, CCSizeMake(16,12)),
         CallFuncN::create(CC_CALLBACK_0(DialogLayer::returnGameLayer, this)),
			NULL
		)
	);
}
void DialogLayer::returnGameLayer(){
	//获取半透明层的父类
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->nextLevel();
	//设置主菜单按钮可用
	ctl->pMenu->setEnabled(true);
	//开启返回键监听
	ctl->setKeypadEnabled(true);
	//从父类中移除该层
	this->removeFromParentAndCleanup(false);
}
void DialogLayer::resumeMenu()
{
	//设置菜单按钮可以用
	m_pMenu->setEnabled(true);
	//暂停场景
	CCDirector::sharedDirector()->pause();

}

