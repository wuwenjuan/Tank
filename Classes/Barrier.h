#ifndef _Barrier_H_
#define _Barrier_H_
#include "cocos2d.h"
#include "string.h"
using namespace cocos2d;
class Barrier : public CCSprite
{
public:
	//�����ϰ���ķ���
	static Barrier *creates( std::string pname,int index,int barrierBlood);
	//�ж��Ƿ���ײ
	bool isCollision(CCSprite *tankOrBullet);
	//�ϰ�����
	int barrierIndex;
	//�ϰ���Ѫ��
	int barrierBlood;
	//�ϰ���Ĵ�С
	CCSize barrierSize;
};
#endif
