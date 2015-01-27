#include "TankBazooka.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

TankBazooka * TankBazooka::creates(CCPoint enemyPosition,CCPoint bazookaSpeed){
	//创建坦克导弹
	TankBazooka *tankBazooka = new TankBazooka();
	//初始化
	if (tankBazooka->initWithFile("tankBazooka.png")){
		//锁定坦克的位置
		tankBazooka->enemyPosition = enemyPosition;
		//坦克导弹的速度
		tankBazooka->bazookaSpeed = bazookaSpeed;
		//设置为自动管理
		tankBazooka->autorelease();
		//返回导弹对象
		return tankBazooka;
	}
	CC_SAFE_DELETE(tankBazooka);//清除资源
	return NULL;
}
bool TankBazooka::platformPosition()
{
	//获取导弹的位置
	CCPoint bazookaPosition = this->getPosition();
	//计算导弹与目标点的连接向量
	CCPoint vector3f = ccp(enemyPosition.x-bazookaPosition.x,enemyPosition.y-bazookaPosition.y);
	if(ccpLength(vector3f)<20)
	{
		return true;
	}
	//规格化向量
	vector3f = ccpNormalize(vector3f);
	//计算导弹的速度
	bazookaSpeed = ccpMult(ccpNormalize(ccp(bazookaSpeed.x+vector3f.x/15,bazookaSpeed.y+vector3f.y/15)),3.5);
	//计算坦克的新位置
	bazookaPosition = ccpAdd(bazookaPosition,bazookaSpeed);
	//计算地方炮塔的旋转角度
	float bazookaAngle = 0;
	//计算角度
	if(bazookaSpeed.x>0)
	{
		bazookaAngle=90-atan(bazookaSpeed.y/bazookaSpeed.x)*180.0/3.1415926;
	}
	else
	{
		bazookaAngle=-atan(bazookaSpeed.y/bazookaSpeed.x)*180.0/3.1415926-90;
	}
	//设置炮弹的旋转角度
	this->setRotation(bazookaAngle);
	//设置炮弹的位置
	this->setPosition(bazookaPosition);
	return false;
}
