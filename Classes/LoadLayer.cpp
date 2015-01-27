#include "LoadLayer.h"
#include <string.h>
#include "SimpleAudioEngine.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
using namespace cocos2d;
//ʵ��LoadLayer���е�init��������ʼ������
bool LoadLayer::init(){
	//���ø���ĳ�ʼ��
    if ( !CCLayer::init()){
        return false;
    }
	//��ȡ�ɼ�����ߴ�
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//��ȡ�ɼ�����ԭ������
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    //��ӱ�������
    CCSprite* background = CCSprite::create("gameBackground.png");
    //���þ����λ��
    background->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
    //��������ӵ�����
    this->addChild(background);
    //��ӱ�������
	CCSprite* decoration = CCSprite::create("decoration.png");
	//���þ����λ��
	decoration->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/3*2));
	//��������ӵ�����
	this->addChild(decoration);
    //�������ؾ���
    CCSprite* load = CCSprite::create("load.png");
    //���ü��ؾ����λ��
    load->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/5));
    //����ɫ����Ӿ���
    this->addChild(load);
    //���������
    int count = rand()%3+1;
    //���־���ִ�ж���
    load->runAction
	(
		CCSequence::create
		(
			CCBlink::create(count,count),
			CCCallFuncND::create(this, callfuncND_selector(LoadLayer::goToMenuLayer), NULL),
			NULL
		)
	);
    return true;
}
void LoadLayer::goToMenuLayer(CCObject* pSender)
{
	//���ű�������
	if(JNIUtil::getInt("musicFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/background.mp3",true);
	}
	tsm->goMenuLayer();
}


