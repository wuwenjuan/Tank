#ifndef __AppMacros_H__
#define __AppMacros_H__

#include "cocos2d.h"

//�����ʾ��Դ��װ�Ľṹ��
typedef struct tagResource
{
    cocos2d::CCSize size;  //����Դ����Ӧ�ķֱ��ʣ�������ȡ��߶ȣ�
    char directory[100];   //����Դ����Ŀ¼��
}Resource;
//�����ʾ������Դ��װ�Ľṹ��
typedef struct SoundResourceStruct
{
    char directory[100];   //����Դ����Ŀ¼��
}SoundResource;
//������ͬ������Դ��װ�Ľṹ��ʵ��
static Resource picResource  =  { cocos2d::CCSizeMake(800, 480),"pic"};
static SoundResource soundResource  =  {"sound"};

//���ݲ�ͬ��Ŀ��ֱ�������Cocos2dx�Ĺ����ߴ�
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(800, 480);
static float ENEMYMINPOSITION = -80;
//��̬���������С
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / picResource.size.width * 36)

//���岻ͬ��ε�Level
#define BACKGROUND_LEVEL_DQX 	0
#define MAKRS_LEVEL_DQX 		1
#define PROPS_LEVEL_DQX 		2
#define BOMB_LEVEL_DQX 			3
#define TANK_LEVEL_DQX 			4
#define BULLET_LEVEL_DQX 		5
#define TREES_LEVEL_DQX 		6
#define DASHBOARD_LEVEL_DQX 	7
//�������̹�˵���
#define ENEMY_TANK_GROUP_ONE 	1
#define ENEMY_TANK_GROUP_TWO 	2
#define ENEMY_TANK_GROUP_THREE 	3
#define ENEMY_TANK_GROUP_FORE 	4
#define ENEMY_TANK_GROUP_FIVE 	5
#define ENEMY_TANK_GROUP_SIX 	6
#define ENEMY_TANK_GROUP_SEVEN 	7
#define ENEMY_TANK_GROUP_EIGHT 	8
#define ENEMY_TANK_GROUP_NINE 	9
#define ENEMY_TANK_GROUP_TEN 	10
#endif
