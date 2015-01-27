#ifndef  _MenuLayer_H_
#define  _MenuLayer_H_
#include "cocos2d.h"
#include "TankSceneManager.h"
using namespace cocos2d;
class MenuLayer : public CCLayer{
public:
	//初始化的方法
	virtual bool init();
	//返回键监听
	//跳转到游戏界面的方法
	void goToGameLayer(CCObject* pSender);
	//跳转到帮助界面的方法
	void goToHelpLayer(CCObject* pSender);
	//退出的回调方法
	void musicLayer(CCObject* pSender);
	//退出游戏
	void exitGame(CCObject* pSender);
	//开始游戏按钮动作播放完毕之后的的回调方法
	void setMenuEnabled(CCObject* pSender);
	//所属管理者
	TankSceneManager *tsm;
	//按钮菜单
	CCMenu* pMenu;
	CREATE_FUNC(MenuLayer);
};
#endif
