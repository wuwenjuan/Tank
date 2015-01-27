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
	//��������
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayerGradient::initWithColor(ccc4(0, 0, 0, 0),ccc4(0, 0, 0, 0),ccp(0,1)));
		//����initDialog����
		this->initDialog();
		bRet = true;
	} while (0);
	return bRet;
}
void LoseLayer::initDialog(){
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
	//����ʧ�ܾ���
	CCSprite* loseSprite = CCSprite::create("lose.png");
	//����ʧ�ܾ����λ��
	loseSprite->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.65));
	//��ӵ�����
	this->addChild(loseSprite);
	//�������ز˵���ť
	CCMenuItemImage* back_menu = CCMenuItemImage::create(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		this,
		menu_selector(LoseLayer::menuCallbackGoMenuLayer)
	);
	//���ð�ťλ��
	back_menu->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.45));

	//�����˳���ť
	CCMenuItemImage* exit_game = CCMenuItemImage::create(
		"pic/exit_game.png",
		"pic/exit_game_select.png",
		this,
		menu_selector(LoseLayer::menuCallbackExitGame)
	);
	//���ð�ťλ��
	exit_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.3));

	//�����˵�
	m_pMenu = CCMenu::create(exit_game,back_menu, NULL);
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
	//��һ�·���

	//�ָ�����
	CCDirector::sharedDirector()->resume();
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//��ȡ�����
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->tsm->goMenuLayer();


}
void LoseLayer::menuCallbackExitGame(cocos2d::CCObject *pSender){
	//��һ�·���

	//�˳�
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		exit(0);
	}
	#endif
}
void LoseLayer::resumeMenu()
{
	//���ò˵���ť������
	m_pMenu->setEnabled(true);
	//�ָ�����
	CCDirector::sharedDirector()->pause();

}

