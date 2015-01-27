#include "TankBazooka.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

TankBazooka * TankBazooka::creates(CCPoint enemyPosition,CCPoint bazookaSpeed){
	//����̹�˵���
	TankBazooka *tankBazooka = new TankBazooka();
	//��ʼ��
	if (tankBazooka->initWithFile("tankBazooka.png")){
		//����̹�˵�λ��
		tankBazooka->enemyPosition = enemyPosition;
		//̹�˵������ٶ�
		tankBazooka->bazookaSpeed = bazookaSpeed;
		//����Ϊ�Զ�����
		tankBazooka->autorelease();
		//���ص�������
		return tankBazooka;
	}
	CC_SAFE_DELETE(tankBazooka);//�����Դ
	return NULL;
}
bool TankBazooka::platformPosition()
{
	//��ȡ������λ��
	CCPoint bazookaPosition = this->getPosition();
	//���㵼����Ŀ������������
	CCPoint vector3f = ccp(enemyPosition.x-bazookaPosition.x,enemyPosition.y-bazookaPosition.y);
	if(ccpLength(vector3f)<20)
	{
		return true;
	}
	//�������
	vector3f = ccpNormalize(vector3f);
	//���㵼�����ٶ�
	bazookaSpeed = ccpMult(ccpNormalize(ccp(bazookaSpeed.x+vector3f.x/15,bazookaSpeed.y+vector3f.y/15)),3.5);
	//����̹�˵���λ��
	bazookaPosition = ccpAdd(bazookaPosition,bazookaSpeed);
	//����ط���������ת�Ƕ�
	float bazookaAngle = 0;
	//����Ƕ�
	if(bazookaSpeed.x>0)
	{
		bazookaAngle=90-atan(bazookaSpeed.y/bazookaSpeed.x)*180.0/3.1415926;
	}
	else
	{
		bazookaAngle=-atan(bazookaSpeed.y/bazookaSpeed.x)*180.0/3.1415926-90;
	}
	//�����ڵ�����ת�Ƕ�
	this->setRotation(bazookaAngle);
	//�����ڵ���λ��
	this->setPosition(bazookaPosition);
	return false;
}
