#ifndef  _LoadLayer_H_
#define  _LoadLayer_H_
#include "cocos2d.h"
#include "TankSceneManager.h"
using namespace cocos2d;
class LoadLayer : public CCLayer{
	public:
	//��ʼ���ķ���
    virtual bool init();
	//����������
    TankSceneManager *tsm;
    //��ת���˵�����ķ���
    void goToMenuLayer(CCObject* pSender);
    CREATE_FUNC(LoadLayer);
};
#endif
