#ifndef _EnemyTank_H_
#define _EnemyTank_H_
#include "cocos2d.h"//导入头文件
#include "string.h"//导入头文件
using namespace cocos2d;//使用命名空间
class EnemyTank : public CCSprite
{
public:
	//创建敌方坦克的方法
	static EnemyTank *creates( std::string pname,int tankIndex,int enemyTankBlood);
	//判独是否碰撞
	bool isCollision(CCSprite *tankBullet);
	//坦克编号
	int tankIndex;
	//坦克血量
	int enemyTankBlood;
	//我放坦克的大小
	CCSize enemySize;
};
#endif
