#ifndef _LoseLayer_h
#define _LoseLayer_h
#include "cocos2d.h"//导入头文件
#include "AppMacros.h"//导入头文件
using namespace cocos2d;//使用命名空间
class LoseLayer: public CCLayerGradient{
public:
	CCMenu *m_pMenu;//主菜单界面
public:
    LoseLayer();
    ~LoseLayer();
    virtual bool init();
    void initDialog();
    void onEnter();
    void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void menuCallbackExitGame(cocos2d::CCObject *pSender);
    void menuCallbackGoMenuLayer(cocos2d::CCObject *pSender);
    //层在进入动作执行完之后的回调方法
    void resumeMenu();
    CREATE_FUNC(LoseLayer);
};
#endif
