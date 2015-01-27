#include "LoadLayer.h"
#include <string.h>
#include "SimpleAudioEngine.h"
#include "JNIUtil.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
using namespace cocos2d;
//实现LoadLayer类中的init方法，初始化布景
bool LoadLayer::init(){
	//调用父类的初始化
    if ( !CCLayer::init()){
        return false;
    }
	//获取可见区域尺寸
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//获取可见区域原点坐标
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    //添加背景精灵
    CCSprite* background = CCSprite::create("gameBackground.png");
    //设置精灵的位置
    background->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
    //将精灵添加到布景
    this->addChild(background);
    //添加背景精灵
	CCSprite* decoration = CCSprite::create("decoration.png");
	//设置精灵的位置
	decoration->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/3*2));
	//将精灵添加到布景
	this->addChild(decoration);
    //创建加载精灵
    CCSprite* load = CCSprite::create("load.png");
    //设置加载精灵的位置
    load->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height/5));
    //想颜色层添加精灵
    this->addChild(load);
    //产生随机数
    int count = rand()%3+1;
    //文字精灵执行动作
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
	//播放背景音乐
	if(JNIUtil::getInt("musicFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/background.mp3",true);
	}
	tsm->goMenuLayer();
}


