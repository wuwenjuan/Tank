#ifndef __JNIUtil_H__
#define __JNIUtil_H__

#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;

/*
 * 在Cocos2dx项目的主Activity中声明成员变量
 * ---------------------------------------------------------------------------
 * public static SharedPreferences sharedPreferences;
 * public static SharedPreferences.Editor editor;
 * ---------------------------------------------------------------------------
 * 在--protected void onCreate(Bundle savedInstanceState){}--方法里面初始化成员变量
 * ---------------------------------------------------------------------------
 * sharedPreferences = this.getSharedPreferences("cn",Context.MODE_PRIVATE);
 * editor = sharedPreferences.edit();
 * ---------------------------------------------------------------------------
 * 即可
 */
//保存数据的JNIUtil类com/bn/cocos2d/tank/TankActivity
//--classPath为java端声明这些方法的类的路径 例如---- "com/bn/cocos2d/Cocos2dxJniTest/HelloActivity"，根据自己的包名不同类名不同而不同
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
