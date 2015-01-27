#include "NextLevelDialogLayer.h"
#include "ControlLayer.h"
#include "SimpleAudioEngine.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
NextLevelDialogLayer::NextLevelDialogLayer(){

}
NextLevelDialogLayer::~NextLevelDialogLayer(){
}
bool NextLevelDialogLayer::init(){
	//返回类型
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayerGradient::initWithColor(ccc4(0, 0, 0, 0),ccc4(250, 250, 250, 0),ccp(0,1)));
		//条用initDialog方法
		this->initDialog();
		//置为true
		bRet = true;
	} while (0);
	return bRet;
}
void NextLevelDialogLayer::initDialog(){
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
	CCSprite* winSprite = CCSprite::create("win.png");
	//设置失败精灵的位置
	winSprite->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.65));
	//添加到布景
	this->addChild(winSprite);

	//创建下一关按钮
	CCMenuItemImage* next_level = CCMenuItemImage::create(
		"pic/next_level.png",
		"pic/next_level_select.png",
		this,
		menu_selector(NextLevelDialogLayer::menuCallbackNextLevel)
	);
	//设置按钮的位置
	next_level->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.45));
	//创建退出按钮
	CCMenuItemImage* exit_game = CCMenuItemImage::create(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		this,
		menu_selector(NextLevelDialogLayer::menuCallbackGoMenuLayer)
	);
	//设置按钮位置
	exit_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.3));
	//创建菜单
	m_pMenu = CCMenu::create(next_level, exit_game, NULL);
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
			CCCallFuncND::create(this, callfuncND_selector(NextLevelDialogLayer::resumeMenu), NULL),
			NULL
		)
	);
}
void NextLevelDialogLayer::onEnter(){
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}
void NextLevelDialogLayer::onExit(){
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool NextLevelDialogLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}
void NextLevelDialogLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void NextLevelDialogLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void NextLevelDialogLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void NextLevelDialogLayer::menuCallbackGoMenuLayer(cocos2d::CCObject *pSender){
	//设置菜单按钮不可以用
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//获取父类层
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->tsm->goMenuLayer();
}
void NextLevelDialogLayer::menuCallbackNextLevel(cocos2d::CCObject *pSender){
	//设置菜单按钮不可以用
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}

	//暂停场景
	CCDirector::sharedDirector()->resume();
	//获取父层
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	//添加背景精灵装饰着
	ctl->gameLayer->addOneGroupTree();
	//产生一组方坦克
	ctl->addEnemyTank(0);
	this->runAction
	(
		CCSequence::create
		(
			CCFadeOutBLTiles::create(1.0, CCSizeMake(16,12)),
			CCCallFuncND::create(this, callfuncND_selector(NextLevelDialogLayer::returnGameLayer), NULL),
			NULL
		)
	);
}
void NextLevelDialogLayer::returnGameLayer(){
	//获取半透明层的父类
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	//恢复触控，按钮以及回调方法
	ctl->nextLevel();
	//从父类中移除该层
	this->removeFromParentAndCleanup(false);
}
void NextLevelDialogLayer::resumeMenu()
{
	//设置菜单按钮可以用
	m_pMenu->setEnabled(true);
	//恢复场景
}

