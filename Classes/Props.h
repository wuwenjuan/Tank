#ifndef _Props_H_
#define _Props_H_
#include "cocos2d.h"
#include "string.h"
using namespace cocos2d;
class Props : public CCSprite
{
public:
	//创建敌方坦克的方法
	static Props *creates( std::string pname,int propsIndex);
	//判独是否碰撞
	bool isCollision(CCSprite *tank);
	//坦克编号
	int propsIndex;
	//道具的大小
	CCSize propsSize;
};
#endif
