#include "TankGameLayer.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include <android/log.h>
#include <string.h>
#include <math.h>
#define CCCA(x) (x->copy()->autorelease())
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
using namespace cocos2d;
//��ʼ������
bool TankGameLayer::init()
{
	//���ø���ĳ�ʼ��
	if ( !CCLayer::init() )
	{
		return false;
	}
	//��ȡ�ɼ�����ߴ�
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//��ȡ�ɼ�����ԭ������
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//��ʼ���ڵ�֡����������
	//����֡��������
	tankBulletAnmi = new CCAnimation*[2];
	tankBulletAnmi[0] = this->initExplosionAnimation("explosion1.png",64);
	tankBulletAnmi[1] = this->initExplosionAnimation("explosion2.png",32);
	//����������������
	pBName[0] = "mark1.png";
	pBName[1] = "tree2.png";
	pBName[2] = "tree1.png";
	//���ߵ�����
	pPName[0] = "propsH.png";
	pPName[1] = "propsH.png";

	pPName[2] = "propsP.png";
	pPName[3] = "propsP.png";

	pPName[4] = "propsC.png";

	//���˾��������б�
	enemyName[0] = "enemyBullet.png";
	enemyName[1] = "enemyBullet.png";
	enemyName[2] = "enemyBazooka.png";
	enemyName[3] = "enemyBazooka.png";
	enemyName[4] = "enemyBazooka.png";
	//̹��Ѫ��
	tankBlood = 100;
	//����
	scores = 0;
	//�ؿ�
	passTemp = 1;
	//����״̬
	SHOOT_STATE = 3;
	//����������
	enemyGroupCount = 3*pow(1.2,passTemp);
	//��ǰ��������
	currentEnemyGroup = 0;
	//̹��Ѫ��
	int tankBlood;
	//�����ҷ�̹�˾���
	tankBody = CCSprite::create("mainbody.png");
	//����̹�˾����λ��
	tankBody->setPosition(ccp(200,200));
	//��̹�˾�����ӵ�����
	addChild(tankBody,TANK_LEVEL_DQX);
	//�����ҷ�̹���ڹܾ���
	tankGun = CCSprite::create("maingun.png",CCRectMake(0,0,33,68));
	//�����ڹܾ����ê��
	tankGun->setAnchorPoint(ccp(0.5,0.27));
	//�����ڹܾ����λ��
	tankGun->setPosition(ccp(200,200));
	//���ڹܾ�����ӵ�����
	addChild(tankGun,TANK_LEVEL_DQX);
	//�ҷ��ӵ�
	tankBullet =  CCSpriteBatchNode::create("tankBullet.png");
	//��ӵ�����
	this->addChild(tankBullet,BULLET_LEVEL_DQX);
	//�ҷ�����
	tankBazooka = CCSpriteBatchNode::create("tankBazooka.png");
	//��ӵ�����
	this->addChild(tankBazooka,BULLET_LEVEL_DQX);
	//�з��ӵ�
	enemyBullet =  CCSpriteBatchNode::create("enemyBullet.png");
	//��ӵ�����
	this->addChild(enemyBullet,BULLET_LEVEL_DQX);
	//�з�����
	enemyBazooka =  CCSpriteBatchNode::create("enemyBazooka.png");
	//��ӵ�����
	this->addChild(enemyBazooka,BULLET_LEVEL_DQX);
	//��ľͼƬ�б�
	trees = CCNode::create();
	//makesͼƬ�б�
	marks = CCNode::create();
	//��������ͼƬ
	nums = CCNode::create();
	//��ӵ�layer��
	this->addChild(nums,DASHBOARD_LEVEL_DQX);
	//��������
	CCSprite* scroe = CCSprite::create("nums.png",CCRectMake(0,0,30,50));
	//����λ��
	scroe->setPosition(ccp(450,450));
	this->nums->addChild(scroe,DASHBOARD_LEVEL_DQX);
	//��������ͼ��
	score_icon = CCSprite::create("score_icon.png");
	score_icon->setPosition(ccp(400,origin.y+visibleSize.height-score_icon->getContentSize().height/2));
	this->addChild(score_icon,DASHBOARD_LEVEL_DQX);
	//����Ѫ����������
	bloods = CCNode::create();
	//��ӵ�layer��
	this->addChild(bloods,DASHBOARD_LEVEL_DQX);
	//��ʼ������
	changeBlood(0);
	//��ӵ�layer��
	this->addChild(marks,MAKRS_LEVEL_DQX);
	//��ӵ�layer��
	this->addChild(trees,TREES_LEVEL_DQX);
    //�����з�̹������
	enemyTankArray = CCArray::create();
	//���ü�����һ
	enemyTankArray->retain();
	//�����ϰ�������
	barrierArray = CCArray::create();
	//���ü�����һ
	barrierArray->retain();
	//������������
	propsArray = CCArray::create();
	//���ü����һ
	propsArray->retain();
	//��ʼ������
	initBackGround();
	//��ʼ���ڹܾ���ķ���֡
	initGunsAnimation();
	//���±����Ļص�����
	this->schedule(schedule_selector(TankGameLayer::updateBackGround),0.04);
	this->schedule(schedule_selector(TankGameLayer::addTrees),3);
	return true;
}
//��ʼ����Ϸ���汳��
void TankGameLayer::initBackGround()
{
	//����һ��������󣬳䵱����
	backSprite1 = CCSprite::create("gameBackground.png");
	//���þ�������λ��
	backSprite1->setPosition(ccp(origin.x, origin.y));
	//���þ���ê��
	backSprite1->setAnchorPoint(ccp(0, 0));
	//������������ӵ�������
	this->addChild(backSprite1,BACKGROUND_LEVEL_DQX);
	//����һ��������󣬳䵱����
	backSprite2 = CCSprite::create("gameBackground.png");
	//���þ�������λ��
	backSprite2->setPosition(ccp(origin.x, 475));
	//���þ���ê��
	backSprite2->setAnchorPoint(ccp(0, 0));
	//������������ӵ�������
	this->addChild(backSprite2,BACKGROUND_LEVEL_DQX);
	//���Ѫ��ʮ��
	CCSprite* redBox = CCSprite::create("redbox.png");
	//���þ����λ��
	redBox->setPosition
	(
		ccp
		(
			origin.x+redBox->getContentSize().width/2,//X����
			origin.y+visibleSize.height-redBox->getContentSize().height/2//Y����
		)
	);
	this->addChild(redBox,DASHBOARD_LEVEL_DQX);
}
//��ӱ���
void TankGameLayer::addTrees(float dt )
{
	//����0-2�������
	int index = rand()%3;
	//�����������ȡͼƬ����
	CCSprite* mTreeOrMarks = CCSprite::create(pBName[index].c_str());
	//�������x����
	float positionX = rand()%800;
	//�������y����
	float positionY = rand()%100+500;
	//����ʱ��
	float time = positionY/480*20;
	//���������������
	mTreeOrMarks->setPosition(ccp(positionX,positionY));
	//������ִ�ж���
	mTreeOrMarks->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
	//������������ű���
	if(index==0)
	{
		//�������ͼƬ�����ű�
		float scale = rand()%2+1.5;
		//����ͼƬ�����ű�
		mTreeOrMarks->setScale(scale);
		//��ӵ����ڵ�
		marks->addChild(mTreeOrMarks);
		return;
	}
	//��ӵ����ڵ�
	trees->addChild(mTreeOrMarks);
}
//���±���
void TankGameLayer::updateBackGround(float dt )
{
	//��ȡ���������λ��
	CCPoint bg1p = backSprite1->getPosition();
	//��ȡ���������λ��
	CCPoint bg2p = backSprite2->getPosition();
	//���ñ��������λ��
	backSprite1->setPosition(ccp(bg1p.x,bg1p.y-2));
	//���ñ��������λ��
	backSprite2->setPosition(ccp(bg2p.x,bg2p.y-2));
	//ִ���ж����
	if (bg2p.y<0)
	{
		//���ñ��������λ��
		backSprite1->setPosition(ccp(bg2p.x,bg2p.y+475));
	}
	//ִ���ж����
	if (bg1p.y<0)
	{
		//���ñ��������λ��
		backSprite2->setPosition(ccp(bg1p.x,bg1p.y+475));
	}
	//��ȡ��ľ�����б�
	CCArray* treesArray = trees->getChildren();
	//��ȡը�۾����б�
	CCArray* marksArray = marks->getChildren();
	//�����ȡ�����б�Ϊ��
	if(marksArray)
	{
		//ִ��forѭ��
		for(int i = 0;i<marksArray->count();i++)
			{
				//��ȡը�۾���
				CCSprite* mMark =  (CCSprite*)marksArray->objectAtIndex(i);
				//ִ���ж�����
				if(mMark->getPosition().y<ENEMYMINPOSITION+5)
				{
					//�Ӹ��ڵ��Ƴ�ը�۾���
					marks->removeChild(mMark);
				}
			}
	}
	//�����ȡ�����б�Ϊ��
	if(treesArray)
	{
		//ִ��forѭ��
		for(int i = 0;i<treesArray->count();i++)
		{
			//��ȡ��ľ�����б�
			CCSprite* mTree =  (CCSprite*)treesArray->objectAtIndex(i);
			//ִ���ж�����
			if(mTree->getPosition().y<-100)
			{
				//�Ӹ��ڵ��Ƴ���ľ����
				trees->removeChild(mTree);
			}
		}
	}
}
//��ʼ���ڵ�֡����
CCAnimation* TankGameLayer::initExplosionAnimation(std::string pname,int span)
{
	//���ر�ը��ͼ
	CCTexture2D *explosionTexture=CCTextureCache::sharedTextureCache()->addImage(pname.c_str());
	//����֡������Array�б�
	CCArray* frames=CCArray::createWithCapacity(25);
	//��ȡ�ζ�����֡
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			CCSpriteFrame *fb=CCSpriteFrame::createWithTexture(explosionTexture,CCRectMake(span*j,span*i,span,span));
			frames->addObject(fb);
		}
	}

    //������������
	CCAnimation *anmi=new CCAnimation();
	//��֡����������������Ϊ0.1��
	anmi->initWithSpriteFrames(frames,0.06*span/64);
	//�ͷ�֡����
	frames->release();
	return anmi;
}
//��ʼ������������֡����
void TankGameLayer::initGunsAnimation()
{
	//����֡��������
	tankGunAnmi = new CCAnimation*[5];
	//��֡������ӵ�֡��������
	tankGunAnmi[0] = initGunAnimation("maingun.png");
	//��֡������ӵ�֡��������
	tankGunAnmi[1] = initGunAnimation("enemygun1.png");
	//��֡������ӵ�֡��������
	tankGunAnmi[2] = initGunAnimation("enemygun2.png");
	//��֡������ӵ�֡��������
	tankGunAnmi[3] = initGunAnimation("enemygun3.png");
	//��֡������ӵ�֡��������
	tankGunAnmi[4] = initGunAnimation("enemygun4.png");

}
//��ʼ���ڹܵĶ���
CCAnimation* TankGameLayer::initGunAnimation(std::string pname)
{
	//�����ڹ���ͼ
	CCTexture2D *tankTexture=CCTextureCache::sharedTextureCache()->addImage(pname.c_str());
	//��ȡ�˶����е�ָ������֡
	CCSpriteFrame *f0=CCSpriteFrame::createWithTexture(tankTexture,CCRectMake(0,0,33,68));
	CCSpriteFrame *f1=CCSpriteFrame::createWithTexture(tankTexture,CCRectMake(33,0,33,68));
	//����֡����
	CCArray* frames=CCArray::createWithCapacity(2);
	//�������е�����֡��ӽ�֡����
	frames->addObject(f1);
	frames->addObject(f0);
	//������������
	CCAnimation *anmi=new CCAnimation();
	//��֡����������������Ϊ0.1��
	anmi->initWithSpriteFrames(frames,0.07f);
	//�ͷ�֡����
	frames->release();
	return anmi;
}
//���������Ѫ���ķ���
bool TankGameLayer::changeBlood(int blood)
{
	//����̹��Ѫ��
	tankBlood =((tankBlood+blood)>100)?100:(tankBlood+blood);
	//���Ѫ��С��0���ʾ��Ϸʧ��
	if(tankBlood<=0)
	{
		return true;
	}
	//��������б�ľ���
	bloods->removeAllChildren();
	int tankBloodTemp = tankBlood;
	//λ�ò���
	int i = 0;
	while(tankBloodTemp!=0)
	{
		//����ÿһλ������
		int number = tankBloodTemp%10;
		CCSprite* blood = CCSprite::create("nums.png",CCRectMake(number*30,0,30,50));
		if(tankBlood>10)
		{
			blood->setPosition(ccp(60+30*(2-i),450));
		}
		else
		{
			blood->setPosition(ccp(60+30*(1-i),450));
		}

		this->bloods->addChild(blood,DASHBOARD_LEVEL_DQX);
		//������֮��ִ�д���
		tankBloodTemp = tankBloodTemp/10;
		i++;
	}
}
//���������Ѫ���ķ���
void TankGameLayer::addScore(int scoreIncrement)
{
	//��������б�ľ���
	nums->removeAllChildren();
	//����÷�
	scores+=scoreIncrement;
	int scroesTemp = scores;
	//λ�ò���
	int i = 0;
	while(scroesTemp!=0)
	{
		//����ÿһλ������
		int number = scroesTemp%10;
		CCSprite* scroe = CCSprite::create("nums.png",CCRectMake(number*30,0,30,50));
		scroe->setPosition(ccp(450-30*i,450));
		this->nums->addChild(scroe,DASHBOARD_LEVEL_DQX);
		//������֮��ִ�д���
		scroesTemp = scroesTemp/10;
		i++;
	}
	score_icon->setPosition(ccp(440-30*i,origin.y+visibleSize.height-score_icon->getContentSize().height/2));
}
//���������Ѫ���ķ���
void TankGameLayer::addOneGroupTree()
{
	//�ڳ�ʼ��������ʱ����Ӽ�����������װ��һ��
		//�������x����
		float positionX = rand()%800;
		//�������y����
		float positionY = rand()%280+100;
		//��������
		CCSprite* back1 = CCSprite::create("mark1.png");
		//���þ����λ��
		back1->setPosition
		(
			ccp
			(
				origin.x+positionX,//X����
				origin.y+positionY//Y����
			)
		);
		//���þ�������ű�
		back1->setScale(2);
		//����ִ�ж���
		back1->runAction(CCMoveTo::create(20,ccp(positionX,positionY-700)));
		//��ӵ����ڵ�
		marks->addChild(back1);
    
    
    
		//�������x����
		positionX = rand()%800;
		//�������y����
		positionY = rand()%280+100;
		//��������
		CCSprite* back2 = CCSprite::create("mark1.png");
		//���þ������
		back2->setPosition
		(
			ccp
			(
				origin.x+positionX,//X����
				origin.y+positionY//Y����
			)
		);
		//���þ�������ű�
		back2->setScale(2);
		//����ʱ��
		float time = positionY/480*20;
		//����ִ�ж���
		back2->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
		//��ӵ����ڵ�
		marks->addChild(back2);
    
    
		//�������x����
		positionX = rand()%800;
		//�������y����
		positionY = rand()%280+100;
		//��������
		CCSprite* back3 = CCSprite::create("tree1.png");
		//���þ����λ��
		back3->setPosition
		(
			ccp
			(
				origin.x+positionX,//X����
				origin.y+positionY//Y����
			)
		);
		time = positionY/480*20;
		//����ִ�ж���
		back3->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
		//��ӵ����ڵ�
		trees->addChild(back3);
    
    
		//�������x����
		positionX = rand()%800;
		//�������y����
		positionY = rand()%280+100;
		//��������
		CCSprite* back4 = CCSprite::create("tree2.png");
		//����ʱ��
		time = positionY/480*20;
		//���þ���
		back4->setPosition
		(
			ccp
			(
				origin.x+positionX,//X����
				origin.y+positionY//Y����
			)
		);
		//����ִ�ж���
		back4->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
		//��ӵ����ڵ�
		trees->addChild(back4,TREES_LEVEL_DQX);
}
