#ifndef _DialogLayer_h
#define _DialogLayer_h
#include "cocos2d.h"//����ͷ�ļ�
#include "AppMacros.h"//����ͷ�ļ�
using namespace cocos2d;//ʹ�������ռ�
class DialogLayer: public CCLayerGradient{
public:
	CCMenu *m_pMenu;//���˵�����
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
    //���ڳ�ȥʱ����ִ����֮��Ļص�����
    void returnGameLayer();
    //���ڽ��붯��ִ����֮��Ļص�����
    void resumeMenu();
    CREATE_FUNC(DialogLayer);
};
#endif
