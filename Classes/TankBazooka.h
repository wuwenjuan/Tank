#ifndef _TankBazooka_H_
#define _TankBazooka_H_
#include "cocos2d.h"//����ͷ�ļ�
#include "string.h"//����ͷ�ļ�
using namespace cocos2d;//ʹ�������ռ�
class TankBazooka : public CCSprite
{
public:
	//�����з�̹�˵ķ���
	static TankBazooka *creates(CCPoint enemyPosition,CCPoint bazookaSpeed);
	//�з��ϰ����λ��
	CCPoint enemyPosition;
	//�������ٶ�
	CCPoint bazookaSpeed;
	//����λ�õķ���
	bool platformPosition();
};
#endif
