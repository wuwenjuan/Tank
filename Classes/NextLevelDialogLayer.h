#ifndef _NextLevelDialogLayer_h
#define _NextLevelDialogLayer_h
#include "cocos2d.h"
#include "AppMacros.h"
using namespace cocos2d;
class NextLevelDialogLayer: public CCLayerGradient{
public:
	CCMenu *m_pMenu;//主菜单界面
public:
    NextLevelDialogLayer();
    ~NextLevelDialogLayer();
    virtual bool init();
    void initDialog();
    void onEnter();
    void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void menuCallbackNextLevel(cocos2d::CCObject *pSender);
    void menuCallbackGoMenuLayer(cocos2d::CCObject *pSender);
    //层在出去时动作执行完之后的回调方法
    void returnGameLayer();
    //层在进入动作执行完之后的回调方法
    void resumeMenu();
    CREATE_FUNC(NextLevelDialogLayer);
};
#endif
