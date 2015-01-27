#ifndef _EnemyTankGun_H_
#define _EnemyTankGun_H_
#include "cocos2d.h"//导入头文件
#include "string.h"//导入头文件
using namespace cocos2d;//使用命名空间
class EnemyTankGun : public CCSprite
{
public:
	//创建敌方坦克的方法
	static EnemyTankGun *creates(std::string pname,int index);
	//特效播放的标志位
	bool enemyIsAnimation;
	//炮管种类索引
	int index;
};
#endif
