#include "HelpLayer.h"
#include <string>
#include "SimpleAudioEngine.h"
#include "JNIUtil.h"
using namespace cocos2d;
//��ʼ������
bool HelpLayer::init(){
	//���ø���ĳ�ʼ��
	if(!CCLayer::init()){
		return false;
	}
	//������������
	setKeypadEnabled(true);
	//��ȡ�ɼ�����ߴ�
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//��ȡ�ɼ�����ԭ������
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//����������ı���
	backGround = CCSprite::create("pic/helpbg.png");
	//���ñ��������λ��
	backGround->setPosition
	(
		ccp
		(
			origin.x,//X����
			origin.y//Y����
		)
	);
	//����ê��
	backGround->setAnchorPoint(ccp(0,0));
	//����ӵ�����
	this->addChild(backGround,0);
	//������Ϸ���ذ�ť
	CCMenuItemImage* back_menu = CCMenuItemImage::create
	(
		"pic/back_menu.png",
		"pic/back_menu_select.png",
		CC_CALLBACK_0(HelpLayer::goToMenuLayer,this)
	);
	//���ð�ťλ��
	back_menu->setPosition(ccp(origin.x+visibleSize.width-back_menu->getContentSize().width,origin.y+back_menu->getContentSize().height));
	//�����˵�����
	pMenu = CCMenu::create(back_menu,NULL);
	//���ò˵�λ��
	pMenu->setPosition(CCPointZero);
	//���˵���ӵ�������
	this->addChild(pMenu, 4);
	return true;
}
void HelpLayer::goToMenuLayer()
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//���ص��˵�����
	tsm->goMenuLayer();

}

