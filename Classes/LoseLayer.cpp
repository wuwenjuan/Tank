#include "LoseLayer.h"
#include "ControlLayer.h"
#include "SimpleAudioEngine.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
LoseLayer::LoseLayer(){

}
LoseLayer::~LoseLayer(){
}
bool LoseLayer::init(){
	//返回类型
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayerGradient::initWithColor(ccc4(0, 0, 0, 0),ccc4(0, 0, 0, 0),ccp(0,1)));
		//条用initDialog方法
		this->initDialog();
		bRet = true;
	} while (0);
	return bRet;
}
void LoseLayer::initDialog(){
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
	//创建失败精灵
	CCSprite* loseSprite = CCSprite::create("lose.png");
	//设置失败精灵的位置
	loseSprite->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.65));
	//添加到布景
	this->addChild(loseSprite);
	//创建返回菜单按钮
	CCMenuItemImage* back_menu = CCMenuItemImage::create(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		this,
		menu_selector(LoseLayer::menuCallbackGoMenuLayer)
	);
	//设置按钮位置
	back_menu->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.45));

	//创建退出按钮
	CCMenuItemImage* exit_game = CCMenuItemImage::create(
		"pic/exit_game.png",
		"pic/exit_game_select.png",
		this,
		menu_selector(LoseLayer::menuCallbackExitGame)
	);
	//设置按钮位置
	exit_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.3));

	//创建菜单
	m_pMenu = CCMenu::create(exit_game,back_menu, NULL);
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
				CCCallFuncND::create(this, callfuncND_selector(LoseLayer::resumeMenu), NULL),
				NULL
			)
	);
}
void LoseLayer::onEnter(){
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}
void LoseLayer::onExit(){
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool LoseLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}
void LoseLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void LoseLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void LoseLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void LoseLayer::menuCallbackGoMenuLayer(cocos2d::CCObject *pSender){
	//存一下分数

	//恢复场景
	CCDirector::sharedDirector()->resume();
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//获取父类层
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->tsm->goMenuLayer();


}
void LoseLayer::menuCallbackExitGame(cocos2d::CCObject *pSender){
	//存一下分数

	//退出
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		exit(0);
	}
	#endif
}
void LoseLayer::resumeMenu()
{
	//设置菜单按钮可以用
	m_pMenu->setEnabled(true);
	//恢复场景
	CCDirector::sharedDirector()->pause();

}

