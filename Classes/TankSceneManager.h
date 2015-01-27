#ifndef __TankSceneManager_H__
#define __TankSceneManager_H__

#include "cocos2d.h"
using namespace cocos2d;
//用于创建场景的类
class TankSceneManager
{
public:
	//创建场景对象的方法
    CCScene *loadScene;
	CCScene *menuScene;
	CCScene *gameScene;
	CCScene *helpScene;
	//创建场景对象的方法
	void createScene();
	void goMenuLayer();
	void goGameLayer();
    void goHelpLayer();
};

#endif
