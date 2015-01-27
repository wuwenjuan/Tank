#include "AppDelegate.h"
#include "AppMacros.h"
#include "TankSceneManager.h"
#include "SimpleAudioEngine.h"
#include <vector>
#include <string>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

//USING_NS_CC;
using namespace cocos2d;

//���캯��
AppDelegate::AppDelegate(){}

//��������
AppDelegate::~AppDelegate(){}

//��ʼ����Ϸ�е�CCDirector�Լ�CCScene
bool AppDelegate::applicationDidFinishLaunching()
{
    //��ȡCCDirector�����ݣ�
    Director* pDirector = Director::getInstance();
    //��ȡCCEGLView
    //CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    CCEGLView* pEGLView = pDirector->getOpenGLView();
    //Ϊ�������û�����OpenGLView
    pDirector->setOpenGLView(pEGLView);
    //����Ŀ��ֱ��ʳߴ�
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
    //��ȡ����֡�ߴ�
	CCSize frameSize = pEGLView->getFrameSize();
    //������Դ��Ŀ¼����
	std::string rDis(picResource.directory);
	std::vector<std::string> pathV(2);
	pathV.push_back(picResource.directory);
	pathV.push_back(soundResource.directory);
	CCFileUtils::sharedFileUtils()->setSearchPaths(pathV);
	//���ر�������
	initMusic();
	//������������
	pDirector->setContentScaleFactor(1.0);
    //��FPS��ʾ
    pDirector->setDisplayStats(false);
    //����FPS����׼ֵΪ1/60
    pDirector->setAnimationInterval(1.0 /60);
    //������������
    TankSceneManager* tsm=new TankSceneManager();
    tsm->createScene();
    //���ߵ���ʹ���Ǹ�����
    pDirector->runWithScene(tsm->loadScene);
    return true;
}
//�������뿪ǰ̨�������̨ʱ����
void AppDelegate::applicationDidEnterBackground()
{
	//ֹͣ����
    CCDirector::sharedDirector()->stopAnimation();
    //����������Ļ�Ҫ��������һ����ͣ��������
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
//���������ǰ̨ʱ����
void AppDelegate::applicationWillEnterForeground()
{
	//��ʼ����
    CCDirector::sharedDirector()->startAnimation();
    //����������Ļ�Ҫ��������һ�俪ʼ��������
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
//���ر������ֵķ���
void AppDelegate::initMusic()
{
	//���ر�������
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic
	(
			"sound/background.mp3"
	);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect
	(
		"sound/grenada.ogg"
	);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect
	(
		"sound/lose.mp3"
	);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect
	(
		"sound/rocket_shoot2.ogg"
	);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect
	(
		"sound/select.wav"
	);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect
	(
		"sound/eatprops.wav"
	);
}
