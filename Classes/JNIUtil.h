#ifndef __JNIUtil_H__
#define __JNIUtil_H__

#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;

/*
 * ��Cocos2dx��Ŀ����Activity��������Ա����
 * ---------------------------------------------------------------------------
 * public static SharedPreferences sharedPreferences;
 * public static SharedPreferences.Editor editor;
 * ---------------------------------------------------------------------------
 * ��--protected void onCreate(Bundle savedInstanceState){}--���������ʼ����Ա����
 * ---------------------------------------------------------------------------
 * sharedPreferences = this.getSharedPreferences("cn",Context.MODE_PRIVATE);
 * editor = sharedPreferences.edit();
 * ---------------------------------------------------------------------------
 * ����
 */
//�������ݵ�JNIUtil��com/bn/cocos2d/tank/TankActivity
//--classPathΪjava��������Щ���������·�� ����---- "com/bn/cocos2d/Cocos2dxJniTest/HelloActivity"�������Լ��İ�����ͬ������ͬ����ͬ
class JNIUtil
{
public:
	static void saveInt(const char* key,int value)
	{
		JniMethodInfo methodInfo;

		if (! JniHelper::getStaticMethodInfo(methodInfo,"com/bn/cocos2d/tank/TankActivity", "saveInt", "(Ljava/lang/String;I)V"))
		{
			return;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(key);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg,value);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	static int getInt(const char* key,int value)
	{
		JniMethodInfo methodInfo;

		if (! JniHelper::getStaticMethodInfo(methodInfo, "com/bn/cocos2d/tank/TankActivity","getInt", "(Ljava/lang/String;I)I"))
		{
			return 0;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(key);
		int res=methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, stringArg,value);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return res;
	}
};

#endif
