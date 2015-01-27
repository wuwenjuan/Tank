#include "EnemyTankGun.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

EnemyTankGun * EnemyTankGun::creates(std::string pname,int index){//敌机类
	//创建敌方坦克
	EnemyTankGun *enemytankGun = new EnemyTankGun();
	//根据图片创建敌方坦克
	if (enemytankGun->initWithFile(pname.c_str(),CCRectMake(0,0,33,68)))
	{
		//初始化动作特效的标志位
		enemytankGun->enemyIsAnimation = false;
		//炮管索引
		enemytankGun->index = index;
		//设置为系统自动管理
		enemytankGun->autorelease();
		//返回对象
		return enemytankGun;
	}
	//清除资源
	CC_SAFE_DELETE(enemytankGun);
	return NULL;
}
