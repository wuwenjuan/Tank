#include "EnemyTankGun.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

EnemyTankGun * EnemyTankGun::creates(std::string pname,int index){//�л���
	//�����з�̹��
	EnemyTankGun *enemytankGun = new EnemyTankGun();
	//����ͼƬ�����з�̹��
	if (enemytankGun->initWithFile(pname.c_str(),CCRectMake(0,0,33,68)))
	{
		//��ʼ��������Ч�ı�־λ
		enemytankGun->enemyIsAnimation = false;
		//�ڹ�����
		enemytankGun->index = index;
		//����Ϊϵͳ�Զ�����
		enemytankGun->autorelease();
		//���ض���
		return enemytankGun;
	}
	//�����Դ
	CC_SAFE_DELETE(enemytankGun);
	return NULL;
}
