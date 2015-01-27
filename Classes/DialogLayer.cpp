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
	//��������
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayerGradient::initWithColor(ccc4(250, 250, 250, 0),ccc4(0, 0, 0, 0),ccp(0,1)));
		//����initDialog����
		this->initDialog();
		bRet = true;
	} while (0);
	return bRet;
}
void DialogLayer::initDialog(){
    //��ȡ�ɼ�����ߴ�
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//��ȡ�ɼ�����ԭ������
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//����Բ�Ǳ�������
	CCSprite* background = CCSprite::create("pausebackground.png");
	//���ñ��������λ��
	background->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	//������ӵ�����
	this->addChild(background);
	//������ͣ��ť
	CCMenuItemImage* continue_game = CCMenuItemImage::create(
		"pic/continue_game.png",
		"pic/continue_game_select.png",
		this,
		menu_selector(DialogLayer::menuCallbackContinueGame)
	);
	//���ð�ť��λ��
	continue_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/3*2));
	//�������ز˵���ť
	CCMenuItemImage* back_menu = CCMenuItemImage::create(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		this,
		menu_selector(DialogLayer::menuCallbackGoMenuLayer)
	);
	//���ð�ťλ��
	back_menu->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));

	//�����˳���ť
	CCMenuItemImage* exit_game = CCMenuItemImage::create(
		"pic/exit_game.png",
		"pic/exit_game_select.png",
		this,
		menu_selector(DialogLayer::menuCallbackExitGame)
	);
		//���ð�ťλ��
		exit_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/3));

	//�����˵�
	m_pMenu = CCMenu::create(continue_game, exit_game,back_menu, NULL);
	//���ò˵�λ��
	m_pMenu->setPosition(CCPointZero);
	//��Ӳ˵�������
	this->addChild(m_pMenu);
	//���ò˵���ť��������
	m_pMenu->setEnabled(false);
	//��ɫ��ִ�ж���
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
	//���ò˵���ť��������
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//��һ�·���
	//�ָ�����
	CCDirector::sharedDirector()->resume();
	//��ȡ�����
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->tsm->goMenuLayer();


}
void DialogLayer::menuCallbackExitGame(cocos2d::CCObject *pSender){
	//���ò˵���ť��������
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//��һ�·���
	//�˳�
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		exit(0);
	}
	#endif
}
void DialogLayer::menuCallbackContinueGame(cocos2d::CCObject *pSender){
	//���ò˵���ť��������
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}

	//��ͣ����
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
	//��ȡ��͸����ĸ���
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->nextLevel();
	//�������˵���ť����
	ctl->pMenu->setEnabled(true);
	//�������ؼ�����
	ctl->setKeypadEnabled(true);
	//�Ӹ������Ƴ��ò�
	this->removeFromParentAndCleanup(false);
}
void DialogLayer::resumeMenu()
{
	//���ò˵���ť������
	m_pMenu->setEnabled(true);
	//��ͣ����
	CCDirector::sharedDirector()->pause();

}

