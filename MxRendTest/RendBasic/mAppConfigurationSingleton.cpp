#include "mAppConfigurationSingleton.h"
#include "mFileConfiguration.h"
//#include "mUnitConfiguration.h"

namespace MAppConfiguration
{
	mAppConfigurationSingleton *mAppConfigurationSingleton::_instance = nullptr;

	mAppConfigurationSingleton* mAppConfigurationSingleton::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new mAppConfigurationSingleton;
		}
		return _instance;
	}

	mAppConfigurationSingleton::mAppConfigurationSingleton()
	{
		_fileConfiguration = new mFileConfiguration();
		//_unitConfiguration = new mUnitConfiguration();
	}

	mAppConfigurationSingleton::~mAppConfigurationSingleton()
	{
		if (_fileConfiguration != nullptr)
		{
			delete _fileConfiguration;
			_fileConfiguration = nullptr;
		}
		//if (_unitConfiguration != nullptr)
		//{
		//	delete _unitConfiguration;
		//	_unitConfiguration = nullptr;
		//}
	}

	mFileConfiguration* mAppConfigurationSingleton::getFileConfiguration()
	{
		return _fileConfiguration;
	}
	//mUnitConfiguration * mAppConfigurationSingleton::getUnitConfiguration()
	//{
	//	return _unitConfiguration;
	//}
}