#ifndef _Barrier_H_
#define _Barrier_H_
#include "cocos2d.h"
#include "string.h"
using namespace cocos2d;
class Barrier : public CCSprite
{
public:
	//创建障碍物的方法
	static Barrier *creates( std::string pname,int index,int barrierBlood);
	//判独是否碰撞
	bool isCollision(CCSprite *tankOrBullet);
	//障碍物编号
	int barrierIndex;
	//障碍物血量
	int barrierBlood;
	//障碍物的大小
	CCSize barrierSize;
};
#endif
