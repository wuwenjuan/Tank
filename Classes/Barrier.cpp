#include "Barrier.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

Barrier * Barrier::creates( std::string pname,int barrierIndex,int barrierBlood){//敌机类
	//创建敌方坦克
	Barrier *barrier = new Barrier();
	if (barrier->initWithFile(pname.c_str())){
		//初始障碍物的类型信息
		barrier->barrierIndex = barrierIndex;
		//障碍物的血量
		barrier->barrierBlood = barrierBlood;
		//设置为系统自动管理
		barrier->autorelease();
		//获取图片大小
		barrier->barrierSize = barrier->getContentSize();
		return barrier;
	}
	CC_SAFE_DELETE(barrier);//清除资源
	return NULL;
}
//检测发生碰撞的方法
bool Barrier::isCollision(CCSprite *tankOrBullet)
{
	//获取子弹的位置
	CCPoint tankOrBulletPositon=tankOrBullet->getPosition();
	//获取敌方坦克的位置
	CCPoint barrierPosition =this->getPosition();
	//获取敌方坦克的大小
	CCSize tankOrBulletSize = tankOrBullet->getContentSize();
	float width = tankOrBulletSize.width+barrierSize.width;
	float height = tankOrBulletSize.height+barrierSize.height;
	//进行碰撞判断
	if(abs(tankOrBulletPositon.x-barrierPosition.x)<=width/2&&abs(tankOrBulletPositon.y-barrierPosition.y)<=height/2)
	{
		return true;
	}
	return false;
}
