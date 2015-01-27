#include "EnemyTank.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

EnemyTank * EnemyTank::creates( std::string pname,int tankIndex,int enemyTankBlood){//敌机类
	//创建敌方坦克
	EnemyTank *tankEnemy = new EnemyTank();
	if (tankEnemy->initWithFile(pname.c_str())){
		//初始敌机的类型信息
		tankEnemy->tankIndex = tankIndex;
		//敌机血量
		tankEnemy->enemyTankBlood = enemyTankBlood;
		//设置为系统自动管理
		tankEnemy->autorelease();
		//获取图片大小
		tankEnemy->enemySize = tankEnemy->getContentSize();
		return tankEnemy;
	}
	CC_SAFE_DELETE(tankEnemy);//清除资源
	return NULL;
}
//检测发生碰撞的方法
bool EnemyTank::isCollision(CCSprite *tankBullet)
{
	//获取子弹的位置
	CCPoint bulletPositon=tankBullet->getPosition();
	//获取敌方坦克的位置
	CCPoint enemyTankPosition =this->getPosition();
	//获取敌方坦克的大小
	CCSize BulletSize = tankBullet->getContentSize();
	float width = BulletSize.width+enemySize.width;
	float height = BulletSize.height+enemySize.height;
	//进行碰撞判断
	if (abs(bulletPositon.x-enemyTankPosition.x)<=width/2&&abs(bulletPositon.y-enemyTankPosition.y)<=height/2){
		return true;
	}
	return false;
}
