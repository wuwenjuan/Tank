#include "TankSceneManager.h"
#include "AppMacros.h"
#include "TankGameLayer.h"
#include "ControlLayer.h"
#include "LoadLayer.h"
#include "MenuLayer.h"
#include "HelpLayer.h"
using namespace cocos2d;
//ʵ��TankSceneManager���е�createScene����
void TankSceneManager::createScene()
{
	//����CCScene������������
    loadScene = CCScene::create();
    //����LoadLayer��������
    LoadLayer *loadLayer = LoadLayer::create();
    //���ó���������
    loadLayer->tsm = this;
	//�򳡾�����Ӳ���
    loadScene->addChild(loadLayer);
}
void TankSceneManager::goGameLayer()
{
    //������������
	gameScene=CCScene::create();
	//������������
	TankGameLayer *tankGameLayer=TankGameLayer::create();
	//����һ�鱳��װ�ξ���
	tankGameLayer->addOneGroupTree();
	//�򳡾�����Ӳ���
	gameScene->addChild(tankGameLayer);
	//�������Ʋ�
    ControlLayer *controlLayer = ControlLayer::create();
    //���ÿ��Ʋ�Ŀ��Ʋ�
    controlLayer->gameLayer = tankGameLayer;
	//Ѹ�����һ������
    controlLayer->addEnemyTank(0);
    //�񳡾�����Ӳ���
    gameScene->addChild(controlLayer);
    //���ó���������
    controlLayer->tsm = this;
	//�����л���Ч
	CCTransitionScene* ccts=CCTransitionFade::create(0.8, gameScene, ccWHITE);
	//ִ���л�
	CCDirector::sharedDirector()->replaceScene(ccts);

}
void TankSceneManager::goMenuLayer()
{
    //������������
	menuScene=CCScene::create();
	//������������
	MenuLayer *menuLayer=MenuLayer::create();
	//�򳡾�����Ӳ���
	menuScene->addChild(menuLayer);
	//���ó���������
	menuLayer->tsm = this;
	//�����л���Ч
	CCTransitionScene* ccts=CCTransitionFade::create(1.5, menuScene, ccWHITE);
	//ִ���л�
	CCDirector::sharedDirector()->replaceScene(ccts);
}
void TankSceneManager::goHelpLayer()
{
    //������������
	helpScene=CCScene::create();
	//������������
	HelpLayer *helpLayer=HelpLayer::create();
	//�򳡾�����Ӳ���
	helpScene->addChild(helpLayer);
	//���ó���������
	helpLayer->tsm = this;
	//�����л���Ч
	CCTransitionScene* ccts=CCTransitionFade::create(1.5, helpScene, ccWHITE);
	//ִ���л�
	CCDirector::sharedDirector()->replaceScene(ccts);
}
