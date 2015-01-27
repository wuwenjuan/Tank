#ifndef _Props_H_
#define _Props_H_
#include "cocos2d.h"
#include "string.h"
using namespace cocos2d;
class Props : public CCSprite
{
public:
	//�����з�̹�˵ķ���
	static Props *creates( std::string pname,int propsIndex);
	//�ж��Ƿ���ײ
	bool isCollision(CCSprite *tank);
	//̹�˱��
	int propsIndex;
	//���ߵĴ�С
	CCSize propsSize;
};
#endif
