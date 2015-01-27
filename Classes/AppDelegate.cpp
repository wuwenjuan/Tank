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

//构造函数
AppDelegate::AppDelegate(){}

//析构函数
AppDelegate::~AppDelegate(){}

//初始化游戏中的CCDirector以及CCScene
bool AppDelegate::applicationDidFinishLaunching()
{
    //获取CCDirector（导演）
    Director* pDirector = Director::getInstance();
    //获取CCEGLView
    //CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    CCEGLView* pEGLView = pDirector->getOpenGLView();
    //为导演设置绘制用OpenGLView
    pDirector->setOpenGLView(pEGLView);
    //设置目标分辨率尺寸
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
    //获取画面帧尺寸
	CCSize frameSize = pEGLView->getFrameSize();
    //设置资源子目录序列
	std::string rDis(picResource.directory);
	std::vector<std::string> pathV(2);
	pathV.push_back(picResource.directory);
	pathV.push_back(soundResource.directory);
	CCFileUtils::sharedFileUtils()->setSearchPaths(pathV);
	//加载背景音乐
	initMusic();
	//设置缩放因子
	pDirector->setContentScaleFactor(1.0);
    //打开FPS显示
    pDirector->setDisplayStats(false);
    //设置FPS，标准值为1/60
    pDirector->setAnimationInterval(1.0 /60);
    //创建场景对象
    TankSceneManager* tsm=new TankSceneManager();
    tsm->createScene();
    //告诉导演使用那个场景
    pDirector->runWithScene(tsm->loadScene);
    return true;
}
//当程序离开前台，进入后台时调用
void AppDelegate::applicationDidEnterBackground()
{
	//停止动画
    CCDirector::sharedDirector()->stopAnimation();
    //如果有声音的话要调用下面一句暂停声音播放
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
//当程序进入前台时调用
void AppDelegate::applicationWillEnterForeground()
{
	//开始动画
    CCDirector::sharedDirector()->startAnimation();
    //如果有声音的话要调用下面一句开始声音播放
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
//加载背景音乐的方法
void AppDelegate::initMusic()
{
	//加载背景音乐
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
