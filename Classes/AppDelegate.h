#ifndef  _AppDelegate_H_
#define  _AppDelegate_H_

#include "cocos2d.h"

//�Լ�д��cocos2dxӦ�ó���Ҫ�̳�CCApplication
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    //��ʼ����Ϸ�е�CCDirector�Լ�CCScene
    virtual bool applicationDidFinishLaunching();
    //����������̨ʱ���ô˷���
    virtual void applicationDidEnterBackground();
    //���������ǰ̨ʱ���ô˷���
    virtual void applicationWillEnterForeground();
    //���ر������ֵķ���
    void initMusic();
};

#endif
