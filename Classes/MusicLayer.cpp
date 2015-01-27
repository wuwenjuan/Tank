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
void MusicLayer::initDialog(){

	setKeypadEnabled(true);
    //��ȡ�ɼ�����ߴ�
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//��ȡ�ɼ�����ԭ������
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//������������
	CCSprite* menuBackground = CCSprite::create("gameBackground.png");
	//���ñ��������λ��
	menuBackground->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	//������������ӵ�����
	this->addChild(menuBackground);
	//������������
	CCSprite* title = CCSprite::create("title.png");
	//���ñ��������λ��
	title->setPosition(ccp(origin.x+visibleSize.width/3,origin.y+visibleSize.height/6*5));
	//������������ӵ�����
	this->addChild(title);
	//������������
	CCSprite* decoration = CCSprite::create("decoration.png");
	//���ñ��������λ��
	decoration->setPosition(ccp(origin.x+visibleSize.width/3,origin.y+visibleSize.height/3));
	//������������ӵ�����
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
	//������Ϸ���ذ�ť
	CCMenuItemImage* back_menu = CCMenuItemImage::create
	(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		CC_CALLBACK_0(MusicLayer::menuCallbacKToMenuLayer,this)
	);
	//���ð�ťλ��
	back_menu->setPosition(ccp(origin.x+back_menu->getContentSize().width/2,origin.y+back_menu->getContentSize().height));
	pMenu = CCMenu::create(soundon,soundclose,effecton,effectclose,back_menu,NULL);
	//���ò˵�λ��
	pMenu->setPosition(CCPointZero);
	//����Ϊ�˵�������
	pMenu->setEnabled(false);
	//���˵���ӵ�������
	this->addChild(pMenu, 4);
	//��ɫ��ִ�ж���
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
	//���ò˵�Ϊ����
	pMenu->setEnabled(true);
}

void MusicLayer::menuCallbacKToMenuLayer()
{

	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	this->setKeypadEnabled(false);
	//���ò˵���ť�Ŀ�����
	pMenu->setEnabled(false);
	//��ɫ��ִ�ж���
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
	//��ȡ���ఴť�������
	MenuLayer* menuLayer = (MenuLayer*)this->getParent();
	//���ð�ť����
	menuLayer->pMenu->setEnabled (true);
	//�迪������㷵�ؼ�����
	menuLayer->setKeypadEnabled(true);
	//�Ӹ������Ƴ��ò�
	this->removeFromParentAndCleanup(false);

}
//������
void  MusicLayer::menuCallbacKOpenMusic()
{

	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//preferencecaoz����
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic
	(
		"sound/background.mp3",
		true
	);
	JNIUtil::saveInt("musicFlag",1);
	soundon->setVisible(false);
	soundclose->setVisible(true);


}
//�ر�����
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
//����Ч
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
//�ر���Ч
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
