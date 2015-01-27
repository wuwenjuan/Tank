#include "Props.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

Props *Props::creates(std::string pname,int propsIndex){//�л���
	//��������
	Props *props = new Props();
	if (props->initWithFile(pname.c_str())){
		//��ʼ���ߵ�������Ϣ
		props->propsIndex = propsIndex;
		//����Ϊϵͳ�Զ�����
		props->autorelease();
		//��ȡͼƬ��С
		props->propsSize = props->getContentSize();
		return props;
	}
	//�����Դ
	CC_SAFE_DELETE(props);
	return NULL;
}
//��ⷢ����ײ�ķ���
bool Props::isCollision(CCSprite *tank)
{
	//��ȡ̹�˵�λ��
	CCPoint tankPosition=tank->getPosition();
	//��ȡ���ߵ�λ��
	CCPoint propsPosition =this->getPosition();
	//��ȡ�ҷ�̹�˵Ĵ�С
	CCSize tankSize = tank->getContentSize();
	//������
	float width = tankSize.width+propsSize.width;
	//����߶�
	float height = tankSize.height+propsSize.height;
	//������ײ�ж�
	if (abs(tankPosition.x-propsPosition.x)<=width/2&&abs(tankPosition.y-propsPosition.y)<=height/2){
		return true;
	}
	return false;
}
