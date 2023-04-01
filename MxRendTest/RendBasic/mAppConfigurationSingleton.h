#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "rendbasic_global.h"

namespace MAppConfiguration
{
	class mFileConfiguration;
	//class mUnitConfiguration;

	class RENDBASIC_EXPORT mAppConfigurationSingleton
	{
	public:
		/*
		* 获取单例指针
		*/
		static mAppConfigurationSingleton* getInstance();

		/*
		* 获取文件配置
		*/
		mFileConfiguration* getFileConfiguration();

		/*
		* 获取单位制配置
		*/
		//mUnitConfiguration* getUnitConfiguration();

	private:
		/*
		* 默认构造函数
		*/
		mAppConfigurationSingleton();

		/*
		* 析构函数
		*/
		~mAppConfigurationSingleton();

	private:
		//单例指针
		static mAppConfigurationSingleton* _instance;

		//配置文件
		mFileConfiguration* _fileConfiguration;

		//单位制
		//mUnitConfiguration* _unitConfiguration;
	};
}
