#ifndef  _HelpLayer_H_
#define  _HelpLayer_H_
#include "cocos2d.h"
#include "TankSceneManager.h"
using namespace cocos2d;
//自定义的布景类
class HelpLayer : public CCLayer
{
public:
	 //初始化的方法
    virtual bool init();
    //返回键监听
	//确定按钮的回调方法
	void goToMenuLayer();
	//并做好相应的初始化与释放工作
    CREATE_FUNC(HelpLayer);
public:
    //按钮引用
    CCMenu* pMenu;
	//场景管理者引用
	TankSceneManager* tsm;
	//可见区域
    cocos2d::CCSize visibleSize;
    //原点坐标
    cocos2d::CCPoint origin;
    //背景精灵
    cocos2d::CCSprite* backGround;
};
#endif
