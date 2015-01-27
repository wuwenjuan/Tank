#include "NextLevelDialogLayer.h"
#include "SimpleAudioEngine.h"
#include "ControlLayer.h"
#include "EnemyTankGun.h"
#include "DialogLayer.h"
#include "AppMacros.h"
#include "LoseLayer.h"
#include "EnemyTank.h"
#include "TankBazooka.h"
#include "Barrier.h"
#include <string.h>
#include "Props.h"
#include <math.h>
#include "JNIUtil.h"
#define CCCA(x) (x->copy()->autorelease())
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

using namespace cocos2d;

//实现ControlLayer类中的init方法，初始化布景
bool ControlLayer::init()
{
    //调用父类的初始化
    if ( !CCLayer::init() )
    {
        return false;
    }
	//开启返回键监听
	setKeypadEnabled(true);
    //获取可见区域尺寸
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//获取可见区域原点坐标
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//坦克移动标志位
	tankMoveFlag = false;
	//炮管移动的标志位
	gunMoveFlag = false;
	//炮塔动画帧的标志位
	gunIsAnimation = false;
	//控制导弹发射时间的标志位
	enemyBazookaFlag = false;
	//炮管的旋转角度
	gunAngle = 0;
	//初始化旋转角度
	tankAngle = 0;
	//初始化关闭菜单项
	initButton();
	//初始化控制坦克的摇杆
	initTankJoyStick();
	//初始化控制炮塔的摇杆
	initGunJoyStick();
	//通过遥控更新坦克与炮塔位置的回调方法
    this->schedule(schedule_selector(ControlLayer::updateTank));
    //通过遥控更新炮塔方向的回调方法
    this->schedule(schedule_selector(ControlLayer::updateGun),0.05);
    //我方坦克发射子弹的方法
    this->schedule(schedule_selector(ControlLayer::tankShoot),0.15);
    //发射导弹的方法
    this->schedule(schedule_selector(ControlLayer::addtankBazooka),1.5);
    //进行碰撞检测的方法
    this->schedule(schedule_selector(ControlLayer::collisionDetection),0.05);
    //添加敌方坦克的方法
    this->schedule(schedule_selector(ControlLayer::addEnemyTank),2);
    //更新敌方炮管方向
    this->schedule(schedule_selector(ControlLayer::updateEnemyGun),0.05);
    //发射子弹的方法
    this->schedule(schedule_selector(ControlLayer::enemyTankShoot),3.5);
    //计算导弹旋转角度的方法
     this->schedule(schedule_selector(ControlLayer::calculateBazookaAngle));
    return true;
}
/*========================================初始化方法=======================================================*/
//初始化摇杆的方法
void ControlLayer::initTankJoyStick()
{
	//创建摇杆底座对象
	tankJoyStickBase = new SneakyJoystickSkinnedBase();
	//自动释放
	tankJoyStickBase->autorelease();
	//初始化
	tankJoyStickBase->init();
	//设置底座图片
	tankJoyStickBase->setBackgroundSprite(CCSprite::create("direction.png"));
	//设置操控杆图片
	tankJoyStickBase->setThumbSprite(CCSprite::create("center.png"));
	//设置摇杆底座位置
	tankJoyStickBase->setPosition
	(
		ccp
		(
			origin.x+220/2,//X坐标
			origin.y+220/2//Y坐标
		)
	);
    //创建摇杆对象
	tankJoyStick = new SneakyJoystick();
	//自动释放
	tankJoyStick->autorelease();
	//将摇杆添加进摇杆底座
	tankJoyStickBase->setJoystick(tankJoyStick);
	//将摇杆底座添加进布景
	this->addChild(tankJoyStickBase,DASHBOARD_LEVEL_DQX);
}
//初始化摇杆的方法
void ControlLayer::initGunJoyStick()
{
	//创建摇杆底座对象
	gunJoyStickBase = new SneakyJoystickSkinnedBase();
	//自动释放
	gunJoyStickBase->autorelease();
	//初始化
	gunJoyStickBase->init();
	//设置底座图片
	gunJoyStickBase->setBackgroundSprite(CCSprite::create("direction.png"));
	//设置操控杆图片
	gunJoyStickBase->setThumbSprite(CCSprite::create("center.png"));
	//设置摇杆底座位置
	gunJoyStickBase->setPosition
	(
		ccp
		(
				origin.x+visibleSize.width-220/2,//X坐标
				origin.y+220/2					//Y坐标
		)
	);
    //创建摇杆对象
	gunJoyStick = new SneakyJoystick();
	//自动释放
	gunJoyStick->autorelease();
	//将摇杆添加进摇杆底座
	gunJoyStickBase->setJoystick(gunJoyStick);
	//将摇杆底座添加进布景
	this->addChild(gunJoyStickBase,DASHBOARD_LEVEL_DQX);
}
//初始化按钮的方法
void ControlLayer::initButton()
{
	//创建关闭菜单项
	CCMenuItemImage *pPauseItem = CCMenuItemImage::create
	(
		"pause.png",     //平时的图片
		"pause_select.png",   //选中时的图片
		this,
		menu_selector(ControlLayer::menuPauseCallback)//点击时执行的回调方法
	);
	//设置关闭菜单项的位置
	pPauseItem->setPosition
	(
		ccp
		(
			origin.x+visibleSize.width-pPauseItem->getContentSize().width/2,//X坐标
			origin.y+visibleSize.height-pPauseItem->getContentSize().height/2//Y坐标
		)
	);
	//创建菜单对象
	pMenu = CCMenu::create(pPauseItem, NULL);
	//设置菜单位置
	pMenu->setPosition(CCPointZero);
	//将菜单添加到布景中
	this->addChild(pMenu, DASHBOARD_LEVEL_DQX);
}
/*=====================================更新炮塔与坦克的方法===================================================*/
//更新敌方坦克的炮管
void ControlLayer::updateEnemyGun(float dt)
{
	//遍历坦克与炮塔的列表，其中奇数为炮塔
	for(int i = 0;i<gameLayer->enemyTankArray->count()/2;i++)
	{
		//获取炮塔精灵对象
		EnemyTankGun *enemyGun = (EnemyTankGun*)gameLayer->enemyTankArray->objectAtIndex(2*i+1);
		if(enemyGun)
		{
			//获取敌方炮塔的位置
			CCPoint enemyGunPosition = enemyGun->getPosition();
			//获取我方坦克的位置
			CCPoint tankPosition =gameLayer->tankBody->getPosition();
			//计算地方炮塔与我方坦克的连接向量
			CCPoint enemyGunVector = ccp(tankPosition.x-enemyGunPosition.x,tankPosition.y-enemyGunPosition.y);
			//计算地方炮塔的旋转角度
			float enemyGunAngle = 0;
			//计算角度
			if(enemyGunVector.x>0)
			{
				enemyGunAngle=90-atan(enemyGunVector.y/enemyGunVector.x)*180.0/3.1415926;
			}
			else
			{
				enemyGunAngle=-atan(enemyGunVector.y/enemyGunVector.x)*180.0/3.1415926-90;
			}
			//敌方炮塔执行旋转动作
			enemyGun->runAction
			(
				CCSequence::create
				(
					//所有的炮塔同时旋转，完之后才能进行下一次的旋转
					CCRotateTo::create(0.3f,enemyGunAngle),
					CCCallFuncND::create(this, callfuncND_selector(ControlLayer::setGunMoveFlag), NULL),
					NULL
				)
			);
		}
	}
}
CCPoint ControlLayer::calculationPosition(CCPoint joyStickV)
{
	//获取当前坦克位置
	CCPoint tankPosition=gameLayer->tankBody->getPosition();
	//获取当前坦克的大小
	CCSize tankSize  = gameLayer->tankBody->getContentSize();
	//求出假走一步后当前飞机的位置
	float hx=tankPosition.x+(gameLayer->SHOOT_STATE+4)/2*joyStickV.x;
	float hy=tankPosition.y+(gameLayer->SHOOT_STATE+4)/2*joyStickV.y;
	//获取飞机的新位置
	CCPoint newPosition = ccp(hx,hy);
	//如果越界该方向位置不变
	if(hx<(origin.x+tankSize.width/2.0)||hx>(origin.x+visibleSize.width - tankSize.width / 2.0))
	{
		newPosition.x = tankPosition.x;
	}
	//如果越界该方向位置不变
	if(hy<(origin.y+tankSize.height / 2.0)||hy>(origin.y+visibleSize.height - tankSize.height/2.0))
	{
		newPosition.y = tankPosition.y;
	}
	return newPosition;
}
//在这里面查看游戏摇杆状态走英雄精灵
void ControlLayer::updateTank()
{
	//获取当前坦克位置
	CCPoint tankPosition=gameLayer->tankBody->getPosition();
	//获取当前坦克的大小
	CCSize tankSize  = gameLayer->tankBody->getContentSize();
	//获取摇杆的速度矢量
	CCPoint joyStickV=tankJoyStick->getVelocity();
	//求速度大小
	if(joyStickV.x==0&&joyStickV.y==0)
	{
		if(origin.y+tankPosition.y-1>origin.y+tankSize.height/2)
		{
			gameLayer->tankBody->setPosition(ccp(tankPosition.x,tankPosition.y-1));
			gameLayer->tankGun->setPosition(ccp(tankPosition.x,tankPosition.y-1));
		}
		return;
	}
	//计算坦克的旋转角度
	if((joyStickV.x>0))
	{
		tankAngle=90-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926;
	}
	else
	{
		tankAngle=-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926-90;
	}
	//坦克执行移动动作与旋转动作
	gameLayer->tankBody->setRotation(tankAngle);
	//计算坦克的新位置
	CCPoint newPosition = this->calculationPosition(joyStickV);
	//设置坦克的新位置
	gameLayer->tankGun->setPosition(newPosition);
	//设置炮塔的新位置
	gameLayer->tankBody->setPosition(newPosition);

}
//在这里面查看游戏摇杆状态走英雄精灵
void ControlLayer::updateGun(float dt)
{
	//若一个动作没做完，不接受新的动作
	if(gunMoveFlag)
	{
		return;
	}
	//获取摇杆的速度矢量
	CCPoint joyStickV=gunJoyStick->getVelocity();
	if(joyStickV.x==0&&joyStickV.y==0)
	{
		return;
	}
	//设置动作执行中标志位
	gunMoveFlag = true;
	if(joyStickV.x>0)
	{
		gunAngle=90-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926;
	}
	else
	{
		gunAngle=-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926-90;
	}
	//炮塔执行旋转动作
	gameLayer->tankGun->runAction
	(
		CCSequence::create
		(
			CCRotateTo::create(0.15f,gunAngle),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::setGunMoveFlag), NULL),
			NULL
		)
	);
}
void ControlLayer::setGunMoveFlag(CCObject* pSender)
{
	gunMoveFlag = false;
}
/*=====================================碰撞检测的方法===================================================*/
//进行碰撞检测的方法
void ControlLayer::collisionDetection(float dt)
{
	//我方子弹与敌方坦克碰撞的方法
	this->tankBullet_enemyTank(gameLayer->tankBullet,1);
	//我方导弹与敌方坦克碰撞的方法
	this->tankBullet_enemyTank(gameLayer->tankBazooka,3);
	//我方子弹与敌方障碍物的碰撞方法
	this->tankBullet_enemy(gameLayer->tankBullet,1);
	//我方导弹与敌方障碍物的碰撞方法
	this->tankBullet_enemy(gameLayer->tankBazooka,3);
	//我方坦克与敌方坦克碰撞的方法
	this->tank_enemyTank();
	//我方坦克与障碍物碰撞的方法
	this->tank_barrier(gameLayer->barrierArray);
	//我方坦克吃到道具的方法
	this->tank_props(gameLayer->propsArray);
	//敌方子弹与我方坦克碰撞的方法
	this->enemyTankBullet_tank(gameLayer->enemyBullet,2);
	//敌方导弹与我方坦克碰撞的方法
	this->enemyTankBullet_tank(gameLayer->enemyBazooka,3);
}
//我方坦克与障碍物碰撞的方法
void ControlLayer::tank_barrier(CCArray* barrierArray)
{
	//遍历障碍物列表
	for(int i = 0;i<barrierArray->count();i++)
	{
		//获取障碍物对象
		Barrier* barrier= (Barrier*)barrierArray->objectAtIndex(i);
		//执行碰撞检测
		if(barrier->isCollision(gameLayer->tankBody))
		{
			/*========================================================================*/
			if(gameLayer->changeBlood(-5))
			{
				//失败界面
				loseView();
			}
			//发射状态减一
			eatProp_TWO(-1);
			//增加分数
			gameLayer->addScore(15);
			//获取障碍物精灵的位置
			CCPoint position = barrier->getPosition();
			//计算时间
			float time = position.y/480*20;
			//移除列表中的精灵对象
			gameLayer->barrierArray->removeObject(barrier);
			//移除布景上的精灵对象
			gameLayer->removeChild(barrier);
			//添加爆炸精灵
			addBomb(position,time,0);
			//添加炸痕精灵
			addMarks(position,time);
			break;
		}
	}
}

//我方坦克吃到道具的 方法
void ControlLayer::tank_props(CCArray* propsArray)
{
	//遍历道具列表物列表
	for(int i = propsArray->count()-1;i>=0;i--)
	{
		Props* props = (Props*)propsArray->objectAtIndex(i);
		//执行碰撞检测
		if(props->isCollision(gameLayer->tankBody))
		{
			if(JNIUtil::getInt("effectFlag",1)==1)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/eatprops.wav");
			}
			switch(props->propsIndex)
			{
				case 0:
				case 1:
					eatProp_ONE();
					break;
				case 2:
				case 3:
					eatProp_TWO(1);
					//增加分数
					gameLayer->addScore(10);
					break;
				case 4:
					eatProp_THREE();
					break;
			}
			propsArray->removeObject(props);
			gameLayer->removeChild(props);

		}
	}
}
void ControlLayer::calculateBazookaAngle()
{
	//获取导弹数组列表
	CCArray* tankBazookaArray = gameLayer->tankBazooka->getChildren();
	//如果导弹数组列表不为空
	if(tankBazookaArray)
	{
		for(int i=0;i<tankBazookaArray->count();i++)
		{
			TankBazooka* tankBazooka = (TankBazooka*)tankBazookaArray->objectAtIndex(i);
			if(tankBazooka->platformPosition())
			{
				//添加爆炸精灵的方法
				this->addBomb(tankBazooka->getPosition(),10,0);
				//删除导弹精灵
				this->removeTankBazooka(tankBazooka);
			}
		}
	}
}
//我方子弹与敌方坦克碰撞的方法
void ControlLayer::tankBullet_enemyTank(CCSpriteBatchNode* bulletNode,int bolldReduce)
{
	//获取我方子弹精灵列表
	CCArray* tankBulletArray = bulletNode->getChildren();
	//如果我方子弹列表不为空
	if(tankBulletArray)
	{
		//遍历我方子弹列表
		for(int i=0;i<tankBulletArray->count();i++)
		{
			//获取我方子弹对象
			CCSprite* tankBullet;
			if(bolldReduce>1)
			{
				tankBullet=(TankBazooka*)tankBulletArray->objectAtIndex(i);
			}
			else
			{
				tankBullet=(CCSprite*)tankBulletArray->objectAtIndex(i);
			}
			CCArray* enemyTankArray = gameLayer->enemyTankArray;
			//遍历敌方坦克列表
			for(int j = 0;j<enemyTankArray->count()/2;j++)
			{
				//获取敌方坦克对象
				EnemyTank* enemyTank= (EnemyTank*)enemyTankArray->objectAtIndex(2*j);
				//获取敌方坦克炮塔对象
				EnemyTankGun* enemyTankGun= (EnemyTankGun*)enemyTankArray->objectAtIndex(2*j+1);
				//执行碰撞检测
				if(enemyTank->isCollision(tankBullet))
				{

					//增加分数
					gameLayer->addScore(bolldReduce*5);
					/*========================================================================*/
					//获取坦克精灵的位置
					CCPoint position = enemyTank->getPosition();
					//获取子弹精灵的位置
					CCPoint bulletPosition = tankBullet->getPosition();
					//移除我方子弹精灵removeTankBullet
					bulletNode->removeChild(tankBullet,true);
					//计算时间
					float time = position.y/480*20;
					//获取血量
					enemyTank->enemyTankBlood-=bolldReduce;

					if(enemyTank->enemyTankBlood<=0)
					{
						if(JNIUtil::getInt("effectFlag",1)==1)
						{
							CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
							(
									"sound/grenada.ogg"
							);
						}
						//移除敌方坦克炮塔
						gameLayer->enemyTankArray->removeObject(enemyTankGun);
						//移除敌方坦克炮塔
						gameLayer->removeChild(enemyTankGun);
						//移除列表中的精灵对象
						gameLayer->enemyTankArray->removeObject(enemyTank);
						//移除敌方坦克
						gameLayer->removeChild(enemyTank);
						//添加爆炸精灵
						addBomb(position,time,0);
						//添加道具
						addProps(position);
						//添加炸痕精灵
						addMarks(position,time);
					}
					else
					{
						//添加爆炸精灵
						addBomb(bulletPosition,time,1);
					}
					break;
				}
			}
		}
	}
}
//我方子弹与敌方障碍物的碰撞方法
void ControlLayer::tankBullet_enemy(CCSpriteBatchNode* bulletNode,int bolldReduce)
{
	//获取我方子弹精灵列表
	CCArray* tankBulletArray = bulletNode->getChildren();
	//如果我方子弹列表不为空
	if(tankBulletArray)
	{
		//遍历我方子弹列表
		for(int i = 0;i<tankBulletArray->count();i++)
		{
			//获取我方子弹对象
			CCSprite* tankBullet;
			if(bolldReduce>1)
			{
				tankBullet=(TankBazooka*)tankBulletArray->objectAtIndex(i);
			}
			else
			{
				tankBullet=(CCSprite*)tankBulletArray->objectAtIndex(i);
			}
			//获取障碍物列表
			CCArray* barrierArray = gameLayer->barrierArray;
			//遍历障碍物列表
			for(int j = 0;j<barrierArray->count();j++)
			{
				//获取障碍物对象
				Barrier* barrier= (Barrier*)barrierArray->objectAtIndex(j);
				//执行碰撞检测
				if(barrier->isCollision(tankBullet))
				{
					//增加分数
					gameLayer->addScore(bolldReduce*5);
					/*========================================================================*/
					//获取障碍物精灵的位置
					CCPoint position = barrier->getPosition();
					//获取子弹的位置
					CCPoint bulletPosition = tankBullet->getPosition();
					//移除子弹
					bulletNode->removeChild(tankBullet,true);
					//计算时间
					float time = position.y/480*20;
					//血量减少
					barrier->barrierBlood -=bolldReduce;
					if(barrier->barrierBlood<=0)
					{
						if(JNIUtil::getInt("effectFlag",1)==1)
						{
							CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
							(
									"sound/grenada.ogg"
							);
						}
						//移除列表中的精灵对象
						gameLayer->barrierArray->removeObject(barrier);
						//移除布景上的精灵对象
						gameLayer->removeChild(barrier);
						//添加爆炸精灵
						addBomb(position,time,0);
						//添加道具
						addProps(position);
						//添加炸痕精灵
						addMarks(position,time);
					}
					else
					{
						//添加爆炸精灵
						addBomb(bulletPosition,time,1);
					}
					break;
				}
			}
		}
	}
}
//我方坦克与敌方坦克碰撞的方法
void ControlLayer::tank_enemyTank()
{
	//获取敌方到坦克列表
	CCArray* enemyTankArray = gameLayer->enemyTankArray;
	//变量敌方坦克列表
	for(int j = 0;j<enemyTankArray->count()/2;j++)
	{
		//获取敌方坦克对象
		EnemyTank* enemyTank= (EnemyTank*)enemyTankArray->objectAtIndex(2*j);
		//获取敌方坦克炮塔对象
		EnemyTankGun* enemyTankGun= (EnemyTankGun*)enemyTankArray->objectAtIndex(2*j+1);
		//执行碰撞检测
		if(enemyTank->isCollision(gameLayer->tankBody))
		{
			if(JNIUtil::getInt("effectFlag",1)==1)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
				(
						"sound/grenada.ogg"
				);
			}
			/*========================================================================*/
			if(gameLayer->changeBlood(-5))
			{
				//失败界面
				loseView();
			}
			//发射状态减一
			eatProp_TWO(-1);
			//增加分数
			gameLayer->addScore(15);
			//获取坦克精灵的位置
			CCPoint position = enemyTank->getPosition();
			//移除敌方坦克炮塔
			gameLayer->enemyTankArray->removeObject(enemyTankGun);
			//移除敌方坦克炮塔
			gameLayer->removeChild(enemyTankGun);
			//移除列表中的精灵对象
			gameLayer->enemyTankArray->removeObject(enemyTank);
			//移除敌方坦克
			gameLayer->removeChild(enemyTank);
			//计算时间
			float time = position.y/480*20;
			//添加爆炸精灵
			addBomb(position,time,0);
			//添加炸痕精灵
			addMarks(position,time);
		}
	}
}
//敌方子弹与我方坦克碰撞的方法
void ControlLayer::enemyTankBullet_tank(CCSpriteBatchNode* enemyBulletNode,int bloodReduce)
{
	//获取敌方子弹列表
	CCArray* enemyBulletArray = enemyBulletNode->getChildren();
	if(enemyBulletArray)
	{
		//遍历敌方子弹列表
		for(int i = 0;i<enemyBulletArray->count();i++)
		{
			//获取敌方坦克子弹精灵对象
			CCSprite* enemyBullet = (CCSprite*)enemyBulletArray->objectAtIndex(i);
			//获取坦克子弹精灵的大小
			CCSize enemyBulletSize = enemyBullet->getContentSize();
			//获取我方坦克精灵的大小
			CCSize tankSize = gameLayer->tankBody->getContentSize();
			//碰撞的范围的计算
			float width = enemyBulletSize.width+tankSize.width;
			float height = enemyBulletSize.height+tankSize.height;
			//获取敌方坦克子弹精灵位置
			CCPoint enemyBulletPositon = enemyBullet->getPosition();
			//获取我方坦克位置
			CCPoint tankPosition = gameLayer->tankBody->getPosition();
			//执行判断
			if(abs(enemyBulletPositon.x-tankPosition.x)<=width/2&&abs(enemyBulletPositon.y-tankPosition.y)<=height/2)
			{
				/*====================================================================================*/
				if(gameLayer->changeBlood(-bloodReduce))
				{
					//失败界面
					loseView();
				}
				//发射状态减一
				eatProp_TWO(-1);
				//获取需要移除的敌方坦克子弹的位置
				CCPoint enemyBulletPosition = enemyBullet->getPosition();
				//移除该字该子弹
				enemyBulletNode->removeChild(enemyBullet,true);
				//创建需要播放爆炸特效的精灵
				CCSprite *bomb = CCSprite::create("bomb.png");
				//设置精灵的位置
				bomb->setPosition(ccp(enemyBulletPosition.x,enemyBulletPosition.y));
				//将精灵添加到布景
				gameLayer->addChild(bomb,BOMB_LEVEL_DQX);
				//执行动作
				bomb->runAction
				(
					CCSequence::create
					(
						CCAnimate::create((CCAnimation*)CCCA(gameLayer->tankBulletAnmi[1])),
						CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeBomb), NULL),
						NULL
					)
				);
			}
		}
	}
}
//移除爆炸精灵的方法
void ControlLayer::removeBomb(CCNode* node,void* param)
{
	//获取爆炸精灵
	CCSprite* bomb = (CCSprite*)node;
	//移除爆炸精灵
	gameLayer->removeChild(bomb);
}
//敌方坦克发射子弹的方法
void ControlLayer::enemyTankShoot(float dt)
{

	for(int i = 0;i<gameLayer->enemyTankArray->count()/2;i++)
	{
		//获取敌方坦克精灵对象
		EnemyTankGun* enemyTankGun = (EnemyTankGun*)gameLayer->enemyTankArray->objectAtIndex(2*i+1);
		//如果敌方炮塔没有进行帧动画
		if(!enemyTankGun->enemyIsAnimation)
		{
			//随机产生一个微小的动作延迟时间
			int index = rand()%3;
			//创建wait动作
			CCDelayTime* waitAction = CCDelayTime::create(index*0.005);
			//将帧动画的标志位置为true
			enemyTankGun->enemyIsAnimation = true;
			//炮管首先播放一下动画然后执行添加炮弹操作
			enemyTankGun->runAction
			(
				CCSequence::create
				(
					waitAction,
					CCAnimate::create((CCAnimation*)CCCA(gameLayer->tankGunAnmi[enemyTankGun->index])),
					CCCallFuncND::create(this, callfuncND_selector(ControlLayer::addEnemyTankBullet), NULL),
					NULL
				)
			);
		}
	}
	for(int i = 0;i<gameLayer->barrierArray->count();i++)
	{
		//获取敌方坦克精灵对象
		Barrier* barier = (Barrier*)gameLayer->barrierArray->objectAtIndex(i);
		//如果敌方炮塔没有进行帧动画
		if(barier->barrierIndex==5)
		{
			//创建获取障碍物的位置
			CCPoint barierPosition = barier->getPosition();
			//获取我方坦克的位置
			CCPoint tankPosition = gameLayer->tankBody->getPosition();
			//得到障碍物与坦克的连线向量
			CCPoint vector2f = ccp(tankPosition.x-barierPosition.x,tankPosition.y-barierPosition.y);
			//规格化向量
			vector2f = ccpNormalize(vector2f);
			//创建精灵对象
			CCSprite* barierBullet = CCSprite::create((gameLayer->enemyName[barier->barrierIndex-1]).c_str());
			//将子弹添加到CCSpriteBatchNode中
			gameLayer->enemyBazooka->addChild(barierBullet);
			//设置精灵的位置
			barierBullet->setPosition(ccp(barierPosition.x,barierPosition.y));
			//计算炮弹的旋转角度
			float enemyAngle = 0;
			//计算角度
			if(vector2f.x>0)
			{
				enemyAngle=90-atan(vector2f.y/vector2f.x)*180.0/3.1415926;
			}
			else
			{
				enemyAngle=-atan(vector2f.y/vector2f.x)*180.0/3.1415926-90;
			}
			//创建移动动作
			CCMoveTo* bulletMoveAction = CCMoveTo::create(5,ccp(barierPosition.x+800*vector2f.x,barierPosition.y+800*vector2f.y));
			//设置炮弹的旋转角度
			barierBullet->setRotation(enemyAngle);
			//执行动作
			barierBullet->runAction
			(
				CCSequence::create
				(
					bulletMoveAction,
					CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankBazooka), NULL),
					NULL
				)
			);
		}
	}
}
//为敌方坦克发射子弹的方法
void ControlLayer::addEnemyTankBullet(CCNode* node,void* param)
{
	//获取该敌方坦克炮塔
	EnemyTankGun* enemyTankGun = (EnemyTankGun*)node;
	//创建精灵对象
	CCSprite* enemyTankBullet = CCSprite::create((gameLayer->enemyName[enemyTankGun->index-1]).c_str());
	//执行判断
	switch(enemyTankGun->index)
	{
		case 1:
		case 2:
			//将子弹添加到CCSpriteBatchNode中
			gameLayer->enemyBullet->addChild(enemyTankBullet);
			break;
		case 3:
		case 4:
			//将子弹添加到CCSpriteBatchNode中
			gameLayer->enemyBazooka->addChild(enemyTankBullet);
			break;
	}
	//重置特效播放的标志位
	enemyTankGun->enemyIsAnimation = false;
	//设置敌方坦克子弹旋转角度
	enemyTankBullet->setRotation(enemyTankGun->getRotation());
	//获取炮塔的旋转角度
	float angleTemp=enemyTankGun->getRotation();
	//转换为弧度角
	angleTemp=angleTemp*3.1415926f/180.0f;
	//获取敌方坦克炮塔的位置
	CCPoint enemyGunPosition = enemyTankGun->getPosition();
	//设置子弹精灵位置
	enemyTankBullet->setPosition(ccp(enemyGunPosition.x+40*sin(angleTemp),enemyGunPosition.y+40*cos(angleTemp)));
	//创建子弹的动作
	CCMoveTo* tankBulletMoveAction  = CCMoveTo::create(8,ccp(enemyGunPosition.x+800*sin(angleTemp),enemyGunPosition.y+800*cos(angleTemp)));
	//执行动作
	if(enemyTankGun->index<3)
	{
		enemyTankBullet->runAction
		(
			CCSequence::create
			(
				tankBulletMoveAction,
				CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankBullet), NULL),
				NULL
			)
		);
	}
	else
	{
		enemyTankBullet->runAction
			(
				CCSequence::create
				(
					tankBulletMoveAction,
					CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankBazooka), NULL),
					NULL
				)
			);
	}
}
void ControlLayer::removeEnemyTankBullet(CCNode* node,void* param)
{
	//获取子弹精灵
	CCSprite* uselessSprite = (CCSprite*)node;
	//移除子弹精灵
	gameLayer->enemyBullet->removeChild(uselessSprite,true);
}
void ControlLayer::removeEnemyTankBazooka(CCNode* node,void* param)
{
	//获取炸弹精灵
	CCSprite* uselessSprite = (CCSprite*)node;
	//移除炸炸弹精灵
	gameLayer->enemyBazooka->removeChild(uselessSprite,true);
}
//发射炮弹的方法
void ControlLayer::tankShoot(float dt)
{
	//如果炮塔没有进行帧动画
	if(!gunIsAnimation)
	{
		//动作延迟时间
		CCDelayTime* waitAction = CCDelayTime::create(0.1*(3-gameLayer->SHOOT_STATE));
		//将帧动画的标志位置为true
		gunIsAnimation = true;
		//炮管首先播放一下动画然后执行添加炮弹操作
		gameLayer->tankGun->runAction
		(
			CCSequence::create
			(
				waitAction,
				CCAnimate::create((CCAnimation*)CCCA(gameLayer->tankGunAnmi[0])),
				CCCallFuncND::create(this, callfuncND_selector(ControlLayer::addTankBullet), NULL),
				NULL
			)
		);
	}
}
void ControlLayer::addTankBullet()
{
	//帧动画进行完毕重置标志位为false
	gunIsAnimation = false;
	//创建子弹精灵
	CCSprite* tankBullet = CCSprite::create("tankBullet.png");
	//将子弹添加到CCSpriteBatchNode中
	gameLayer->tankBullet->addChild(tankBullet);
	//设置坦克子弹旋转角度
	tankBullet->setRotation(gameLayer->tankGun->getRotation());
	//获取炮塔的旋转角度
	float angleTemp=gameLayer->tankGun->getRotation();
	//转换为弧度角
	angleTemp=angleTemp*3.1415926f/180.0f;
	//获取炮塔的位置
	CCPoint gunPosition = gameLayer->tankGun->getPosition();
	//设置子弹精灵位置
	tankBullet->setPosition(ccp(gunPosition.x+40*sin(angleTemp),gunPosition.y+40*cos(angleTemp)));
	//创建子弹的动作
	CCMoveTo* tankBulletMoveAction  = CCMoveTo::create(4,ccp(gunPosition.x+800*sin(angleTemp),gunPosition.y+800*cos(angleTemp)));
	//执行动作
	tankBullet->runAction
	(
		CCSequence::create
		(
			tankBulletMoveAction,
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeTankBullet), NULL),
			NULL
		)
	);
}
//添加子弹
void ControlLayer::addtankBazooka(float dt)
{
	if(gameLayer->SHOOT_STATE==3)
	{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
			(
				"sound/rocket_shoot2.ogg"
			);
		}
		//产生随机数
		int index = rand()%2;
		//获取追踪导弹的位置
		CCPoint enemyPosition = this->getRandEnemyPosition();
		//获取炮塔的旋转角度
		float angleTemp=gameLayer->tankBody->getRotation();
		//转换为弧度角
		angleTemp=angleTemp*3.1415926f/180.0f;
		//获取炮塔的位置
		CCPoint bodyPosition = gameLayer->tankBody->getPosition();
		//创建炮弹精灵
		TankBazooka* tankBazooka = TankBazooka::creates(enemyPosition,ccp(3*sin(angleTemp),3*cos(angleTemp)));
		//将子弹添加到CCSpriteBatchNode中
		gameLayer->tankBazooka->addChild(tankBazooka);
		//设置坦克子弹旋转角度
		tankBazooka->setRotation(gameLayer->tankBody->getRotation());
		//设置子弹精灵位置
		tankBazooka->setPosition(ccp(bodyPosition.x+40*sin(angleTemp),bodyPosition.y+40*cos(angleTemp)));
	}
}
void ControlLayer::removeTankBullet(CCNode* node,void* param)
{
	//获取精灵对象
	CCSprite* uselessSprite =(CCSprite*)node;
	//从spriteBatchNode中移除精灵
	gameLayer->tankBullet->removeChild(uselessSprite,true);
}
void ControlLayer::removeTankBazooka(CCNode* node)
{
	//获取精灵对象
	TankBazooka* uselessSprite =(TankBazooka*)node;
	//从spriteBatchNode中移除精灵
	gameLayer->tankBazooka->removeChild(uselessSprite,true);
}
/*=====================================移除敌方坦克的方法===================================================*/
//成功过关之后的工作
void ControlLayer::reparation_for_next_level()
{
	//暂停控制层的回调方法
	this->pauseSchedulerAndActions();
	//暂停游戏界面的回调方法
	gameLayer->pauseSchedulerAndActions();
	//控制层的按钮设置为不可用
	pMenu->setEnabled(false);
	//取消控制层返回键监听
	setKeypadEnabled(false);
	//清空子弹列表
	gameLayer->tankBullet->removeAllChildrenWithCleanup(true);
	//清空炮弹列表
	gameLayer->tankBazooka->removeAllChildrenWithCleanup(true);
	//清空炮弹列表
	gameLayer->enemyBullet->removeAllChildrenWithCleanup(true);
	//清空炮弹列表
	gameLayer->enemyBazooka->removeAllChildrenWithCleanup(true);
	//清空灌木列表
	gameLayer->trees->removeAllChildren();
	//清空炸痕列表
	gameLayer->marks->removeAllChildren();
	//当前关卡加1
	gameLayer->passTemp++;
	//重新计算总关数
	gameLayer->enemyGroupCount = 3*pow(1.2,gameLayer->passTemp);
	//重置当前敌人组数
	gameLayer->currentEnemyGroup = 0;
}
//下一关的方法
void ControlLayer::nextLevel()
{
	//恢复控制层的回调方法
	this->resumeSchedulerAndActions();
	//恢复游戏界面的回调方法
	gameLayer->resumeSchedulerAndActions();
	//控制层的按钮设置为可用
	pMenu->setEnabled(true);
	//取消控制层返回键监听
	setKeypadEnabled(true);
}
//添加敌方坦克的方法
void ControlLayer::addEnemyTank(float dt)
{
	if(++(gameLayer->currentEnemyGroup)>(gameLayer->enemyGroupCount))
	{
		//获取敌方坦克数组大学
		int eCount = gameLayer->enemyTankArray->count();
		//获取障碍物数组大小
		int bCount = gameLayer->barrierArray->count();
		//获取道具精灵数组大学
		int pCount = gameLayer->propsArray->count();
		//执行判断
		if(eCount==0 && bCount==0&&pCount==0)
		{

			//成功过关之后的工作准备工作
			this->reparation_for_next_level();
			//创建半透明层
			NextLevelDialogLayer *nextLevelDialogLayer = NextLevelDialogLayer::create();
			//设置半透明层的位置
			nextLevelDialogLayer->setPosition(ccp(0,0));
			//添加到父类层
			this->addChild(nextLevelDialogLayer,DASHBOARD_LEVEL_DQX);
		}
		return;
	}
	//产生1-10的随机数
	int index = rand()%10+1;
	switch(index)
	{
		case ENEMY_TANK_GROUP_ONE:
			this->addEnemyTankGroup_ONE();
			break;
		case ENEMY_TANK_GROUP_TWO:
			this->addEnemyTankGroup_TWO();
			break;
		case ENEMY_TANK_GROUP_THREE:
			this->addEnemyTankGroup_THREE();
			break;
		case ENEMY_TANK_GROUP_FORE:
			this->addEnemyTankGroup_FORE();
			break;
		case ENEMY_TANK_GROUP_FIVE:
			this->addEnemyTankGroup_FIVE();
			break;
		case ENEMY_TANK_GROUP_SIX:
			this->addEnemyTankGroup_SIX();
			break;
		case ENEMY_TANK_GROUP_SEVEN:
			this->addEnemyTankGroup_SEVEN();
			break;
		case ENEMY_TANK_GROUP_EIGHT:
			this->addEnemyTankGroup_EIGHT();
			break;
		case ENEMY_TANK_GROUP_NINE:
			this->addEnemyTankGroup_NINE();
			break;
		case ENEMY_TANK_GROUP_TEN:
			this->addEnemyTankGroup_TEN();
			break;
	}
}
//添加敌方坦克的方法
void ControlLayer::addEnemyTankGroup_ONE()
{
	//添加一个地方坦克或障碍的方法的方法
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8,origin.y+visibleSize.height));
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*7,origin.x+visibleSize.height));
}
void ControlLayer::addEnemyTankGroup_TWO()
{
	//添加一个地方坦克或障碍的方法的方法
	addEnemyTank_TWO(ccp(origin.x+visibleSize.width/8,origin.y+visibleSize.height));
	addEnemyTank_TWO(ccp(origin.x+visibleSize.width/8*2,origin.x+visibleSize.height));

	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*6,origin.y+visibleSize.height));
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*7,origin.x+visibleSize.height));

}
void ControlLayer::addEnemyTankGroup_THREE()
{
	addBarrier(ccp(origin.x+visibleSize.width/8*3,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*4,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*5,origin.y+visibleSize.height));
	addTower(ccp(origin.x+visibleSize.width/8*4,origin.y+visibleSize.height+100));

}
void ControlLayer::addEnemyTankGroup_FORE()
{

	addBarrier(ccp(origin.x+visibleSize.width/8*1,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*2,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*3,origin.y+visibleSize.height));
	addTower(ccp(origin.x+visibleSize.width/16*3,origin.y+visibleSize.height+100));
	addEnemyTank_TWO(ccp(origin.x+visibleSize.width/16*5,origin.y+visibleSize.height+100));




	addBarrier(ccp(origin.x+visibleSize.width/8*5,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*6,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*7,origin.y+visibleSize.height));
	addTower(ccp(origin.x+visibleSize.width/16*11,origin.y+visibleSize.height+100));
	addEnemyTank_TWO(ccp(origin.x+visibleSize.width/16*13,origin.y+visibleSize.height+100));

}
void ControlLayer::addEnemyTankGroup_FIVE()
{
	addBarrier(ccp(origin.x+visibleSize.width/8*3,origin.y+visibleSize.height));
	addBarrier(ccp(origin.x+visibleSize.width/8*5,origin.y+visibleSize.height));
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*4,origin.y+visibleSize.height));
	addTower(ccp(origin.x+visibleSize.width/16*7,origin.y+visibleSize.height+100));
	addTower(ccp(origin.x+visibleSize.width/16*9,origin.y+visibleSize.height+100));

}
void ControlLayer::addEnemyTankGroup_SIX()
{
	addEnemyTank_THREE(ccp(origin.x+visibleSize.width/8*1,origin.y+visibleSize.height));
	addEnemyTank_THREE(ccp(origin.x+visibleSize.width/8*7,origin.y+visibleSize.height));

	addEnemyTank_FORE(ccp(origin.x+visibleSize.width/8*2,origin.y+visibleSize.height));
	addEnemyTank_FORE(ccp(origin.x+visibleSize.width/8*6,origin.y+visibleSize.height));

}
void ControlLayer::addEnemyTankGroup_SEVEN()
{
	for(int i = 0;i<9;i++)
	{
		addBarrier(ccp(origin.x+visibleSize.width/8*i,origin.y+visibleSize.height));
	}
}
void ControlLayer::addEnemyTankGroup_EIGHT()
{
	for(int i = 0;i<4;i++)
	{
		addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*(2*i+1),origin.y+visibleSize.height));
	}
	for(int i = 1;i<5;i++)
	{
		addEnemyTank_TWO(ccp(origin.x+visibleSize.width/8*(2*i),origin.y+visibleSize.height));
	}
}
void ControlLayer::addEnemyTankGroup_NINE()
{
	addTower(ccp(origin.x+visibleSize.width/16*7,origin.y+visibleSize.height));
	addTower(ccp(origin.x+visibleSize.width/16*9,origin.y+visibleSize.height));
	addTower(ccp(origin.x+visibleSize.width/16*8,origin.y+visibleSize.height+100));

}
void ControlLayer::addEnemyTankGroup_TEN()
{
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*1,origin.y+visibleSize.height));
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*7,origin.y+visibleSize.height));

	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*2,origin.y+visibleSize.height));
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*6,origin.y+visibleSize.height));
}
//添加一个地方坦克或障碍的方法的方法
void ControlLayer::addEnemyTank_ONE(CCPoint position)
{
	//此处写添加敌方坦克发射子弹的方法
	EnemyTank *enemyTank = EnemyTank::creates("enemybody1.png",1,1);
	//设置敌方坦克的位置
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//计算时间
	float time = (position.y+60)/480*20;
	//敌方坦克执行动作
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//创建敌方炮塔精灵
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun1.png",1);
	//设置精灵位置
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//设置精灵锚点
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//设置坦克棍的旋转角度
	enemyTankGun->setRotation(180);
	//精灵执行动作
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//将精灵添加到布景
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//将精灵添加到列表
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addEnemyTank_TWO(CCPoint position)
{
	//此处写添加敌方坦克发射子弹的方法
	EnemyTank *enemyTank = EnemyTank::creates("enemybody2.png",2,1);
	//设置敌方坦克的位置
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//计算时间
	float time = (position.y+60)/480*20;
	//敌方坦克执行动作
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//创建敌方炮塔精灵
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun2.png",2);
	//设置精灵位置
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//设置精灵锚点
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//设置坦克棍的旋转角度
	enemyTankGun->setRotation(180);
	//精灵执行动作
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//将精灵添加到布景
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//将精灵添加到列表
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addEnemyTank_THREE(CCPoint position)
{
	//此处写添加敌方坦克发射子弹的方法
	EnemyTank *enemyTank = EnemyTank::creates("enemybody3.png",3,2);
	//设置敌方坦克的位置
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//计算时间
	float time = (position.y+60)/480*20;
	//敌方坦克执行动作
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//创建敌方炮塔精灵
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun3.png",2);
	//设置精灵位置
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//设置精灵锚点
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//设置坦克棍的旋转角度
	enemyTankGun->setRotation(180);
	//精灵执行动作
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//将精灵添加到布景
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//将精灵添加到列表
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addEnemyTank_FORE(CCPoint position)
{
	//此处写添加敌方坦克发射子弹的方法
	EnemyTank *enemyTank = EnemyTank::creates("enemybody4.png",4,2);
	//设置敌方坦克的位置
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//计算时间
	float time = (position.y+60)/480*20;
	//敌方坦克执行动作
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//创建敌方炮塔精灵
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun1.png",4);
	//设置精灵位置
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//设置精灵锚点
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//设置坦克棍的旋转角度
	enemyTankGun->setRotation(180);
	//精灵执行动作
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//将精灵添加到布景
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//将精灵添加到列表
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addBarrier(CCPoint position)
{
	//此处写添加障碍物的方法
	Barrier *barrier = Barrier::creates("barrier.png",6,2);
	//设置敌障碍物的位置
	barrier->setPosition(ccp(position.x,position.y+60));
	//计算时间
	float time = (position.y+60)/480*20;
	//执行动作
	barrier->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeBarrier), NULL),
			NULL
		)
	);
	//像障碍物列表中添加障碍物对象
	gameLayer->barrierArray->addObject(barrier);
	//将障碍物添加到布景层
	gameLayer->addChild(barrier,TANK_LEVEL_DQX);
}
void ControlLayer::addTower(CCPoint position)
{
	//此处写添加障碍物的方法
	Barrier *barrier = Barrier::creates("tower.png",5,2);
	//设置障碍物的位置
	barrier->setPosition(ccp(position.x,position.y+60));
	//计算时间
	float time = (position.y+60)/480*20;
	//障碍物执行动作
	barrier->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeBarrier), NULL),
			NULL
		)
	);
	gameLayer->barrierArray->addObject(barrier);
	gameLayer->addChild(barrier,TANK_LEVEL_DQX);
}
//移除敌方坦克的回调方法
void ControlLayer::removeEnemyTank(CCNode* node,void* param)
{
	//获取需要移除的坦克
	EnemyTank* uselessSprite = (EnemyTank*)node;
	//移除敌方坦克
	gameLayer->removeChild(uselessSprite);
	//从列表中移除敌方坦克
	gameLayer->enemyTankArray->removeObject(uselessSprite);
}
//移除地方坦克炮管的回调方法
void ControlLayer::removeEnemyTankGun(CCNode* node,void* param)
{
	//获取需要移除的坦克炮塔
	EnemyTankGun* uselessSprite = (EnemyTankGun*)node;
	//移除敌方坦克炮塔
	gameLayer->removeChild(uselessSprite);
	//从列表中移除敌方坦克炮塔
	gameLayer->enemyTankArray->removeObject(uselessSprite);
}
void ControlLayer::removeBarrier(CCNode* node,void* param)
{
	//获取需要移除的精灵
	Barrier* uselessSprite = (Barrier*)node;
	//从列表中移除精灵
	gameLayer->barrierArray->removeObject(uselessSprite);
	//从布景中移除精灵
	gameLayer->removeChild(uselessSprite);
}
//加血
void ControlLayer::eatProp_ONE()
{

	gameLayer->changeBlood(5);
}
//变子弹
void ControlLayer::eatProp_TWO(int state_increment)
{
	//计算发射子弹的状态
	gameLayer->SHOOT_STATE = ((gameLayer->SHOOT_STATE+state_increment)>3)?3:(gameLayer->SHOOT_STATE+state_increment);
	//发射子弹的状态最小为0
	if(gameLayer->SHOOT_STATE<=0)
	{
		gameLayer->SHOOT_STATE = 0;
	}
}
//清屏
void ControlLayer::eatProp_THREE()
{
	//清空子弹列表
	gameLayer->tankBullet->removeAllChildrenWithCleanup(true);
	//清空炮弹列表
	gameLayer->tankBazooka->removeAllChildrenWithCleanup(true);
	//清空炮弹列表
	gameLayer->enemyBullet->removeAllChildrenWithCleanup(true);
	//清空炮弹列表
	gameLayer->enemyBazooka->removeAllChildrenWithCleanup(true);
	//遍历障碍物列表
	for(int i = 0;i<gameLayer->barrierArray->count();i++)
	{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
			(
					"sound/grenada.ogg"
			);
		}
		//获取障碍物对象
		Barrier* barrier= (Barrier*)(gameLayer->barrierArray->objectAtIndex(i));
		//增加分数
		gameLayer->addScore(15);
		//获取障碍物精灵的位置
		CCPoint position = barrier->getPosition();
		//计算时间
		float time = position.y/480*20;
		//添加爆炸精灵
		addBomb(position,time,0);
		//添加道具
		addProps(position);
		//添加炸痕精灵
		addMarks(position,time);
	}
	for(int i = 0;i<gameLayer->barrierArray->count();i++)
	{
		//移除障碍物
		gameLayer->removeChild((Barrier*)gameLayer->barrierArray->objectAtIndex(i));
	}
	//清空障碍物列表
	gameLayer->barrierArray->removeAllObjects();
	//变量敌方坦克列表
	for(int j = 0;j<gameLayer->enemyTankArray->count()/2;j++)
	{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
			(
					"sound/grenada.ogg"
			);
		}
		//获取敌方坦克对象
		EnemyTank* enemyTank= (EnemyTank*)(gameLayer->enemyTankArray->objectAtIndex(2*j));
		//获取敌方坦克炮塔对象
		EnemyTankGun* enemyTankGun= (EnemyTankGun*)(gameLayer->enemyTankArray->objectAtIndex(2*j+1));
		//增加分数
		gameLayer->addScore(15);
		//获取坦克精灵的位置
		CCPoint position = enemyTank->getPosition();
		//计算时间
		float time = position.y/480*20;
		//添加爆炸精灵
		addBomb(position,time,0);
		//添加道具精灵
		addProps(position);
		//添加炸痕精灵
		addMarks(position,time);
	}
	for(int i = 0;i<gameLayer->enemyTankArray->count()/2;i++)
	{
		//获取敌方坦克对象
		EnemyTank* enemyTank= (EnemyTank*)gameLayer->enemyTankArray->objectAtIndex(2*i);
		//获取敌方坦克炮塔对象
		EnemyTankGun* enemyTankGun= (EnemyTankGun*)gameLayer->enemyTankArray->objectAtIndex(2*i+1);
		//移除敌方坦克
		gameLayer->removeChild(enemyTank);
		//移除敌方坦克炮管
		gameLayer->removeChild(enemyTankGun);
	}
	//清空敌方坦克列表
	gameLayer->enemyTankArray->removeAllObjects();
}
//添加炸痕精灵的方法
void ControlLayer::addMarks(CCPoint position,float time)
{
	//创建弹痕精灵
	CCSprite* mark = CCSprite::create("mark1.png");
	//设置弹痕精灵的位置
	mark->setPosition(ccp(position.x,position.y));
	//设置弹痕精灵的位置
	mark->runAction(CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)));
	//将弹痕精灵添加到根节点
	gameLayer->marks->addChild(mark);
}
//添加爆炸精灵的方法
void ControlLayer::addBomb(CCPoint position,float time,int animIndex)
{
	//创建需要播放爆炸特效的精灵
	CCSprite *bomb = CCSprite::create("bomb.png");
	//设置精灵的位置
	bomb->setPosition(ccp(position.x,position.y));
	//将精灵添加到布景
	gameLayer->addChild(bomb,BOMB_LEVEL_DQX);
	//执行动作
	bomb->runAction
	(
		CCSequence::create
		(
			CCSpawn::createWithTwoActions
			(
				CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
				CCAnimate::create((CCAnimation*)CCCA(gameLayer->tankBulletAnmi[animIndex]))
			),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeBomb), NULL),
			NULL
		)
	);
}
//移除爆炸精灵的方法
void ControlLayer::removeProps(CCNode* node,void* param)
{
	//获取道具精灵
	Props* props = (Props*)node;
	//从列表中移除精灵
	gameLayer->propsArray->removeObject(props);
	//从布景中移除精灵
	gameLayer->removeChild(props);
}
//添加道具精灵
void ControlLayer::addProps(CCPoint position)
{
	//产生随机索引
	int index = rand()%20;
	//进行判读
	if(index>=15)
	{
		//创建道具精灵
		Props* props = Props::creates(gameLayer->pPName[index-15],index-15);
		//设置道具精灵的位置
		props->setPosition(ccp(position.x,position.y));
		//将道具精灵添加进数组
		gameLayer->propsArray->addObject(props);
		//将道具精灵添加进层
		gameLayer->addChild(props,PROPS_LEVEL_DQX);
		//执行动作
		props->runAction
		(
			CCSequence::create
			(
				CCMoveTo::create(6,ccp(position.x,ENEMYMINPOSITION)),
				CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeProps), NULL),
				NULL
			)
		);
	}
}
//实现ControlLayer类中的menuPauseCallback方法，处理关闭菜单项点击事件
void ControlLayer::menuPauseCallback(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//关闭返回键监听
	setKeypadEnabled(false);
	//设置菜单按钮的可用性
	pMenu->setEnabled(false);
	//创建半透明层
	DialogLayer *dialogLayer = DialogLayer::create();
	//设置半透明层的位置
	dialogLayer->setPosition(ccp(0,0));
	//添加到父类层
	this->addChild(dialogLayer,DASHBOARD_LEVEL_DQX);
}
CCPoint ControlLayer::getRandEnemyPosition()
{
	//获取敌方坦克列表
	int tCount = gameLayer->enemyTankArray->count();
	if(tCount!=0)
	{
		//产生随机数
		int index = rand()%tCount;
		EnemyTank* tankEnemy;
		//如果是偶数
		if(index%2==0)
		{
			tankEnemy= (EnemyTank*) gameLayer->enemyTankArray->objectAtIndex(index);
		}
		else
		{
			 tankEnemy = (EnemyTank*) gameLayer->enemyTankArray->objectAtIndex(index-1);
		}
		return tankEnemy->getPosition();
	}
	int bCount = gameLayer->barrierArray->count();
	if(bCount!=0)
	{
		int index = rand()%bCount;
		Barrier* barrier = (Barrier*) gameLayer->barrierArray->objectAtIndex(index);
		return barrier->getPosition();
	}
	return ccp(400,300);

}
void ControlLayer::loseView()
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
		(
				"sound/lose.mp3"
		);
	}

	//暂停控制层的回调方法
	this->pauseSchedulerAndActions();
	//暂停游戏界面的回调方法
	gameLayer->pauseSchedulerAndActions();
	//关闭返回键监听
	setKeypadEnabled(false);
	//设置菜单按钮的可用性
	pMenu->setEnabled(false);
	//创建半透明层
	LoseLayer *loseLayer = LoseLayer::create();
	//设置半透明层的位置
	loseLayer->setPosition(ccp(0,0));
	//添加到父类层
	this->addChild(loseLayer,DASHBOARD_LEVEL_DQX);
}

