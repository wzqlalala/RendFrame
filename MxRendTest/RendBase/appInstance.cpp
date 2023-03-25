#include "appInstance.h"


namespace mxr
{
	ApplicationInstance& ApplicationInstance::GetInstance()
	{
		// since C++11, this will be thread-safe, there's no need for manual locking
		static ApplicationInstance instance;
		return instance;

	}
}

	


