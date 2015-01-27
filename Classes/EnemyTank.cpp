#include "EnemyTank.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

EnemyTank * EnemyTank::creates( std::string pname,int tankIndex,int enemyTankBlood){//�л���
	//�����з�̹��
	EnemyTank *tankEnemy = new EnemyTank();
	if (tankEnemy->initWithFile(pname.c_str())){
		//��ʼ�л���������Ϣ
		tankEnemy->tankIndex = tankIndex;
		//�л�Ѫ��
		tankEnemy->enemyTankBlood = enemyTankBlood;
		//����Ϊϵͳ�Զ�����
		tankEnemy->autorelease();
		//��ȡͼƬ��С
		tankEnemy->enemySize = tankEnemy->getContentSize();
		return tankEnemy;
	}
	CC_SAFE_DELETE(tankEnemy);//�����Դ
	return NULL;
}
//��ⷢ����ײ�ķ���
bool EnemyTank::isCollision(CCSprite *tankBullet)
{
	//��ȡ�ӵ���λ��
	CCPoint bulletPositon=tankBullet->getPosition();
	//��ȡ�з�̹�˵�λ��
	CCPoint enemyTankPosition =this->getPosition();
	//��ȡ�з�̹�˵Ĵ�С
	CCSize BulletSize = tankBullet->getContentSize();
	float width = BulletSize.width+enemySize.width;
	float height = BulletSize.height+enemySize.height;
	//������ײ�ж�
	if (abs(bulletPositon.x-enemyTankPosition.x)<=width/2&&abs(bulletPositon.y-enemyTankPosition.y)<=height/2){
		return true;
	}
	return false;
}
