#ifndef  _ControlLayer_H_
#define  _ControlLayer_H_
#include "SneakyJoystickSkinnedBase.h"
#include "TankSceneManager.h"
#include "TankGameLayer.h"
#include "cocos2d.h"
using namespace cocos2d;
//自定义的布景类
class ControlLayer : public CCLayer
{
public:
	//初始化的方法
    virtual bool init();
    //返回键监听
	//初始化控制坦克的遥感的方法
	void initTankJoyStick();
	//初始化控制炮塔的遥感的方法
	void initGunJoyStick();
	//初始化界面中的按钮的方法
	void initButton();
	//移除我方坦克子弹的方法
	void removeTankBullet(CCNode* node,void* param);
	//添加导弹的方法
	void removeTankBazooka(CCNode* node);
	//移除敌方坦克子弹的方法
	void removeEnemyTankBullet(CCNode* node,void* param);
	//移除敌方坦克炮弹的方法
	void removeEnemyTankBazooka(CCNode* node,void* param);
	//移除敌方坦克的方法
	void removeEnemyTank(CCNode* node,void* param);
	//移除爆炸精灵的方法
	void removeBomb(CCNode* node,void* param);
	//移除道具精灵
	void removeProps(CCNode* node,void* param);
	//移除敌方坦克炮塔的方法
	void removeEnemyTankGun(CCNode* node,void* param);
	//移除障碍物的方法
	void removeBarrier(CCNode* node,void* param);
	//重置炮塔旋转标志位的方法
	void setGunMoveFlag(CCObject* pSender);
	//我方坦克与敌方坦克碰撞的方法
	void tank_enemyTank();
	//我方坦克与障碍物碰撞的方法
	void tank_barrier(CCArray* barrierArray);
	//我方坦克与吃到道具执行的方法
	void tank_props(CCArray* propsArray);
	//我方子弹与障碍物碰撞的方法
	void tankBullet_enemy(CCSpriteBatchNode* bulletNode,int bolldReduce);
	//我方子弹与敌方坦克碰撞的方法
	void tankBullet_enemyTank(CCSpriteBatchNode* bulletNode,int bolldReduce);
	//敌方子弹与我方坦克碰撞的方法
	void enemyTankBullet_tank(CCSpriteBatchNode* enemyBulletNode,int bloodReduce);
	//菜单关闭回调方法
	void menuPauseCallback(CCObject* pSender);
	//成功过关之后的工作
	void reparation_for_next_level();
	//下一关的方法
	void nextLevel();
	//跳转到失败界面
	void loseView();

public:
	//添加敌方坦克的方法
	void addEnemyTankGroup_ONE();
	void addEnemyTankGroup_TWO();
	void addEnemyTankGroup_THREE();
	void addEnemyTankGroup_FORE();
	void addEnemyTankGroup_FIVE();
	void addEnemyTankGroup_SIX();
	void addEnemyTankGroup_SEVEN();
	void addEnemyTankGroup_EIGHT();
	void addEnemyTankGroup_NINE();
	void addEnemyTankGroup_TEN();
public:
	//添加一个敌方坦克或障碍物的方法的方法
	void addEnemyTank_ONE(CCPoint position);
	void addEnemyTank_TWO(CCPoint position);
	void addEnemyTank_THREE(CCPoint position);
	void addEnemyTank_FORE(CCPoint position);
	void addBarrier(CCPoint position);
	void addTower(CCPoint position);

public:
	//更新坦克与炮塔位置的方法
	void updateTank();
	//更新炮塔角度的方法
	void updateGun(float dt);
	//更新炮塔角度的方法
	void updateEnemyGun(float dt);
	//坦克发射炮弹的方法
	void tankShoot(float dt);
	//添加导弹的方法
	void addTankBullet();
	//添加导弹的方法
	void addtankBazooka(float dt);
	//计算导弹的旋转角度
	void calculateBazookaAngle();
	//添加道具的方法
	void addProps(CCPoint position);
	//添加爆炸精灵的方法
	void addBomb(CCPoint position,float time,int animIndex);
	CCPoint getRandEnemyPosition();
	//添加炸痕精灵的方法
	void addMarks(CCPoint position,float time);
	//定时添加敌方坦克的方法
	void addEnemyTank(float dt);
	//敌方坦克定时发送子弹的方法
	void enemyTankShoot(float dt);
	//碰撞检测的方法
	void collisionDetection(float dt);
	//添加敌方坦克子弹的方法
	void addEnemyTankBullet(CCNode* node,void* param);
	//计算坦克的新位置
	CCPoint calculationPosition(CCPoint joyStickV);
public:
	//加血
	void eatProp_ONE();
	//变子弹
	void eatProp_TWO(int state_increment);
	//清屏
	void eatProp_THREE();


public:
	//所属管理者
    TankSceneManager *tsm;
	//坦克摇杆
	SneakyJoystick *tankJoyStick;
	//坦克摇杆底座
	SneakyJoystickSkinnedBase *tankJoyStickBase;
	//炮塔摇杆底座
	SneakyJoystickSkinnedBase *gunJoyStickBase;
	//炮塔摇杆
	SneakyJoystick *gunJoyStick;
	//游戏场景
	TankGameLayer *gameLayer;
	//获取可见区域尺寸
	CCSize visibleSize;
	//获取可见区域原点坐标
	CCPoint origin;
	//菜单按钮
	CCMenu* pMenu;
	//坦克移动的标志位
	bool tankMoveFlag;
	//炮塔旋转的标志位
	bool gunMoveFlag;
	//炮塔动画帧的标志位
	bool gunIsAnimation;
	//控制导弹发射时间的标志位
	bool enemyBazookaFlag;
	//炮塔的旋转角度
	float gunAngle;
	//坦克的旋转角度
	float tankAngle;
	//并做好相应的初始化与释放工作
	CREATE_FUNC(ControlLayer);
};

#endif
