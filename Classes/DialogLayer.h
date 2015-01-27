#ifndef _DialogLayer_h
#define _DialogLayer_h
#include "cocos2d.h"//导入头文件
#include "AppMacros.h"//导入头文件
using namespace cocos2d;//使用命名空间
class DialogLayer: public CCLayerGradient{
public:
	CCMenu *m_pMenu;//主菜单界面
public:
    DialogLayer();
    ~DialogLayer();
    virtual bool init();
    void initDialog();
    void onEnter();
    void onExit();

    void menuCallbackExitGame(cocos2d::CCObject *pSender);
    void menuCallbackContinueGame(cocos2d::CCObject *pSender);
    void menuCallbackGoMenuLayer(cocos2d::CCObject *pSender);
    //层在出去时动作执行完之后的回调方法
    void returnGameLayer();
    //层在进入动作执行完之后的回调方法
    void resumeMenu();
    CREATE_FUNC(DialogLayer);
};
#endif
