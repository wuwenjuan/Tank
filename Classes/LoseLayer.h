#ifndef _LoseLayer_h
#define _LoseLayer_h
#include "cocos2d.h"//����ͷ�ļ�
#include "AppMacros.h"//����ͷ�ļ�
using namespace cocos2d;//ʹ�������ռ�
class LoseLayer: public CCLayerGradient{
public:
	CCMenu *m_pMenu;//���˵�����
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
    //���ڽ��붯��ִ����֮��Ļص�����
    void resumeMenu();
    CREATE_FUNC(LoseLayer);
};
#endif
