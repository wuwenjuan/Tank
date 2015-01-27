#ifndef _MusicLayer_h
#define _MusicLayer_h
#include "cocos2d.h"//����ͷ�ļ�
#include "AppMacros.h"//����ͷ�ļ�
using namespace cocos2d;
class MusicLayer: public CCLayerGradient{
public:
	CCMenu* pMenu;
	//����������ť�ľ���
	CCMenuItemImage* soundon;
	//����������ť�ľ���
	CCMenuItemImage* soundclose;
	//������Ч��ť�ľ���
	CCMenuItemImage* effecton;
	//������Ч��ť�ľ���
	CCMenuItemImage* effectclose;

public:
    MusicLayer();
    ~MusicLayer();
    virtual bool init();
    void initDialog();


    void menuCallbackKeyBackClicked(cocos2d::CCObject *pSender);
    //���ڽ��붯��ִ����֮��Ļص�����
    void resumeMenu();
    //������
    void  menuCallbacKOpenMusic();
    //�ر�����
    void  menuCallbackCloseMusic();
    //����Ч
    void  menuCallbackOpengEffect();
    //�ر���Ч
    void  menuCallbackCloseEffect();
    //���ز˵�
    void  menuCallbacKToMenuLayer();
    CREATE_FUNC(MusicLayer);
};
#endif
