#ifndef __TankSceneManager_H__
#define __TankSceneManager_H__

#include "cocos2d.h"
using namespace cocos2d;
//���ڴ�����������
class TankSceneManager
{
public:
	//������������ķ���
    CCScene *loadScene;
	CCScene *menuScene;
	CCScene *gameScene;
	CCScene *helpScene;
	//������������ķ���
	void createScene();
	void goMenuLayer();
	void goGameLayer();
    void goHelpLayer();
};

#endif
