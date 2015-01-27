#ifndef _NextLevelDialogLayer_h
#define _NextLevelDialogLayer_h
#include "cocos2d.h"
#include "AppMacros.h"
using namespace cocos2d;
class NextLevelDialogLayer: public CCLayerGradient{
public:
	CCMenu *m_pMenu;//���˵�����
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
    //���ڳ�ȥʱ����ִ����֮��Ļص�����
    void returnGameLayer();
    //���ڽ��붯��ִ����֮��Ļص�����
    void resumeMenu();
    CREATE_FUNC(NextLevelDialogLayer);
};
#endif
