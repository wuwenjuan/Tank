#ifndef  _TankGameLayer_H_
#define  _TankGameLayer_H_
#include "SneakyJoystickSkinnedBase.h"
#include "cocos2d.h"
#include "string.h"
using namespace cocos2d;
//�Զ���Ĳ�����
class TankGameLayer : public CCLayer
{
public:
	//��ʼ���ķ���
	virtual bool init();
	//��ʼ�������ķ���
	void initBackGround();
	//��ʼ�����е�������֡����
	void initGunsAnimation();
	//��ʼ���ڵ�֡����������
	CCAnimation* initExplosionAnimation(std::string pname,int span);
	//��ʼ���ڹ�֡�����ķ���
	CCAnimation* initGunAnimation(std::string pname);
	//���������Ѫ���ķ���
	bool changeBlood(int blood);
	//���ӷ����ķ���
	void addScore(int scoreIncrement);
	//�˵��رջص�����
public:
	//���������Ļص�����
	void updateBackGround(float dt);
	//��ӱ������ķ���
	void addTrees(float dt);
	//����һ�鱳��װ�ξ���ķ���
	void addOneGroupTree();
public:
	//����װ��ͼƬ������
	std::string pBName[3];
	//����ͼƬ������
	std::string pPName[5];
	//�з�̹�����ϰ��������
	std::string enemyName[5];
	//����ͼƬ����
	std::string num[10];
	//��ȡ�ɼ�����ߴ�
	CCSize visibleSize;
	//��ȡ�ɼ�����ԭ������
	CCPoint origin;
	//��������
	CCSprite *backSprite1;
	//��������
	CCSprite *backSprite2;
	//̹�˾���
	CCSprite* tankBody;
	//�ڹܾ���
	CCSprite* tankGun;
	//��������ͼ��
	CCSprite* score_icon;
    //������֡��������
	CCAnimation** tankGunAnmi;
    //�ڵ���֡��������
	CCAnimation** tankBulletAnmi;
	//�ҷ��ӵ�
	CCSpriteBatchNode* tankBullet;
	//�ҷ�����
	CCSpriteBatchNode* tankBazooka;
	//�з��ӵ�
	CCSpriteBatchNode* enemyBullet;
	//�з�����
	CCSpriteBatchNode* enemyBazooka;
	//��ľ�б�
	CCNode* trees ;
	//ը���б�
	CCNode* marks ;
	//�����б�
	CCNode* nums;
	//Ѫ��
	CCNode* bloods;
	//�з�̹������
	CCArray *enemyTankArray;
	//�з��ϰ�������
	CCArray *barrierArray;
	//��������
	CCArray *propsArray;
	//����
	int scores;
	//�ؿ�
	int passTemp;
	//�ӵ�״̬
	int SHOOT_STATE;
	//����������
	int enemyGroupCount;
	//��ǰ������
	int currentEnemyGroup;
	//̹��Ѫ��
	int tankBlood;
	//��������Ӧ�ĳ�ʼ�����ͷŹ���
	CREATE_FUNC(TankGameLayer);
};

#endif
