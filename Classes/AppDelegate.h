#ifndef  _AppDelegate_H_
#define  _AppDelegate_H_

#include "cocos2d.h"

//自己写的cocos2dx应用程序要继承CCApplication
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    //初始化游戏中的CCDirector以及CCScene
    virtual bool applicationDidFinishLaunching();
    //当程序进入后台时调用此方法
    virtual void applicationDidEnterBackground();
    //当程序进入前台时调用此方法
    virtual void applicationWillEnterForeground();
    //加载背景音乐的方法
    void initMusic();
};

#endif
