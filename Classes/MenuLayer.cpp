#include "MenuLayer.h"
#include "SimpleAudioEngine.h"
#include <string.h>
#include "MusicLayer.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
using namespace cocos2d;
//ʵ��MenuLayer���е�init��������ʼ������
bool MenuLayer::init(){
	//���ø���ĳ�ʼ��
    if ( !CCLayer::init()){
        return false;
    }
	//�������ؼ�����
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
	//������ʼ��ť�ľ���
	CCMenuItemImage* startgame = CCMenuItemImage::create
	(
			"pic/start_game.png",
			"pic/start_game_select.png",
			this,
			menu_selector(MenuLayer::goToGameLayer)
	);
	//����������ť�ľ���
	CCMenuItemImage* help = CCMenuItemImage::create
	(
			"pic/help.png",
			"pic/help_select.png",
			this,
			menu_selector(MenuLayer::goToHelpLayer)
	);
	//����������ť�ľ���
	CCMenuItemImage* soundset = CCMenuItemImage::create
	(
			"pic/soundset.png",
			"pic/soundset_select.png",
			this,
			menu_selector(MenuLayer::musicLayer)
	);
	//�����˳���ť�ľ���
	CCMenuItemImage* exit = CCMenuItemImage::create
	(
			"pic/exit_game.png",
			"pic/exit_game_select.png",
			this,
			menu_selector(MenuLayer::exitGame)
	);
	pMenu = CCMenu::create(startgame,help,soundset,exit,NULL);
	//���ò˵�λ��
	pMenu->setPosition(CCPointZero);
	//����Ϊ�˵�������
	pMenu->setEnabled(false);
	//���˵���ӵ�������
	this->addChild(pMenu, 4);
	//����CCNode
	CCNode* child = NULL;
	//���pMenu�еİ�ť���鲢����CCArray
	CCArray * pArray = pMenu->getChildren();
	//�õ�CCArray��Ԫ�ص�����
	int count = pArray->count();
	//����CCArray
	for(int m=0;m<count;m++)
	{
		//��ȡmenu�еİ�ť
		child = (CCNode*)(pArray->objectAtIndex(m));
		//���ð�ť�ĳ�ʼλ��
		child->setPosition(ccp(650,520));
		//������������Ķ���
		CCActionInterval* moveTo1 = CCMoveTo::create(0.2f,ccp(650,-40));
		//���������ߵĶ���
		CCActionInterval* moveTo2 = CCMoveTo::create(0.25f*(count-m),ccp(650,80*(count-m)));
		//���������з�װ
		CCActionInterval* move_ease_in2 = CCEaseElasticOut::create(moveTo2);
		//ʱ���ӳ�����
		CCDelayTime *delay = CCDelayTime::create(1.5+0.2f*(count-m-1));
		if(m!=0)
		{
			//������������
			CCSequence* seq = CCSequence::create(delay,moveTo1,move_ease_in2, NULL);
			//ִ�ж���
			child->runAction(seq);
		}
		else
		{
			//������������
			CCSequence* seq0 = CCSequence::create
			(
				delay,
				moveTo1,
				move_ease_in2,
				CCCallFuncND::create(this, callfuncND_selector(MenuLayer::setMenuEnabled), NULL),
				NULL
			);
			//��ִ�ж���
			child->runAction(seq0);
		}
	}
    return true;
}
//�˵��а�ťִ���궯���Ļص�����
void MenuLayer::setMenuEnabled(CCObject* pSender)
{
	//���ò˵�����
	pMenu->setEnabled(true);
}
//������Ϸ����
void MenuLayer::goToGameLayer(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//���ò˵���ť�Ŀ�����
	pMenu->setEnabled(false);
	//������Ϸ����
	tsm->goGameLayer();
}
//�˳��Ļص�����
void MenuLayer::musicLayer(CCObject* pSender)
{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
		}
		//�رշ��ؼ�����
		this->setKeypadEnabled(false);
		//���ò˵���ť�Ŀ�����
		pMenu->setEnabled(false);
		//������͸����
		MusicLayer *musicLayer = MusicLayer::create();
		//���ð�͸�����λ��
		musicLayer->setPosition(ccp(0,0));
		//��ӵ������
		this->addChild(musicLayer,DASHBOARD_LEVEL_DQX);
}
//�����������
void MenuLayer::goToHelpLayer(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//���ò˵���ť�Ŀ�����
	pMenu->setEnabled(false);
	//������Ϸ����
	tsm->goHelpLayer();
}
//�˳�Ӧ�ó���
void MenuLayer::exitGame(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//���ò˵���ť�Ŀ�����
	pMenu->setEnabled(false);
	//�˳�
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		 exit(0);
	}
	#endif
}


