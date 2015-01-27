#ifndef  _ControlLayer_H_
#define  _ControlLayer_H_
#include "SneakyJoystickSkinnedBase.h"
#include "TankSceneManager.h"
#include "TankGameLayer.h"
#include "cocos2d.h"
using namespace cocos2d;
//�Զ���Ĳ�����
class ControlLayer : public CCLayer
{
public:
	//��ʼ���ķ���
    virtual bool init();
    //���ؼ�����
	//��ʼ������̹�˵�ң�еķ���
	void initTankJoyStick();
	//��ʼ������������ң�еķ���
	void initGunJoyStick();
	//��ʼ�������еİ�ť�ķ���
	void initButton();
	//�Ƴ��ҷ�̹���ӵ��ķ���
	void removeTankBullet(CCNode* node,void* param);
	//��ӵ����ķ���
	void removeTankBazooka(CCNode* node);
	//�Ƴ��з�̹���ӵ��ķ���
	void removeEnemyTankBullet(CCNode* node,void* param);
	//�Ƴ��з�̹���ڵ��ķ���
	void removeEnemyTankBazooka(CCNode* node,void* param);
	//�Ƴ��з�̹�˵ķ���
	void removeEnemyTank(CCNode* node,void* param);
	//�Ƴ���ը����ķ���
	void removeBomb(CCNode* node,void* param);
	//�Ƴ����߾���
	void removeProps(CCNode* node,void* param);
	//�Ƴ��з�̹�������ķ���
	void removeEnemyTankGun(CCNode* node,void* param);
	//�Ƴ��ϰ���ķ���
	void removeBarrier(CCNode* node,void* param);
	//����������ת��־λ�ķ���
	void setGunMoveFlag(CCObject* pSender);
	//�ҷ�̹����з�̹����ײ�ķ���
	void tank_enemyTank();
	//�ҷ�̹�����ϰ�����ײ�ķ���
	void tank_barrier(CCArray* barrierArray);
	//�ҷ�̹����Ե�����ִ�еķ���
	void tank_props(CCArray* propsArray);
	//�ҷ��ӵ����ϰ�����ײ�ķ���
	void tankBullet_enemy(CCSpriteBatchNode* bulletNode,int bolldReduce);
	//�ҷ��ӵ���з�̹����ײ�ķ���
	void tankBullet_enemyTank(CCSpriteBatchNode* bulletNode,int bolldReduce);
	//�з��ӵ����ҷ�̹����ײ�ķ���
	void enemyTankBullet_tank(CCSpriteBatchNode* enemyBulletNode,int bloodReduce);
	//�˵��رջص�����
	void menuPauseCallback(CCObject* pSender);
	//�ɹ�����֮��Ĺ���
	void reparation_for_next_level();
	//��һ�صķ���
	void nextLevel();
	//��ת��ʧ�ܽ���
	void loseView();

public:
	//��ӵз�̹�˵ķ���
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
	//���һ���з�̹�˻��ϰ���ķ����ķ���
	void addEnemyTank_ONE(CCPoint position);
	void addEnemyTank_TWO(CCPoint position);
	void addEnemyTank_THREE(CCPoint position);
	void addEnemyTank_FORE(CCPoint position);
	void addBarrier(CCPoint position);
	void addTower(CCPoint position);

public:
	//����̹��������λ�õķ���
	void updateTank();
	//���������Ƕȵķ���
	void updateGun(float dt);
	//���������Ƕȵķ���
	void updateEnemyGun(float dt);
	//̹�˷����ڵ��ķ���
	void tankShoot(float dt);
	//��ӵ����ķ���
	void addTankBullet();
	//��ӵ����ķ���
	void addtankBazooka(float dt);
	//���㵼������ת�Ƕ�
	void calculateBazookaAngle();
	//��ӵ��ߵķ���
	void addProps(CCPoint position);
	//��ӱ�ը����ķ���
	void addBomb(CCPoint position,float time,int animIndex);
	CCPoint getRandEnemyPosition();
	//���ը�۾���ķ���
	void addMarks(CCPoint position,float time);
	//��ʱ��ӵз�̹�˵ķ���
	void addEnemyTank(float dt);
	//�з�̹�˶�ʱ�����ӵ��ķ���
	void enemyTankShoot(float dt);
	//��ײ���ķ���
	void collisionDetection(float dt);
	//��ӵз�̹���ӵ��ķ���
	void addEnemyTankBullet(CCNode* node,void* param);
	//����̹�˵���λ��
	CCPoint calculationPosition(CCPoint joyStickV);
public:
	//��Ѫ
	void eatProp_ONE();
	//���ӵ�
	void eatProp_TWO(int state_increment);
	//����
	void eatProp_THREE();


public:
	//����������
    TankSceneManager *tsm;
	//̹��ҡ��
	SneakyJoystick *tankJoyStick;
	//̹��ҡ�˵���
	SneakyJoystickSkinnedBase *tankJoyStickBase;
	//����ҡ�˵���
	SneakyJoystickSkinnedBase *gunJoyStickBase;
	//����ҡ��
	SneakyJoystick *gunJoyStick;
	//��Ϸ����
	TankGameLayer *gameLayer;
	//��ȡ�ɼ�����ߴ�
	CCSize visibleSize;
	//��ȡ�ɼ�����ԭ������
	CCPoint origin;
	//�˵���ť
	CCMenu* pMenu;
	//̹���ƶ��ı�־λ
	bool tankMoveFlag;
	//������ת�ı�־λ
	bool gunMoveFlag;
	//��������֡�ı�־λ
	bool gunIsAnimation;
	//���Ƶ�������ʱ��ı�־λ
	bool enemyBazookaFlag;
	//��������ת�Ƕ�
	float gunAngle;
	//̹�˵���ת�Ƕ�
	float tankAngle;
	//��������Ӧ�ĳ�ʼ�����ͷŹ���
	CREATE_FUNC(ControlLayer);
};

#endif
