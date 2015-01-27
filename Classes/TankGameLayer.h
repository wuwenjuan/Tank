#ifndef  _TankGameLayer_H_
#define  _TankGameLayer_H_
#include "SneakyJoystickSkinnedBase.h"
#include "cocos2d.h"
#include "string.h"
using namespace cocos2d;
//自定义的布景类
class TankGameLayer : public CCLayer
{
public:
	//初始化的方法
	virtual bool init();
	//初始化背景的方法
	void initBackGround();
	//初始化所有的炮塔的帧动画
	void initGunsAnimation();
	//初始化炮弹帧动画的数组
	CCAnimation* initExplosionAnimation(std::string pname,int span);
	//初始化炮管帧动画的方法
	CCAnimation* initGunAnimation(std::string pname);
	//增加与减少血量的方法
	bool changeBlood(int blood);
	//增加分数的方法
	void addScore(int scoreIncrement);
	//菜单关闭回调方法
public:
	//背景滚动的回调方法
	void updateBackGround(float dt);
	//添加背景树的方法
	void addTrees(float dt);
	//增加一组背景装饰精灵的方法
	void addOneGroupTree();
public:
	//背景装饰图片的名字
	std::string pBName[3];
	//道具图片的名字
	std::string pPName[5];
	//敌方坦克与障碍物的名字
	std::string enemyName[5];
	//数字图片数字
	std::string num[10];
	//获取可见区域尺寸
	CCSize visibleSize;
	//获取可见区域原点坐标
	CCPoint origin;
	//背景精灵
	CCSprite *backSprite1;
	//背景精灵
	CCSprite *backSprite2;
	//坦克精灵
	CCSprite* tankBody;
	//炮管精灵
	CCSprite* tankGun;
	//创建分数图标
	CCSprite* score_icon;
    //炮塔的帧动画数组
	CCAnimation** tankGunAnmi;
    //炮弹的帧动画数组
	CCAnimation** tankBulletAnmi;
	//我方子弹
	CCSpriteBatchNode* tankBullet;
	//我方导弹
	CCSpriteBatchNode* tankBazooka;
	//敌方子弹
	CCSpriteBatchNode* enemyBullet;
	//敌方导弹
	CCSpriteBatchNode* enemyBazooka;
	//灌木列表
	CCNode* trees ;
	//炸痕列表
	CCNode* marks ;
	//数量列表
	CCNode* nums;
	//血量
	CCNode* bloods;
	//敌方坦克数组
	CCArray *enemyTankArray;
	//敌方障碍物数组
	CCArray *barrierArray;
	//道具数组
	CCArray *propsArray;
	//分数
	int scores;
	//关卡
	int passTemp;
	//子弹状态
	int SHOOT_STATE;
	//敌人组数量
	int enemyGroupCount;
	//当前敌人组
	int currentEnemyGroup;
	//坦克血量
	int tankBlood;
	//并做好相应的初始化与释放工作
	CREATE_FUNC(TankGameLayer);
};

#endif
