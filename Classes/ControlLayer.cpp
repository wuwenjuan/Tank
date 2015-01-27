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

//ʵ��ControlLayer���е�init��������ʼ������
bool ControlLayer::init()
{
    //���ø���ĳ�ʼ��
    if ( !CCLayer::init() )
    {
        return false;
    }
	//�������ؼ�����
	setKeypadEnabled(true);
    //��ȡ�ɼ�����ߴ�
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//��ȡ�ɼ�����ԭ������
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//̹���ƶ���־λ
	tankMoveFlag = false;
	//�ڹ��ƶ��ı�־λ
	gunMoveFlag = false;
	//��������֡�ı�־λ
	gunIsAnimation = false;
	//���Ƶ�������ʱ��ı�־λ
	enemyBazookaFlag = false;
	//�ڹܵ���ת�Ƕ�
	gunAngle = 0;
	//��ʼ����ת�Ƕ�
	tankAngle = 0;
	//��ʼ���رղ˵���
	initButton();
	//��ʼ������̹�˵�ҡ��
	initTankJoyStick();
	//��ʼ������������ҡ��
	initGunJoyStick();
	//ͨ��ң�ظ���̹��������λ�õĻص�����
    this->schedule(schedule_selector(ControlLayer::updateTank));
    //ͨ��ң�ظ�����������Ļص�����
    this->schedule(schedule_selector(ControlLayer::updateGun),0.05);
    //�ҷ�̹�˷����ӵ��ķ���
    this->schedule(schedule_selector(ControlLayer::tankShoot),0.15);
    //���䵼���ķ���
    this->schedule(schedule_selector(ControlLayer::addtankBazooka),1.5);
    //������ײ���ķ���
    this->schedule(schedule_selector(ControlLayer::collisionDetection),0.05);
    //��ӵз�̹�˵ķ���
    this->schedule(schedule_selector(ControlLayer::addEnemyTank),2);
    //���µз��ڹܷ���
    this->schedule(schedule_selector(ControlLayer::updateEnemyGun),0.05);
    //�����ӵ��ķ���
    this->schedule(schedule_selector(ControlLayer::enemyTankShoot),3.5);
    //���㵼����ת�Ƕȵķ���
     this->schedule(schedule_selector(ControlLayer::calculateBazookaAngle));
    return true;
}
/*========================================��ʼ������=======================================================*/
//��ʼ��ҡ�˵ķ���
void ControlLayer::initTankJoyStick()
{
	//����ҡ�˵�������
	tankJoyStickBase = new SneakyJoystickSkinnedBase();
	//�Զ��ͷ�
	tankJoyStickBase->autorelease();
	//��ʼ��
	tankJoyStickBase->init();
	//���õ���ͼƬ
	tankJoyStickBase->setBackgroundSprite(CCSprite::create("direction.png"));
	//���òٿظ�ͼƬ
	tankJoyStickBase->setThumbSprite(CCSprite::create("center.png"));
	//����ҡ�˵���λ��
	tankJoyStickBase->setPosition
	(
		ccp
		(
			origin.x+220/2,//X����
			origin.y+220/2//Y����
		)
	);
    //����ҡ�˶���
	tankJoyStick = new SneakyJoystick();
	//�Զ��ͷ�
	tankJoyStick->autorelease();
	//��ҡ����ӽ�ҡ�˵���
	tankJoyStickBase->setJoystick(tankJoyStick);
	//��ҡ�˵�����ӽ�����
	this->addChild(tankJoyStickBase,DASHBOARD_LEVEL_DQX);
}
//��ʼ��ҡ�˵ķ���
void ControlLayer::initGunJoyStick()
{
	//����ҡ�˵�������
	gunJoyStickBase = new SneakyJoystickSkinnedBase();
	//�Զ��ͷ�
	gunJoyStickBase->autorelease();
	//��ʼ��
	gunJoyStickBase->init();
	//���õ���ͼƬ
	gunJoyStickBase->setBackgroundSprite(CCSprite::create("direction.png"));
	//���òٿظ�ͼƬ
	gunJoyStickBase->setThumbSprite(CCSprite::create("center.png"));
	//����ҡ�˵���λ��
	gunJoyStickBase->setPosition
	(
		ccp
		(
				origin.x+visibleSize.width-220/2,//X����
				origin.y+220/2					//Y����
		)
	);
    //����ҡ�˶���
	gunJoyStick = new SneakyJoystick();
	//�Զ��ͷ�
	gunJoyStick->autorelease();
	//��ҡ����ӽ�ҡ�˵���
	gunJoyStickBase->setJoystick(gunJoyStick);
	//��ҡ�˵�����ӽ�����
	this->addChild(gunJoyStickBase,DASHBOARD_LEVEL_DQX);
}
//��ʼ����ť�ķ���
void ControlLayer::initButton()
{
	//�����رղ˵���
	CCMenuItemImage *pPauseItem = CCMenuItemImage::create
	(
		"pause.png",     //ƽʱ��ͼƬ
		"pause_select.png",   //ѡ��ʱ��ͼƬ
		this,
		menu_selector(ControlLayer::menuPauseCallback)//���ʱִ�еĻص�����
	);
	//���ùرղ˵����λ��
	pPauseItem->setPosition
	(
		ccp
		(
			origin.x+visibleSize.width-pPauseItem->getContentSize().width/2,//X����
			origin.y+visibleSize.height-pPauseItem->getContentSize().height/2//Y����
		)
	);
	//�����˵�����
	pMenu = CCMenu::create(pPauseItem, NULL);
	//���ò˵�λ��
	pMenu->setPosition(CCPointZero);
	//���˵���ӵ�������
	this->addChild(pMenu, DASHBOARD_LEVEL_DQX);
}
/*=====================================����������̹�˵ķ���===================================================*/
//���µз�̹�˵��ڹ�
void ControlLayer::updateEnemyGun(float dt)
{
	//����̹�����������б���������Ϊ����
	for(int i = 0;i<gameLayer->enemyTankArray->count()/2;i++)
	{
		//��ȡ�����������
		EnemyTankGun *enemyGun = (EnemyTankGun*)gameLayer->enemyTankArray->objectAtIndex(2*i+1);
		if(enemyGun)
		{
			//��ȡ�з�������λ��
			CCPoint enemyGunPosition = enemyGun->getPosition();
			//��ȡ�ҷ�̹�˵�λ��
			CCPoint tankPosition =gameLayer->tankBody->getPosition();
			//����ط��������ҷ�̹�˵���������
			CCPoint enemyGunVector = ccp(tankPosition.x-enemyGunPosition.x,tankPosition.y-enemyGunPosition.y);
			//����ط���������ת�Ƕ�
			float enemyGunAngle = 0;
			//����Ƕ�
			if(enemyGunVector.x>0)
			{
				enemyGunAngle=90-atan(enemyGunVector.y/enemyGunVector.x)*180.0/3.1415926;
			}
			else
			{
				enemyGunAngle=-atan(enemyGunVector.y/enemyGunVector.x)*180.0/3.1415926-90;
			}
			//�з�����ִ����ת����
			enemyGun->runAction
			(
				CCSequence::create
				(
					//���е�����ͬʱ��ת����֮����ܽ�����һ�ε���ת
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
	//��ȡ��ǰ̹��λ��
	CCPoint tankPosition=gameLayer->tankBody->getPosition();
	//��ȡ��ǰ̹�˵Ĵ�С
	CCSize tankSize  = gameLayer->tankBody->getContentSize();
	//�������һ����ǰ�ɻ���λ��
	float hx=tankPosition.x+(gameLayer->SHOOT_STATE+4)/2*joyStickV.x;
	float hy=tankPosition.y+(gameLayer->SHOOT_STATE+4)/2*joyStickV.y;
	//��ȡ�ɻ�����λ��
	CCPoint newPosition = ccp(hx,hy);
	//���Խ��÷���λ�ò���
	if(hx<(origin.x+tankSize.width/2.0)||hx>(origin.x+visibleSize.width - tankSize.width / 2.0))
	{
		newPosition.x = tankPosition.x;
	}
	//���Խ��÷���λ�ò���
	if(hy<(origin.y+tankSize.height / 2.0)||hy>(origin.y+visibleSize.height - tankSize.height/2.0))
	{
		newPosition.y = tankPosition.y;
	}
	return newPosition;
}
//��������鿴��Ϸҡ��״̬��Ӣ�۾���
void ControlLayer::updateTank()
{
	//��ȡ��ǰ̹��λ��
	CCPoint tankPosition=gameLayer->tankBody->getPosition();
	//��ȡ��ǰ̹�˵Ĵ�С
	CCSize tankSize  = gameLayer->tankBody->getContentSize();
	//��ȡҡ�˵��ٶ�ʸ��
	CCPoint joyStickV=tankJoyStick->getVelocity();
	//���ٶȴ�С
	if(joyStickV.x==0&&joyStickV.y==0)
	{
		if(origin.y+tankPosition.y-1>origin.y+tankSize.height/2)
		{
			gameLayer->tankBody->setPosition(ccp(tankPosition.x,tankPosition.y-1));
			gameLayer->tankGun->setPosition(ccp(tankPosition.x,tankPosition.y-1));
		}
		return;
	}
	//����̹�˵���ת�Ƕ�
	if((joyStickV.x>0))
	{
		tankAngle=90-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926;
	}
	else
	{
		tankAngle=-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926-90;
	}
	//̹��ִ���ƶ���������ת����
	gameLayer->tankBody->setRotation(tankAngle);
	//����̹�˵���λ��
	CCPoint newPosition = this->calculationPosition(joyStickV);
	//����̹�˵���λ��
	gameLayer->tankGun->setPosition(newPosition);
	//������������λ��
	gameLayer->tankBody->setPosition(newPosition);

}
//��������鿴��Ϸҡ��״̬��Ӣ�۾���
void ControlLayer::updateGun(float dt)
{
	//��һ������û���꣬�������µĶ���
	if(gunMoveFlag)
	{
		return;
	}
	//��ȡҡ�˵��ٶ�ʸ��
	CCPoint joyStickV=gunJoyStick->getVelocity();
	if(joyStickV.x==0&&joyStickV.y==0)
	{
		return;
	}
	//���ö���ִ���б�־λ
	gunMoveFlag = true;
	if(joyStickV.x>0)
	{
		gunAngle=90-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926;
	}
	else
	{
		gunAngle=-atan(joyStickV.y/joyStickV.x)*180.0/3.1415926-90;
	}
	//����ִ����ת����
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
/*=====================================��ײ���ķ���===================================================*/
//������ײ���ķ���
void ControlLayer::collisionDetection(float dt)
{
	//�ҷ��ӵ���з�̹����ײ�ķ���
	this->tankBullet_enemyTank(gameLayer->tankBullet,1);
	//�ҷ�������з�̹����ײ�ķ���
	this->tankBullet_enemyTank(gameLayer->tankBazooka,3);
	//�ҷ��ӵ���з��ϰ������ײ����
	this->tankBullet_enemy(gameLayer->tankBullet,1);
	//�ҷ�������з��ϰ������ײ����
	this->tankBullet_enemy(gameLayer->tankBazooka,3);
	//�ҷ�̹����з�̹����ײ�ķ���
	this->tank_enemyTank();
	//�ҷ�̹�����ϰ�����ײ�ķ���
	this->tank_barrier(gameLayer->barrierArray);
	//�ҷ�̹�˳Ե����ߵķ���
	this->tank_props(gameLayer->propsArray);
	//�з��ӵ����ҷ�̹����ײ�ķ���
	this->enemyTankBullet_tank(gameLayer->enemyBullet,2);
	//�з��������ҷ�̹����ײ�ķ���
	this->enemyTankBullet_tank(gameLayer->enemyBazooka,3);
}
//�ҷ�̹�����ϰ�����ײ�ķ���
void ControlLayer::tank_barrier(CCArray* barrierArray)
{
	//�����ϰ����б�
	for(int i = 0;i<barrierArray->count();i++)
	{
		//��ȡ�ϰ������
		Barrier* barrier= (Barrier*)barrierArray->objectAtIndex(i);
		//ִ����ײ���
		if(barrier->isCollision(gameLayer->tankBody))
		{
			/*========================================================================*/
			if(gameLayer->changeBlood(-5))
			{
				//ʧ�ܽ���
				loseView();
			}
			//����״̬��һ
			eatProp_TWO(-1);
			//���ӷ���
			gameLayer->addScore(15);
			//��ȡ�ϰ��ﾫ���λ��
			CCPoint position = barrier->getPosition();
			//����ʱ��
			float time = position.y/480*20;
			//�Ƴ��б��еľ������
			gameLayer->barrierArray->removeObject(barrier);
			//�Ƴ������ϵľ������
			gameLayer->removeChild(barrier);
			//��ӱ�ը����
			addBomb(position,time,0);
			//���ը�۾���
			addMarks(position,time);
			break;
		}
	}
}

//�ҷ�̹�˳Ե����ߵ� ����
void ControlLayer::tank_props(CCArray* propsArray)
{
	//���������б����б�
	for(int i = propsArray->count()-1;i>=0;i--)
	{
		Props* props = (Props*)propsArray->objectAtIndex(i);
		//ִ����ײ���
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
					//���ӷ���
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
	//��ȡ���������б�
	CCArray* tankBazookaArray = gameLayer->tankBazooka->getChildren();
	//������������б�Ϊ��
	if(tankBazookaArray)
	{
		for(int i=0;i<tankBazookaArray->count();i++)
		{
			TankBazooka* tankBazooka = (TankBazooka*)tankBazookaArray->objectAtIndex(i);
			if(tankBazooka->platformPosition())
			{
				//��ӱ�ը����ķ���
				this->addBomb(tankBazooka->getPosition(),10,0);
				//ɾ����������
				this->removeTankBazooka(tankBazooka);
			}
		}
	}
}
//�ҷ��ӵ���з�̹����ײ�ķ���
void ControlLayer::tankBullet_enemyTank(CCSpriteBatchNode* bulletNode,int bolldReduce)
{
	//��ȡ�ҷ��ӵ������б�
	CCArray* tankBulletArray = bulletNode->getChildren();
	//����ҷ��ӵ��б�Ϊ��
	if(tankBulletArray)
	{
		//�����ҷ��ӵ��б�
		for(int i=0;i<tankBulletArray->count();i++)
		{
			//��ȡ�ҷ��ӵ�����
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
			//�����з�̹���б�
			for(int j = 0;j<enemyTankArray->count()/2;j++)
			{
				//��ȡ�з�̹�˶���
				EnemyTank* enemyTank= (EnemyTank*)enemyTankArray->objectAtIndex(2*j);
				//��ȡ�з�̹����������
				EnemyTankGun* enemyTankGun= (EnemyTankGun*)enemyTankArray->objectAtIndex(2*j+1);
				//ִ����ײ���
				if(enemyTank->isCollision(tankBullet))
				{

					//���ӷ���
					gameLayer->addScore(bolldReduce*5);
					/*========================================================================*/
					//��ȡ̹�˾����λ��
					CCPoint position = enemyTank->getPosition();
					//��ȡ�ӵ������λ��
					CCPoint bulletPosition = tankBullet->getPosition();
					//�Ƴ��ҷ��ӵ�����removeTankBullet
					bulletNode->removeChild(tankBullet,true);
					//����ʱ��
					float time = position.y/480*20;
					//��ȡѪ��
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
						//�Ƴ��з�̹������
						gameLayer->enemyTankArray->removeObject(enemyTankGun);
						//�Ƴ��з�̹������
						gameLayer->removeChild(enemyTankGun);
						//�Ƴ��б��еľ������
						gameLayer->enemyTankArray->removeObject(enemyTank);
						//�Ƴ��з�̹��
						gameLayer->removeChild(enemyTank);
						//��ӱ�ը����
						addBomb(position,time,0);
						//��ӵ���
						addProps(position);
						//���ը�۾���
						addMarks(position,time);
					}
					else
					{
						//��ӱ�ը����
						addBomb(bulletPosition,time,1);
					}
					break;
				}
			}
		}
	}
}
//�ҷ��ӵ���з��ϰ������ײ����
void ControlLayer::tankBullet_enemy(CCSpriteBatchNode* bulletNode,int bolldReduce)
{
	//��ȡ�ҷ��ӵ������б�
	CCArray* tankBulletArray = bulletNode->getChildren();
	//����ҷ��ӵ��б�Ϊ��
	if(tankBulletArray)
	{
		//�����ҷ��ӵ��б�
		for(int i = 0;i<tankBulletArray->count();i++)
		{
			//��ȡ�ҷ��ӵ�����
			CCSprite* tankBullet;
			if(bolldReduce>1)
			{
				tankBullet=(TankBazooka*)tankBulletArray->objectAtIndex(i);
			}
			else
			{
				tankBullet=(CCSprite*)tankBulletArray->objectAtIndex(i);
			}
			//��ȡ�ϰ����б�
			CCArray* barrierArray = gameLayer->barrierArray;
			//�����ϰ����б�
			for(int j = 0;j<barrierArray->count();j++)
			{
				//��ȡ�ϰ������
				Barrier* barrier= (Barrier*)barrierArray->objectAtIndex(j);
				//ִ����ײ���
				if(barrier->isCollision(tankBullet))
				{
					//���ӷ���
					gameLayer->addScore(bolldReduce*5);
					/*========================================================================*/
					//��ȡ�ϰ��ﾫ���λ��
					CCPoint position = barrier->getPosition();
					//��ȡ�ӵ���λ��
					CCPoint bulletPosition = tankBullet->getPosition();
					//�Ƴ��ӵ�
					bulletNode->removeChild(tankBullet,true);
					//����ʱ��
					float time = position.y/480*20;
					//Ѫ������
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
						//�Ƴ��б��еľ������
						gameLayer->barrierArray->removeObject(barrier);
						//�Ƴ������ϵľ������
						gameLayer->removeChild(barrier);
						//��ӱ�ը����
						addBomb(position,time,0);
						//��ӵ���
						addProps(position);
						//���ը�۾���
						addMarks(position,time);
					}
					else
					{
						//��ӱ�ը����
						addBomb(bulletPosition,time,1);
					}
					break;
				}
			}
		}
	}
}
//�ҷ�̹����з�̹����ײ�ķ���
void ControlLayer::tank_enemyTank()
{
	//��ȡ�з���̹���б�
	CCArray* enemyTankArray = gameLayer->enemyTankArray;
	//�����з�̹���б�
	for(int j = 0;j<enemyTankArray->count()/2;j++)
	{
		//��ȡ�з�̹�˶���
		EnemyTank* enemyTank= (EnemyTank*)enemyTankArray->objectAtIndex(2*j);
		//��ȡ�з�̹����������
		EnemyTankGun* enemyTankGun= (EnemyTankGun*)enemyTankArray->objectAtIndex(2*j+1);
		//ִ����ײ���
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
				//ʧ�ܽ���
				loseView();
			}
			//����״̬��һ
			eatProp_TWO(-1);
			//���ӷ���
			gameLayer->addScore(15);
			//��ȡ̹�˾����λ��
			CCPoint position = enemyTank->getPosition();
			//�Ƴ��з�̹������
			gameLayer->enemyTankArray->removeObject(enemyTankGun);
			//�Ƴ��з�̹������
			gameLayer->removeChild(enemyTankGun);
			//�Ƴ��б��еľ������
			gameLayer->enemyTankArray->removeObject(enemyTank);
			//�Ƴ��з�̹��
			gameLayer->removeChild(enemyTank);
			//����ʱ��
			float time = position.y/480*20;
			//��ӱ�ը����
			addBomb(position,time,0);
			//���ը�۾���
			addMarks(position,time);
		}
	}
}
//�з��ӵ����ҷ�̹����ײ�ķ���
void ControlLayer::enemyTankBullet_tank(CCSpriteBatchNode* enemyBulletNode,int bloodReduce)
{
	//��ȡ�з��ӵ��б�
	CCArray* enemyBulletArray = enemyBulletNode->getChildren();
	if(enemyBulletArray)
	{
		//�����з��ӵ��б�
		for(int i = 0;i<enemyBulletArray->count();i++)
		{
			//��ȡ�з�̹���ӵ��������
			CCSprite* enemyBullet = (CCSprite*)enemyBulletArray->objectAtIndex(i);
			//��ȡ̹���ӵ�����Ĵ�С
			CCSize enemyBulletSize = enemyBullet->getContentSize();
			//��ȡ�ҷ�̹�˾���Ĵ�С
			CCSize tankSize = gameLayer->tankBody->getContentSize();
			//��ײ�ķ�Χ�ļ���
			float width = enemyBulletSize.width+tankSize.width;
			float height = enemyBulletSize.height+tankSize.height;
			//��ȡ�з�̹���ӵ�����λ��
			CCPoint enemyBulletPositon = enemyBullet->getPosition();
			//��ȡ�ҷ�̹��λ��
			CCPoint tankPosition = gameLayer->tankBody->getPosition();
			//ִ���ж�
			if(abs(enemyBulletPositon.x-tankPosition.x)<=width/2&&abs(enemyBulletPositon.y-tankPosition.y)<=height/2)
			{
				/*====================================================================================*/
				if(gameLayer->changeBlood(-bloodReduce))
				{
					//ʧ�ܽ���
					loseView();
				}
				//����״̬��һ
				eatProp_TWO(-1);
				//��ȡ��Ҫ�Ƴ��ĵз�̹���ӵ���λ��
				CCPoint enemyBulletPosition = enemyBullet->getPosition();
				//�Ƴ����ָ��ӵ�
				enemyBulletNode->removeChild(enemyBullet,true);
				//������Ҫ���ű�ը��Ч�ľ���
				CCSprite *bomb = CCSprite::create("bomb.png");
				//���þ����λ��
				bomb->setPosition(ccp(enemyBulletPosition.x,enemyBulletPosition.y));
				//��������ӵ�����
				gameLayer->addChild(bomb,BOMB_LEVEL_DQX);
				//ִ�ж���
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
//�Ƴ���ը����ķ���
void ControlLayer::removeBomb(CCNode* node,void* param)
{
	//��ȡ��ը����
	CCSprite* bomb = (CCSprite*)node;
	//�Ƴ���ը����
	gameLayer->removeChild(bomb);
}
//�з�̹�˷����ӵ��ķ���
void ControlLayer::enemyTankShoot(float dt)
{

	for(int i = 0;i<gameLayer->enemyTankArray->count()/2;i++)
	{
		//��ȡ�з�̹�˾������
		EnemyTankGun* enemyTankGun = (EnemyTankGun*)gameLayer->enemyTankArray->objectAtIndex(2*i+1);
		//����з�����û�н���֡����
		if(!enemyTankGun->enemyIsAnimation)
		{
			//�������һ��΢С�Ķ����ӳ�ʱ��
			int index = rand()%3;
			//����wait����
			CCDelayTime* waitAction = CCDelayTime::create(index*0.005);
			//��֡�����ı�־λ��Ϊtrue
			enemyTankGun->enemyIsAnimation = true;
			//�ڹ����Ȳ���һ�¶���Ȼ��ִ������ڵ�����
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
		//��ȡ�з�̹�˾������
		Barrier* barier = (Barrier*)gameLayer->barrierArray->objectAtIndex(i);
		//����з�����û�н���֡����
		if(barier->barrierIndex==5)
		{
			//������ȡ�ϰ����λ��
			CCPoint barierPosition = barier->getPosition();
			//��ȡ�ҷ�̹�˵�λ��
			CCPoint tankPosition = gameLayer->tankBody->getPosition();
			//�õ��ϰ�����̹�˵���������
			CCPoint vector2f = ccp(tankPosition.x-barierPosition.x,tankPosition.y-barierPosition.y);
			//�������
			vector2f = ccpNormalize(vector2f);
			//�����������
			CCSprite* barierBullet = CCSprite::create((gameLayer->enemyName[barier->barrierIndex-1]).c_str());
			//���ӵ���ӵ�CCSpriteBatchNode��
			gameLayer->enemyBazooka->addChild(barierBullet);
			//���þ����λ��
			barierBullet->setPosition(ccp(barierPosition.x,barierPosition.y));
			//�����ڵ�����ת�Ƕ�
			float enemyAngle = 0;
			//����Ƕ�
			if(vector2f.x>0)
			{
				enemyAngle=90-atan(vector2f.y/vector2f.x)*180.0/3.1415926;
			}
			else
			{
				enemyAngle=-atan(vector2f.y/vector2f.x)*180.0/3.1415926-90;
			}
			//�����ƶ�����
			CCMoveTo* bulletMoveAction = CCMoveTo::create(5,ccp(barierPosition.x+800*vector2f.x,barierPosition.y+800*vector2f.y));
			//�����ڵ�����ת�Ƕ�
			barierBullet->setRotation(enemyAngle);
			//ִ�ж���
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
//Ϊ�з�̹�˷����ӵ��ķ���
void ControlLayer::addEnemyTankBullet(CCNode* node,void* param)
{
	//��ȡ�õз�̹������
	EnemyTankGun* enemyTankGun = (EnemyTankGun*)node;
	//�����������
	CCSprite* enemyTankBullet = CCSprite::create((gameLayer->enemyName[enemyTankGun->index-1]).c_str());
	//ִ���ж�
	switch(enemyTankGun->index)
	{
		case 1:
		case 2:
			//���ӵ���ӵ�CCSpriteBatchNode��
			gameLayer->enemyBullet->addChild(enemyTankBullet);
			break;
		case 3:
		case 4:
			//���ӵ���ӵ�CCSpriteBatchNode��
			gameLayer->enemyBazooka->addChild(enemyTankBullet);
			break;
	}
	//������Ч���ŵı�־λ
	enemyTankGun->enemyIsAnimation = false;
	//���õз�̹���ӵ���ת�Ƕ�
	enemyTankBullet->setRotation(enemyTankGun->getRotation());
	//��ȡ��������ת�Ƕ�
	float angleTemp=enemyTankGun->getRotation();
	//ת��Ϊ���Ƚ�
	angleTemp=angleTemp*3.1415926f/180.0f;
	//��ȡ�з�̹��������λ��
	CCPoint enemyGunPosition = enemyTankGun->getPosition();
	//�����ӵ�����λ��
	enemyTankBullet->setPosition(ccp(enemyGunPosition.x+40*sin(angleTemp),enemyGunPosition.y+40*cos(angleTemp)));
	//�����ӵ��Ķ���
	CCMoveTo* tankBulletMoveAction  = CCMoveTo::create(8,ccp(enemyGunPosition.x+800*sin(angleTemp),enemyGunPosition.y+800*cos(angleTemp)));
	//ִ�ж���
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
	//��ȡ�ӵ�����
	CCSprite* uselessSprite = (CCSprite*)node;
	//�Ƴ��ӵ�����
	gameLayer->enemyBullet->removeChild(uselessSprite,true);
}
void ControlLayer::removeEnemyTankBazooka(CCNode* node,void* param)
{
	//��ȡը������
	CCSprite* uselessSprite = (CCSprite*)node;
	//�Ƴ�ըը������
	gameLayer->enemyBazooka->removeChild(uselessSprite,true);
}
//�����ڵ��ķ���
void ControlLayer::tankShoot(float dt)
{
	//�������û�н���֡����
	if(!gunIsAnimation)
	{
		//�����ӳ�ʱ��
		CCDelayTime* waitAction = CCDelayTime::create(0.1*(3-gameLayer->SHOOT_STATE));
		//��֡�����ı�־λ��Ϊtrue
		gunIsAnimation = true;
		//�ڹ����Ȳ���һ�¶���Ȼ��ִ������ڵ�����
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
	//֡��������������ñ�־λΪfalse
	gunIsAnimation = false;
	//�����ӵ�����
	CCSprite* tankBullet = CCSprite::create("tankBullet.png");
	//���ӵ���ӵ�CCSpriteBatchNode��
	gameLayer->tankBullet->addChild(tankBullet);
	//����̹���ӵ���ת�Ƕ�
	tankBullet->setRotation(gameLayer->tankGun->getRotation());
	//��ȡ��������ת�Ƕ�
	float angleTemp=gameLayer->tankGun->getRotation();
	//ת��Ϊ���Ƚ�
	angleTemp=angleTemp*3.1415926f/180.0f;
	//��ȡ������λ��
	CCPoint gunPosition = gameLayer->tankGun->getPosition();
	//�����ӵ�����λ��
	tankBullet->setPosition(ccp(gunPosition.x+40*sin(angleTemp),gunPosition.y+40*cos(angleTemp)));
	//�����ӵ��Ķ���
	CCMoveTo* tankBulletMoveAction  = CCMoveTo::create(4,ccp(gunPosition.x+800*sin(angleTemp),gunPosition.y+800*cos(angleTemp)));
	//ִ�ж���
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
//����ӵ�
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
		//���������
		int index = rand()%2;
		//��ȡ׷�ٵ�����λ��
		CCPoint enemyPosition = this->getRandEnemyPosition();
		//��ȡ��������ת�Ƕ�
		float angleTemp=gameLayer->tankBody->getRotation();
		//ת��Ϊ���Ƚ�
		angleTemp=angleTemp*3.1415926f/180.0f;
		//��ȡ������λ��
		CCPoint bodyPosition = gameLayer->tankBody->getPosition();
		//�����ڵ�����
		TankBazooka* tankBazooka = TankBazooka::creates(enemyPosition,ccp(3*sin(angleTemp),3*cos(angleTemp)));
		//���ӵ���ӵ�CCSpriteBatchNode��
		gameLayer->tankBazooka->addChild(tankBazooka);
		//����̹���ӵ���ת�Ƕ�
		tankBazooka->setRotation(gameLayer->tankBody->getRotation());
		//�����ӵ�����λ��
		tankBazooka->setPosition(ccp(bodyPosition.x+40*sin(angleTemp),bodyPosition.y+40*cos(angleTemp)));
	}
}
void ControlLayer::removeTankBullet(CCNode* node,void* param)
{
	//��ȡ�������
	CCSprite* uselessSprite =(CCSprite*)node;
	//��spriteBatchNode���Ƴ�����
	gameLayer->tankBullet->removeChild(uselessSprite,true);
}
void ControlLayer::removeTankBazooka(CCNode* node)
{
	//��ȡ�������
	TankBazooka* uselessSprite =(TankBazooka*)node;
	//��spriteBatchNode���Ƴ�����
	gameLayer->tankBazooka->removeChild(uselessSprite,true);
}
/*=====================================�Ƴ��з�̹�˵ķ���===================================================*/
//�ɹ�����֮��Ĺ���
void ControlLayer::reparation_for_next_level()
{
	//��ͣ���Ʋ�Ļص�����
	this->pauseSchedulerAndActions();
	//��ͣ��Ϸ����Ļص�����
	gameLayer->pauseSchedulerAndActions();
	//���Ʋ�İ�ť����Ϊ������
	pMenu->setEnabled(false);
	//ȡ�����Ʋ㷵�ؼ�����
	setKeypadEnabled(false);
	//����ӵ��б�
	gameLayer->tankBullet->removeAllChildrenWithCleanup(true);
	//����ڵ��б�
	gameLayer->tankBazooka->removeAllChildrenWithCleanup(true);
	//����ڵ��б�
	gameLayer->enemyBullet->removeAllChildrenWithCleanup(true);
	//����ڵ��б�
	gameLayer->enemyBazooka->removeAllChildrenWithCleanup(true);
	//��չ�ľ�б�
	gameLayer->trees->removeAllChildren();
	//���ը���б�
	gameLayer->marks->removeAllChildren();
	//��ǰ�ؿ���1
	gameLayer->passTemp++;
	//���¼����ܹ���
	gameLayer->enemyGroupCount = 3*pow(1.2,gameLayer->passTemp);
	//���õ�ǰ��������
	gameLayer->currentEnemyGroup = 0;
}
//��һ�صķ���
void ControlLayer::nextLevel()
{
	//�ָ����Ʋ�Ļص�����
	this->resumeSchedulerAndActions();
	//�ָ���Ϸ����Ļص�����
	gameLayer->resumeSchedulerAndActions();
	//���Ʋ�İ�ť����Ϊ����
	pMenu->setEnabled(true);
	//ȡ�����Ʋ㷵�ؼ�����
	setKeypadEnabled(true);
}
//��ӵз�̹�˵ķ���
void ControlLayer::addEnemyTank(float dt)
{
	if(++(gameLayer->currentEnemyGroup)>(gameLayer->enemyGroupCount))
	{
		//��ȡ�з�̹�������ѧ
		int eCount = gameLayer->enemyTankArray->count();
		//��ȡ�ϰ��������С
		int bCount = gameLayer->barrierArray->count();
		//��ȡ���߾��������ѧ
		int pCount = gameLayer->propsArray->count();
		//ִ���ж�
		if(eCount==0 && bCount==0&&pCount==0)
		{

			//�ɹ�����֮��Ĺ���׼������
			this->reparation_for_next_level();
			//������͸����
			NextLevelDialogLayer *nextLevelDialogLayer = NextLevelDialogLayer::create();
			//���ð�͸�����λ��
			nextLevelDialogLayer->setPosition(ccp(0,0));
			//��ӵ������
			this->addChild(nextLevelDialogLayer,DASHBOARD_LEVEL_DQX);
		}
		return;
	}
	//����1-10�������
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
//��ӵз�̹�˵ķ���
void ControlLayer::addEnemyTankGroup_ONE()
{
	//���һ���ط�̹�˻��ϰ��ķ����ķ���
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8,origin.y+visibleSize.height));
	addEnemyTank_ONE(ccp(origin.x+visibleSize.width/8*7,origin.x+visibleSize.height));
}
void ControlLayer::addEnemyTankGroup_TWO()
{
	//���һ���ط�̹�˻��ϰ��ķ����ķ���
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
//���һ���ط�̹�˻��ϰ��ķ����ķ���
void ControlLayer::addEnemyTank_ONE(CCPoint position)
{
	//�˴�д��ӵз�̹�˷����ӵ��ķ���
	EnemyTank *enemyTank = EnemyTank::creates("enemybody1.png",1,1);
	//���õз�̹�˵�λ��
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//����ʱ��
	float time = (position.y+60)/480*20;
	//�з�̹��ִ�ж���
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//�����з���������
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun1.png",1);
	//���þ���λ��
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//���þ���ê��
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//����̹�˹�����ת�Ƕ�
	enemyTankGun->setRotation(180);
	//����ִ�ж���
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//��������ӵ�����
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//��������ӵ��б�
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addEnemyTank_TWO(CCPoint position)
{
	//�˴�д��ӵз�̹�˷����ӵ��ķ���
	EnemyTank *enemyTank = EnemyTank::creates("enemybody2.png",2,1);
	//���õз�̹�˵�λ��
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//����ʱ��
	float time = (position.y+60)/480*20;
	//�з�̹��ִ�ж���
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//�����з���������
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun2.png",2);
	//���þ���λ��
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//���þ���ê��
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//����̹�˹�����ת�Ƕ�
	enemyTankGun->setRotation(180);
	//����ִ�ж���
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//��������ӵ�����
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//��������ӵ��б�
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addEnemyTank_THREE(CCPoint position)
{
	//�˴�д��ӵз�̹�˷����ӵ��ķ���
	EnemyTank *enemyTank = EnemyTank::creates("enemybody3.png",3,2);
	//���õз�̹�˵�λ��
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//����ʱ��
	float time = (position.y+60)/480*20;
	//�з�̹��ִ�ж���
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//�����з���������
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun3.png",2);
	//���þ���λ��
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//���þ���ê��
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//����̹�˹�����ת�Ƕ�
	enemyTankGun->setRotation(180);
	//����ִ�ж���
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//��������ӵ�����
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//��������ӵ��б�
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addEnemyTank_FORE(CCPoint position)
{
	//�˴�д��ӵз�̹�˷����ӵ��ķ���
	EnemyTank *enemyTank = EnemyTank::creates("enemybody4.png",4,2);
	//���õз�̹�˵�λ��
	enemyTank->setPosition(ccp(position.x,position.y+60));
	//����ʱ��
	float time = (position.y+60)/480*20;
	//�з�̹��ִ�ж���
	enemyTank->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTank), NULL),
			NULL
		)
	);
	//�����з���������
	EnemyTankGun* enemyTankGun = EnemyTankGun::creates("enemygun1.png",4);
	//���þ���λ��
	enemyTankGun->setPosition(ccp(position.x,position.y+60));
	//���þ���ê��
	enemyTankGun->setAnchorPoint(ccp(0.5,0.27));
	//����̹�˹�����ת�Ƕ�
	enemyTankGun->setRotation(180);
	//����ִ�ж���
	enemyTankGun->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeEnemyTankGun), NULL),
			NULL
		)

	);
	//��������ӵ�����
	gameLayer->addChild(enemyTank,TANK_LEVEL_DQX);
	gameLayer->addChild(enemyTankGun,TANK_LEVEL_DQX);
	//��������ӵ��б�
	gameLayer->enemyTankArray->addObject(enemyTank);
	gameLayer->enemyTankArray->addObject(enemyTankGun);
}
void ControlLayer::addBarrier(CCPoint position)
{
	//�˴�д����ϰ���ķ���
	Barrier *barrier = Barrier::creates("barrier.png",6,2);
	//���õ��ϰ����λ��
	barrier->setPosition(ccp(position.x,position.y+60));
	//����ʱ��
	float time = (position.y+60)/480*20;
	//ִ�ж���
	barrier->runAction
	(
		CCSequence::create
		(
			CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)),
			CCCallFuncND::create(this, callfuncND_selector(ControlLayer::removeBarrier), NULL),
			NULL
		)
	);
	//���ϰ����б�������ϰ������
	gameLayer->barrierArray->addObject(barrier);
	//���ϰ�����ӵ�������
	gameLayer->addChild(barrier,TANK_LEVEL_DQX);
}
void ControlLayer::addTower(CCPoint position)
{
	//�˴�д����ϰ���ķ���
	Barrier *barrier = Barrier::creates("tower.png",5,2);
	//�����ϰ����λ��
	barrier->setPosition(ccp(position.x,position.y+60));
	//����ʱ��
	float time = (position.y+60)/480*20;
	//�ϰ���ִ�ж���
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
//�Ƴ��з�̹�˵Ļص�����
void ControlLayer::removeEnemyTank(CCNode* node,void* param)
{
	//��ȡ��Ҫ�Ƴ���̹��
	EnemyTank* uselessSprite = (EnemyTank*)node;
	//�Ƴ��з�̹��
	gameLayer->removeChild(uselessSprite);
	//���б����Ƴ��з�̹��
	gameLayer->enemyTankArray->removeObject(uselessSprite);
}
//�Ƴ��ط�̹���ڹܵĻص�����
void ControlLayer::removeEnemyTankGun(CCNode* node,void* param)
{
	//��ȡ��Ҫ�Ƴ���̹������
	EnemyTankGun* uselessSprite = (EnemyTankGun*)node;
	//�Ƴ��з�̹������
	gameLayer->removeChild(uselessSprite);
	//���б����Ƴ��з�̹������
	gameLayer->enemyTankArray->removeObject(uselessSprite);
}
void ControlLayer::removeBarrier(CCNode* node,void* param)
{
	//��ȡ��Ҫ�Ƴ��ľ���
	Barrier* uselessSprite = (Barrier*)node;
	//���б����Ƴ�����
	gameLayer->barrierArray->removeObject(uselessSprite);
	//�Ӳ������Ƴ�����
	gameLayer->removeChild(uselessSprite);
}
//��Ѫ
void ControlLayer::eatProp_ONE()
{

	gameLayer->changeBlood(5);
}
//���ӵ�
void ControlLayer::eatProp_TWO(int state_increment)
{
	//���㷢���ӵ���״̬
	gameLayer->SHOOT_STATE = ((gameLayer->SHOOT_STATE+state_increment)>3)?3:(gameLayer->SHOOT_STATE+state_increment);
	//�����ӵ���״̬��СΪ0
	if(gameLayer->SHOOT_STATE<=0)
	{
		gameLayer->SHOOT_STATE = 0;
	}
}
//����
void ControlLayer::eatProp_THREE()
{
	//����ӵ��б�
	gameLayer->tankBullet->removeAllChildrenWithCleanup(true);
	//����ڵ��б�
	gameLayer->tankBazooka->removeAllChildrenWithCleanup(true);
	//����ڵ��б�
	gameLayer->enemyBullet->removeAllChildrenWithCleanup(true);
	//����ڵ��б�
	gameLayer->enemyBazooka->removeAllChildrenWithCleanup(true);
	//�����ϰ����б�
	for(int i = 0;i<gameLayer->barrierArray->count();i++)
	{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
			(
					"sound/grenada.ogg"
			);
		}
		//��ȡ�ϰ������
		Barrier* barrier= (Barrier*)(gameLayer->barrierArray->objectAtIndex(i));
		//���ӷ���
		gameLayer->addScore(15);
		//��ȡ�ϰ��ﾫ���λ��
		CCPoint position = barrier->getPosition();
		//����ʱ��
		float time = position.y/480*20;
		//��ӱ�ը����
		addBomb(position,time,0);
		//��ӵ���
		addProps(position);
		//���ը�۾���
		addMarks(position,time);
	}
	for(int i = 0;i<gameLayer->barrierArray->count();i++)
	{
		//�Ƴ��ϰ���
		gameLayer->removeChild((Barrier*)gameLayer->barrierArray->objectAtIndex(i));
	}
	//����ϰ����б�
	gameLayer->barrierArray->removeAllObjects();
	//�����з�̹���б�
	for(int j = 0;j<gameLayer->enemyTankArray->count()/2;j++)
	{
		if(JNIUtil::getInt("effectFlag",1)==1)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect
			(
					"sound/grenada.ogg"
			);
		}
		//��ȡ�з�̹�˶���
		EnemyTank* enemyTank= (EnemyTank*)(gameLayer->enemyTankArray->objectAtIndex(2*j));
		//��ȡ�з�̹����������
		EnemyTankGun* enemyTankGun= (EnemyTankGun*)(gameLayer->enemyTankArray->objectAtIndex(2*j+1));
		//���ӷ���
		gameLayer->addScore(15);
		//��ȡ̹�˾����λ��
		CCPoint position = enemyTank->getPosition();
		//����ʱ��
		float time = position.y/480*20;
		//��ӱ�ը����
		addBomb(position,time,0);
		//��ӵ��߾���
		addProps(position);
		//���ը�۾���
		addMarks(position,time);
	}
	for(int i = 0;i<gameLayer->enemyTankArray->count()/2;i++)
	{
		//��ȡ�з�̹�˶���
		EnemyTank* enemyTank= (EnemyTank*)gameLayer->enemyTankArray->objectAtIndex(2*i);
		//��ȡ�з�̹����������
		EnemyTankGun* enemyTankGun= (EnemyTankGun*)gameLayer->enemyTankArray->objectAtIndex(2*i+1);
		//�Ƴ��з�̹��
		gameLayer->removeChild(enemyTank);
		//�Ƴ��з�̹���ڹ�
		gameLayer->removeChild(enemyTankGun);
	}
	//��յз�̹���б�
	gameLayer->enemyTankArray->removeAllObjects();
}
//���ը�۾���ķ���
void ControlLayer::addMarks(CCPoint position,float time)
{
	//�������۾���
	CCSprite* mark = CCSprite::create("mark1.png");
	//���õ��۾����λ��
	mark->setPosition(ccp(position.x,position.y));
	//���õ��۾����λ��
	mark->runAction(CCMoveTo::create(time,ccp(position.x,ENEMYMINPOSITION)));
	//�����۾�����ӵ����ڵ�
	gameLayer->marks->addChild(mark);
}
//��ӱ�ը����ķ���
void ControlLayer::addBomb(CCPoint position,float time,int animIndex)
{
	//������Ҫ���ű�ը��Ч�ľ���
	CCSprite *bomb = CCSprite::create("bomb.png");
	//���þ����λ��
	bomb->setPosition(ccp(position.x,position.y));
	//��������ӵ�����
	gameLayer->addChild(bomb,BOMB_LEVEL_DQX);
	//ִ�ж���
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
//�Ƴ���ը����ķ���
void ControlLayer::removeProps(CCNode* node,void* param)
{
	//��ȡ���߾���
	Props* props = (Props*)node;
	//���б����Ƴ�����
	gameLayer->propsArray->removeObject(props);
	//�Ӳ������Ƴ�����
	gameLayer->removeChild(props);
}
//��ӵ��߾���
void ControlLayer::addProps(CCPoint position)
{
	//�����������
	int index = rand()%20;
	//�����ж�
	if(index>=15)
	{
		//�������߾���
		Props* props = Props::creates(gameLayer->pPName[index-15],index-15);
		//���õ��߾����λ��
		props->setPosition(ccp(position.x,position.y));
		//�����߾�����ӽ�����
		gameLayer->propsArray->addObject(props);
		//�����߾�����ӽ���
		gameLayer->addChild(props,PROPS_LEVEL_DQX);
		//ִ�ж���
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
//ʵ��ControlLayer���е�menuPauseCallback����������رղ˵������¼�
void ControlLayer::menuPauseCallback(CCObject* pSender)
{
	if(JNIUtil::getInt("effectFlag",1)==1)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");
	}
	//�رշ��ؼ�����
	setKeypadEnabled(false);
	//���ò˵���ť�Ŀ�����
	pMenu->setEnabled(false);
	//������͸����
	DialogLayer *dialogLayer = DialogLayer::create();
	//���ð�͸�����λ��
	dialogLayer->setPosition(ccp(0,0));
	//��ӵ������
	this->addChild(dialogLayer,DASHBOARD_LEVEL_DQX);
}
CCPoint ControlLayer::getRandEnemyPosition()
{
	//��ȡ�з�̹���б�
	int tCount = gameLayer->enemyTankArray->count();
	if(tCount!=0)
	{
		//���������
		int index = rand()%tCount;
		EnemyTank* tankEnemy;
		//�����ż��
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

	//��ͣ���Ʋ�Ļص�����
	this->pauseSchedulerAndActions();
	//��ͣ��Ϸ����Ļص�����
	gameLayer->pauseSchedulerAndActions();
	//�رշ��ؼ�����
	setKeypadEnabled(false);
	//���ò˵���ť�Ŀ�����
	pMenu->setEnabled(false);
	//������͸����
	LoseLayer *loseLayer = LoseLayer::create();
	//���ð�͸�����λ��
	loseLayer->setPosition(ccp(0,0));
	//��ӵ������
	this->addChild(loseLayer,DASHBOARD_LEVEL_DQX);
}

