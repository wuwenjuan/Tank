#ifndef _EnemyTankGun_H_
#define _EnemyTankGun_H_
#include "cocos2d.h"//����ͷ�ļ�
#include "string.h"//����ͷ�ļ�
using namespace cocos2d;//ʹ�������ռ�
class EnemyTankGun : public CCSprite
{
public:
	//�����з�̹�˵ķ���
	static EnemyTankGun *creates(std::string pname,int index);
	//��Ч���ŵı�־λ
	bool enemyIsAnimation;
	//�ڹ���������
	int index;
};
#endif
