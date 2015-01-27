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
//初始化布景
bool TankGameLayer::init()
{
	//调用父类的初始化
	if ( !CCLayer::init() )
	{
		return false;
	}
	//获取可见区域尺寸
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//获取可见区域原点坐标
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//初始化炮弹帧动画的数组
	//创建帧动画数组
	tankBulletAnmi = new CCAnimation*[2];
	tankBulletAnmi[0] = this->initExplosionAnimation("explosion1.png",64);
	tankBulletAnmi[1] = this->initExplosionAnimation("explosion2.png",32);
	//背景精灵名称数组
	pBName[0] = "mark1.png";
	pBName[1] = "tree2.png";
	pBName[2] = "tree1.png";
	//道具的名称
	pPName[0] = "propsH.png";
	pPName[1] = "propsH.png";

	pPName[2] = "propsP.png";
	pPName[3] = "propsP.png";

	pPName[4] = "propsC.png";

	//敌人精灵名称列表
	enemyName[0] = "enemyBullet.png";
	enemyName[1] = "enemyBullet.png";
	enemyName[2] = "enemyBazooka.png";
	enemyName[3] = "enemyBazooka.png";
	enemyName[4] = "enemyBazooka.png";
	//坦克血量
	tankBlood = 100;
	//分数
	scores = 0;
	//关卡
	passTemp = 1;
	//发射状态
	SHOOT_STATE = 3;
	//敌人组数量
	enemyGroupCount = 3*pow(1.2,passTemp);
	//当前敌人组数
	currentEnemyGroup = 0;
	//坦克血量
	int tankBlood;
	//创建我方坦克精灵
	tankBody = CCSprite::create("mainbody.png");
	//设置坦克精灵的位置
	tankBody->setPosition(ccp(200,200));
	//将坦克精灵添加到布景
	addChild(tankBody,TANK_LEVEL_DQX);
	//创建我方坦克炮管精灵
	tankGun = CCSprite::create("maingun.png",CCRectMake(0,0,33,68));
	//设置炮管精灵的锚点
	tankGun->setAnchorPoint(ccp(0.5,0.27));
	//设置炮管精灵的位置
	tankGun->setPosition(ccp(200,200));
	//将炮管精灵添加到布景
	addChild(tankGun,TANK_LEVEL_DQX);
	//我方子弹
	tankBullet =  CCSpriteBatchNode::create("tankBullet.png");
	//添加到布景
	this->addChild(tankBullet,BULLET_LEVEL_DQX);
	//我方导弹
	tankBazooka = CCSpriteBatchNode::create("tankBazooka.png");
	//添加到布景
	this->addChild(tankBazooka,BULLET_LEVEL_DQX);
	//敌方子弹
	enemyBullet =  CCSpriteBatchNode::create("enemyBullet.png");
	//添加到布景
	this->addChild(enemyBullet,BULLET_LEVEL_DQX);
	//敌方导弹
	enemyBazooka =  CCSpriteBatchNode::create("enemyBazooka.png");
	//添加到布景
	this->addChild(enemyBazooka,BULLET_LEVEL_DQX);
	//灌木图片列表
	trees = CCNode::create();
	//makes图片列表
	marks = CCNode::create();
	//创建数组图片
	nums = CCNode::create();
	//添加到layer层
	this->addChild(nums,DASHBOARD_LEVEL_DQX);
	//创建精灵
	CCSprite* scroe = CCSprite::create("nums.png",CCRectMake(0,0,30,50));
	//设置位置
	scroe->setPosition(ccp(450,450));
	this->nums->addChild(scroe,DASHBOARD_LEVEL_DQX);
	//创建分数图标
	score_icon = CCSprite::create("score_icon.png");
	score_icon->setPosition(ccp(400,origin.y+visibleSize.height-score_icon->getContentSize().height/2));
	this->addChild(score_icon,DASHBOARD_LEVEL_DQX);
	//创建血量精灵数组
	bloods = CCNode::create();
	//添加到layer层
	this->addChild(bloods,DASHBOARD_LEVEL_DQX);
	//初始化分数
	changeBlood(0);
	//添加到layer层
	this->addChild(marks,MAKRS_LEVEL_DQX);
	//添加到layer层
	this->addChild(trees,TREES_LEVEL_DQX);
    //创建敌方坦克数组
	enemyTankArray = CCArray::create();
	//引用计数加一
	enemyTankArray->retain();
	//创建障碍物数组
	barrierArray = CCArray::create();
	//引用计数加一
	barrierArray->retain();
	//创建道具数组
	propsArray = CCArray::create();
	//引用计算加一
	propsArray->retain();
	//初始化背景
	initBackGround();
	//初始化炮管精灵的发射帧
	initGunsAnimation();
	//更新背景的回调方法
	this->schedule(schedule_selector(TankGameLayer::updateBackGround),0.04);
	this->schedule(schedule_selector(TankGameLayer::addTrees),3);
	return true;
}
//初始化游戏界面背景
void TankGameLayer::initBackGround()
{
	//创建一个精灵对象，充当背景
	backSprite1 = CCSprite::create("gameBackground.png");
	//设置精灵对象的位置
	backSprite1->setPosition(ccp(origin.x, origin.y));
	//设置精灵锚点
	backSprite1->setAnchorPoint(ccp(0, 0));
	//将背景精灵添加到布景中
	this->addChild(backSprite1,BACKGROUND_LEVEL_DQX);
	//创建一个精灵对象，充当背景
	backSprite2 = CCSprite::create("gameBackground.png");
	//设置精灵对象的位置
	backSprite2->setPosition(ccp(origin.x, 475));
	//设置精灵锚点
	backSprite2->setAnchorPoint(ccp(0, 0));
	//将背景精灵添加到布景中
	this->addChild(backSprite2,BACKGROUND_LEVEL_DQX);
	//添加血量十字
	CCSprite* redBox = CCSprite::create("redbox.png");
	//设置精灵的位置
	redBox->setPosition
	(
		ccp
		(
			origin.x+redBox->getContentSize().width/2,//X坐标
			origin.y+visibleSize.height-redBox->getContentSize().height/2//Y坐标
		)
	);
	this->addChild(redBox,DASHBOARD_LEVEL_DQX);
}
//添加背景
void TankGameLayer::addTrees(float dt )
{
	//产生0-2的随机数
	int index = rand()%3;
	//根据随机数获取图片名称
	CCSprite* mTreeOrMarks = CCSprite::create(pBName[index].c_str());
	//随机产生x坐标
	float positionX = rand()%800;
	//随机产生y坐标
	float positionY = rand()%100+500;
	//计算时间
	float time = positionY/480*20;
	//设置树精灵的坐标
	mTreeOrMarks->setPosition(ccp(positionX,positionY));
	//树精灵执行东西
	mTreeOrMarks->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
	//设置随机的缩放比例
	if(index==0)
	{
		//随机产生图片的缩放比
		float scale = rand()%2+1.5;
		//设置图片的缩放比
		mTreeOrMarks->setScale(scale);
		//添加到根节点
		marks->addChild(mTreeOrMarks);
		return;
	}
	//添加到根节点
	trees->addChild(mTreeOrMarks);
}
//更新背景
void TankGameLayer::updateBackGround(float dt )
{
	//获取背景精灵的位置
	CCPoint bg1p = backSprite1->getPosition();
	//获取背景精灵的位置
	CCPoint bg2p = backSprite2->getPosition();
	//设置背景精灵的位置
	backSprite1->setPosition(ccp(bg1p.x,bg1p.y-2));
	//设置背景精灵的位置
	backSprite2->setPosition(ccp(bg2p.x,bg2p.y-2));
	//执行判断语句
	if (bg2p.y<0)
	{
		//设置背景精灵的位置
		backSprite1->setPosition(ccp(bg2p.x,bg2p.y+475));
	}
	//执行判断语句
	if (bg1p.y<0)
	{
		//设置背景精灵的位置
		backSprite2->setPosition(ccp(bg1p.x,bg1p.y+475));
	}
	//获取灌木精灵列表
	CCArray* treesArray = trees->getChildren();
	//获取炸痕精灵列表
	CCArray* marksArray = marks->getChildren();
	//如果获取到的列表不为空
	if(marksArray)
	{
		//执行for循环
		for(int i = 0;i<marksArray->count();i++)
			{
				//获取炸痕精灵
				CCSprite* mMark =  (CCSprite*)marksArray->objectAtIndex(i);
				//执行判断挑挑
				if(mMark->getPosition().y<ENEMYMINPOSITION+5)
				{
					//从根节点移除炸痕精灵
					marks->removeChild(mMark);
				}
			}
	}
	//如果获取到的列表不为空
	if(treesArray)
	{
		//执行for循环
		for(int i = 0;i<treesArray->count();i++)
		{
			//获取灌木精灵列表
			CCSprite* mTree =  (CCSprite*)treesArray->objectAtIndex(i);
			//执行判断条件
			if(mTree->getPosition().y<-100)
			{
				//从根节点移除灌木精灵
				trees->removeChild(mTree);
			}
		}
	}
}
//初始化炮弹帧动画
CCAnimation* TankGameLayer::initExplosionAnimation(std::string pname,int span)
{
	//加载爆炸总图
	CCTexture2D *explosionTexture=CCTextureCache::sharedTextureCache()->addImage(pname.c_str());
	//创建帧动画的Array列表
	CCArray* frames=CCArray::createWithCapacity(25);
	//获取次动画的帧
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			CCSpriteFrame *fb=CCSpriteFrame::createWithTexture(explosionTexture,CCRectMake(span*j,span*i,span,span));
			frames->addObject(fb);
		}
	}

    //创建动画对象
	CCAnimation *anmi=new CCAnimation();
	//从帧数组产生动画，间隔为0.1秒
	anmi->initWithSpriteFrames(frames,0.06*span/64);
	//释放帧数组
	frames->release();
	return anmi;
}
//初始化所有炮塔的帧动画
void TankGameLayer::initGunsAnimation()
{
	//创建帧动画数组
	tankGunAnmi = new CCAnimation*[5];
	//将帧动画添加到帧动画数组
	tankGunAnmi[0] = initGunAnimation("maingun.png");
	//将帧动画添加到帧动画数组
	tankGunAnmi[1] = initGunAnimation("enemygun1.png");
	//将帧动画添加到帧动画数组
	tankGunAnmi[2] = initGunAnimation("enemygun2.png");
	//将帧动画添加到帧动画数组
	tankGunAnmi[3] = initGunAnimation("enemygun3.png");
	//将帧动画添加到帧动画数组
	tankGunAnmi[4] = initGunAnimation("enemygun4.png");

}
//初始化炮管的动画
CCAnimation* TankGameLayer::initGunAnimation(std::string pname)
{
	//加载炮管总图
	CCTexture2D *tankTexture=CCTextureCache::sharedTextureCache()->addImage(pname.c_str());
	//获取此动画中的指定两个帧
	CCSpriteFrame *f0=CCSpriteFrame::createWithTexture(tankTexture,CCRectMake(0,0,33,68));
	CCSpriteFrame *f1=CCSpriteFrame::createWithTexture(tankTexture,CCRectMake(33,0,33,68));
	//创建帧数组
	CCArray* frames=CCArray::createWithCapacity(2);
	//将动画中的两个帧添加进帧数组
	frames->addObject(f1);
	frames->addObject(f0);
	//创建动画对象
	CCAnimation *anmi=new CCAnimation();
	//从帧数组产生动画，间隔为0.1秒
	anmi->initWithSpriteFrames(frames,0.07f);
	//释放帧数组
	frames->release();
	return anmi;
}
//增加与减少血量的方法
bool TankGameLayer::changeBlood(int blood)
{
	//计算坦克血量
	tankBlood =((tankBlood+blood)>100)?100:(tankBlood+blood);
	//如果血量小于0则表示游戏失败
	if(tankBlood<=0)
	{
		return true;
	}
	//清空数子列表的精灵
	bloods->removeAllChildren();
	int tankBloodTemp = tankBlood;
	//位置参数
	int i = 0;
	while(tankBloodTemp!=0)
	{
		//计算每一位的数字
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
		//计算完之后执行处罚
		tankBloodTemp = tankBloodTemp/10;
		i++;
	}
}
//增加与减少血量的方法
void TankGameLayer::addScore(int scoreIncrement)
{
	//清空数子列表的精灵
	nums->removeAllChildren();
	//计算得分
	scores+=scoreIncrement;
	int scroesTemp = scores;
	//位置参数
	int i = 0;
	while(scroesTemp!=0)
	{
		//计算每一位的数字
		int number = scroesTemp%10;
		CCSprite* scroe = CCSprite::create("nums.png",CCRectMake(number*30,0,30,50));
		scroe->setPosition(ccp(450-30*i,450));
		this->nums->addChild(scroe,DASHBOARD_LEVEL_DQX);
		//计算完之后执行处罚
		scroesTemp = scroesTemp/10;
		i++;
	}
	score_icon->setPosition(ccp(440-30*i,origin.y+visibleSize.height-score_icon->getContentSize().height/2));
}
//增加与减少血量的方法
void TankGameLayer::addOneGroupTree()
{
	//在初始化背景的时候添加几个背景精灵装饰一下
		//随机产生x坐标
		float positionX = rand()%800;
		//随机产生y坐标
		float positionY = rand()%280+100;
		//创建精灵
		CCSprite* back1 = CCSprite::create("mark1.png");
		//设置精灵的位置
		back1->setPosition
		(
			ccp
			(
				origin.x+positionX,//X坐标
				origin.y+positionY//Y坐标
			)
		);
		//设置精灵的缩放比
		back1->setScale(2);
		//精灵执行动作
		back1->runAction(CCMoveTo::create(20,ccp(positionX,positionY-700)));
		//添加到根节点
		marks->addChild(back1);
    
    
    
		//随机产生x坐标
		positionX = rand()%800;
		//随机产生y坐标
		positionY = rand()%280+100;
		//创建精灵
		CCSprite* back2 = CCSprite::create("mark1.png");
		//设置精灵对象
		back2->setPosition
		(
			ccp
			(
				origin.x+positionX,//X坐标
				origin.y+positionY//Y坐标
			)
		);
		//设置精灵的缩放比
		back2->setScale(2);
		//计算时间
		float time = positionY/480*20;
		//精灵执行动作
		back2->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
		//添加到根节点
		marks->addChild(back2);
    
    
		//随机产生x坐标
		positionX = rand()%800;
		//随机产生y坐标
		positionY = rand()%280+100;
		//创建精灵
		CCSprite* back3 = CCSprite::create("tree1.png");
		//设置精灵的位置
		back3->setPosition
		(
			ccp
			(
				origin.x+positionX,//X坐标
				origin.y+positionY//Y坐标
			)
		);
		time = positionY/480*20;
		//精灵执行动作
		back3->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
		//添加到根节点
		trees->addChild(back3);
    
    
		//随机产生x坐标
		positionX = rand()%800;
		//随机产生y坐标
		positionY = rand()%280+100;
		//创建精灵
		CCSprite* back4 = CCSprite::create("tree2.png");
		//计算时间
		time = positionY/480*20;
		//设置精灵
		back4->setPosition
		(
			ccp
			(
				origin.x+positionX,//X坐标
				origin.y+positionY//Y坐标
			)
		);
		//精灵执行动作
		back4->runAction(CCMoveTo::create(time,ccp(positionX,ENEMYMINPOSITION)));
		//添加到根节点
		trees->addChild(back4,TREES_LEVEL_DQX);
}
