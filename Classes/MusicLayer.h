#ifndef _MusicLayer_h
#define _MusicLayer_h
#include "cocos2d.h"//导入头文件
#include "AppMacros.h"//导入头文件
using namespace cocos2d;
class MusicLayer: public CCLayerGradient{
public:
	CCMenu* pMenu;
	//创建声音按钮的精灵
	CCMenuItemImage* soundon;
	//创建声音按钮的精灵
	CCMenuItemImage* soundclose;
	//创建音效按钮的精灵
	CCMenuItemImage* effecton;
	//创建音效按钮的精灵
	CCMenuItemImage* effectclose;

public:
    MusicLayer();
    ~MusicLayer();
    virtual bool init();
    void initDialog();


    void menuCallbackKeyBackClicked(cocos2d::CCObject *pSender);
    //层在进入动作执行完之后的回调方法
    void resumeMenu();
    //打开声音
    void  menuCallbacKOpenMusic();
    //关闭声音
    void  menuCallbackCloseMusic();
    //打开音效
    void  menuCallbackOpengEffect();
    //关闭音效
    void  menuCallbackCloseEffect();
    //返回菜单
    void  menuCallbacKToMenuLayer();
    CREATE_FUNC(MusicLayer);
};
#endif
