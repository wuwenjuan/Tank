#include "Props.h"
#include "cocos2d.h"
#include "AppMacros.h"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

Props *Props::creates(std::string pname,int propsIndex){//敌机类
	//创建道具
	Props *props = new Props();
	if (props->initWithFile(pname.c_str())){
		//初始道具的类型信息
		props->propsIndex = propsIndex;
		//设置为系统自动管理
		props->autorelease();
		//获取图片大小
		props->propsSize = props->getContentSize();
		return props;
	}
	//清除资源
	CC_SAFE_DELETE(props);
	return NULL;
}
//检测发生碰撞的方法
bool Props::isCollision(CCSprite *tank)
{
	//获取坦克的位置
	CCPoint tankPosition=tank->getPosition();
	//获取道具的位置
	CCPoint propsPosition =this->getPosition();
	//获取我方坦克的大小
	CCSize tankSize = tank->getContentSize();
	//计算宽度
	float width = tankSize.width+propsSize.width;
	//计算高度
	float height = tankSize.height+propsSize.height;
	//进行碰撞判断
	if (abs(tankPosition.x-propsPosition.x)<=width/2&&abs(tankPosition.y-propsPosition.y)<=height/2){
		return true;
	}
	return false;
}
