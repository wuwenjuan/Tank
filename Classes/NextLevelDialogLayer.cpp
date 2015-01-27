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
	//��������
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayerGradient::initWithColor(ccc4(0, 0, 0, 0),ccc4(250, 250, 250, 0),ccp(0,1)));
		//����initDialog����
		this->initDialog();
		//��Ϊtrue
		bRet = true;
	} while (0);
	return bRet;
}
void NextLevelDialogLayer::initDialog(){
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
	CCSprite* winSprite = CCSprite::create("win.png");
	//����ʧ�ܾ����λ��
	winSprite->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.65));
	//��ӵ�����
	this->addChild(winSprite);

	//������һ�ذ�ť
	CCMenuItemImage* next_level = CCMenuItemImage::create(
		"pic/next_level.png",
		"pic/next_level_select.png",
		this,
		menu_selector(NextLevelDialogLayer::menuCallbackNextLevel)
	);
	//���ð�ť��λ��
	next_level->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.45));
	//�����˳���ť
	CCMenuItemImage* exit_game = CCMenuItemImage::create(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		this,
		menu_selector(NextLevelDialogLayer::menuCallbackGoMenuLayer)
	);
	//���ð�ťλ��
	exit_game->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.3));
	//�����˵�
	m_pMenu = CCMenu::create(next_level, exit_game, NULL);
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
	//���ò˵���ť��������
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//��ȡ�����
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	ctl->tsm->goMenuLayer();
}
void NextLevelDialogLayer::menuCallbackNextLevel(cocos2d::CCObject *pSender){
	//���ò˵���ť��������
	m_pMenu->setEnabled(false);
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}

	//��ͣ����
	CCDirector::sharedDirector()->resume();
	//��ȡ����
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	//��ӱ�������װ����
	ctl->gameLayer->addOneGroupTree();
	//����һ�鷽̹��
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
	//��ȡ��͸����ĸ���
	ControlLayer* ctl = (ControlLayer*)this->getParent();
	//�ָ����أ���ť�Լ��ص�����
	ctl->nextLevel();
	//�Ӹ������Ƴ��ò�
	this->removeFromParentAndCleanup(false);
}
void NextLevelDialogLayer::resumeMenu()
{
	//���ò˵���ť������
	m_pMenu->setEnabled(true);
	//�ָ�����
}

