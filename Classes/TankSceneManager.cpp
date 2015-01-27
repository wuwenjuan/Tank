#include "TankSceneManager.h"
#include "AppMacros.h"
#include "TankGameLayer.h"
#include "ControlLayer.h"
#include "LoadLayer.h"
#include "MenuLayer.h"
#include "HelpLayer.h"
using namespace cocos2d;
//实现TankSceneManager类中的createScene方法
void TankSceneManager::createScene()
{
	//创建CCScene（场景）对象
    loadScene = CCScene::create();
    //创建LoadLayer布景对象
    LoadLayer *loadLayer = LoadLayer::create();
    //设置场景管理者
    loadLayer->tsm = this;
	//向场景中添加布景
    loadScene->addChild(loadLayer);
}
void TankSceneManager::goGameLayer()
{
    //创建场景对象
	gameScene=CCScene::create();
	//创建布景对象
	TankGameLayer *tankGameLayer=TankGameLayer::create();
	//产生一组背景装饰精灵
	tankGameLayer->addOneGroupTree();
	//向场景中添加布景
	gameScene->addChild(tankGameLayer);
	//创建控制层
    ControlLayer *controlLayer = ControlLayer::create();
    //设置控制层的控制层
    controlLayer->gameLayer = tankGameLayer;
	//迅速添加一波敌人
    controlLayer->addEnemyTank(0);
    //像场景中添加布景
    gameScene->addChild(controlLayer);
    //设置场景管理者
    controlLayer->tsm = this;
	//创建切换特效
	CCTransitionScene* ccts=CCTransitionFade::create(0.8, gameScene, ccWHITE);
	//执行切换
	CCDirector::sharedDirector()->replaceScene(ccts);

}
void TankSceneManager::goMenuLayer()
{
    //创建场景对象
	menuScene=CCScene::create();
	//创建布景对象
	MenuLayer *menuLayer=MenuLayer::create();
	//向场景中添加布景
	menuScene->addChild(menuLayer);
	//设置场景管理者
	menuLayer->tsm = this;
	//创建切换特效
	CCTransitionScene* ccts=CCTransitionFade::create(1.5, menuScene, ccWHITE);
	//执行切换
	CCDirector::sharedDirector()->replaceScene(ccts);
}
void TankSceneManager::goHelpLayer()
{
    //创建场景对象
	helpScene=CCScene::create();
	//创建布景对象
	HelpLayer *helpLayer=HelpLayer::create();
	//向场景中添加布景
	helpScene->addChild(helpLayer);
	//设置场景管理者
	helpLayer->tsm = this;
	//创建切换特效
	CCTransitionScene* ccts=CCTransitionFade::create(1.5, helpScene, ccWHITE);
	//执行切换
	CCDirector::sharedDirector()->replaceScene(ccts);
}
