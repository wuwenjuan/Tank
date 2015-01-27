#ifndef _TankBazooka_H_
#define _TankBazooka_H_
#include "cocos2d.h"//导入头文件
#include "string.h"//导入头文件
using namespace cocos2d;//使用命名空间
class TankBazooka : public CCSprite
{
public:
	//创建敌方坦克的方法
	static TankBazooka *creates(CCPoint enemyPosition,CCPoint bazookaSpeed);
	//敌方障碍物的位置
	CCPoint enemyPosition;
	//导弹的速度
	CCPoint bazookaSpeed;
	//计算位置的方法
	bool platformPosition();
};
#endif
