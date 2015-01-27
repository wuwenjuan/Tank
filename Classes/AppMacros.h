#ifndef __AppMacros_H__
#define __AppMacros_H__

#include "cocos2d.h"

//定义表示资源套装的结构体
typedef struct tagResource
{
    cocos2d::CCSize size;  //此资源包对应的分辨率（包括宽度、高度）
    char directory[100];   //此资源包子目录名
}Resource;
//定义表示声音资源套装的结构体
typedef struct SoundResourceStruct
{
    char directory[100];   //此资源包子目录名
}SoundResource;
//创建不同名称资源套装的结构体实例
static Resource picResource  =  { cocos2d::CCSizeMake(800, 480),"pic"};
static SoundResource soundResource  =  {"sound"};

//根据不同的目标分辨率设置Cocos2dx的工作尺寸
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(800, 480);
static float ENEMYMINPOSITION = -80;
//动态计算字体大小
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / picResource.size.width * 36)

//定义不同层次的Level
#define BACKGROUND_LEVEL_DQX 	0
#define MAKRS_LEVEL_DQX 		1
#define PROPS_LEVEL_DQX 		2
#define BOMB_LEVEL_DQX 			3
#define TANK_LEVEL_DQX 			4
#define BULLET_LEVEL_DQX 		5
#define TREES_LEVEL_DQX 		6
#define DASHBOARD_LEVEL_DQX 	7
//定义产生坦克的组
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
