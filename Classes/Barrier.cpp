#include "Barrier.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

Barrier * Barrier::creates( std::string pname,int barrierIndex,int barrierBlood){//�л���
	//�����з�̹��
	Barrier *barrier = new Barrier();
	if (barrier->initWithFile(pname.c_str())){
		//��ʼ�ϰ����������Ϣ
		barrier->barrierIndex = barrierIndex;
		//�ϰ����Ѫ��
		barrier->barrierBlood = barrierBlood;
		//����Ϊϵͳ�Զ�����
		barrier->autorelease();
		//��ȡͼƬ��С
		barrier->barrierSize = barrier->getContentSize();
		return barrier;
	}
	CC_SAFE_DELETE(barrier);//�����Դ
	return NULL;
}
//��ⷢ����ײ�ķ���
bool Barrier::isCollision(CCSprite *tankOrBullet)
{
	//��ȡ�ӵ���λ��
	CCPoint tankOrBulletPositon=tankOrBullet->getPosition();
	//��ȡ�з�̹�˵�λ��
	CCPoint barrierPosition =this->getPosition();
	//��ȡ�з�̹�˵Ĵ�С
	CCSize tankOrBulletSize = tankOrBullet->getContentSize();
	float width = tankOrBulletSize.width+barrierSize.width;
	float height = tankOrBulletSize.height+barrierSize.height;
	//������ײ�ж�
	if(abs(tankOrBulletPositon.x-barrierPosition.x)<=width/2&&abs(tankOrBulletPositon.y-barrierPosition.y)<=height/2)
	{
		return true;
	}
	return false;
}
