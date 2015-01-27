#ifndef _EnemyTank_H_
#define _EnemyTank_H_
#include "cocos2d.h"//����ͷ�ļ�
#include "string.h"//����ͷ�ļ�
using namespace cocos2d;//ʹ�������ռ�
class EnemyTank : public CCSprite
{
public:
	//�����з�̹�˵ķ���
	static EnemyTank *creates( std::string pname,int tankIndex,int enemyTankBlood);
	//�ж��Ƿ���ײ
	bool isCollision(CCSprite *tankBullet);
	//̹�˱��
	int tankIndex;
	//̹��Ѫ��
	int enemyTankBlood;
	//�ҷ�̹�˵Ĵ�С
	CCSize enemySize;
};
#endif
