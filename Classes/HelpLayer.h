#ifndef  _HelpLayer_H_
#define  _HelpLayer_H_
#include "cocos2d.h"
#include "TankSceneManager.h"
using namespace cocos2d;
//�Զ���Ĳ�����
class HelpLayer : public CCLayer
{
public:
	 //��ʼ���ķ���
    virtual bool init();
    //���ؼ�����
	//ȷ����ť�Ļص�����
	void goToMenuLayer();
	//��������Ӧ�ĳ�ʼ�����ͷŹ���
    CREATE_FUNC(HelpLayer);
public:
    //��ť����
    CCMenu* pMenu;
	//��������������
	TankSceneManager* tsm;
	//�ɼ�����
    cocos2d::CCSize visibleSize;
    //ԭ������
    cocos2d::CCPoint origin;
    //��������
    cocos2d::CCSprite* backGround;
};
#endif
