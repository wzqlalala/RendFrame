#include "mPostAnimationRendData.h"
#include "mPostOneFrameRendData.h"

namespace MDataPost
{
	mPostAnimationRendData::mPostAnimationRendData()
	{
		_allMinData = 0;
		_allMaxData = 0;
	}

	mPostAnimationRendData::~mPostAnimationRendData()
	{
		deleteAnimationRendData();
	}

	std::set<int> mPostAnimationRendData::getRendAnimationIds()
	{
		return _allRendFrameids;
	}

	void mPostAnimationRendData::appendRendOneFrameData(int id, mPostOneFrameRendData* postOneFrameData)
	{
		if (_rendFrameid_RendFrameData.contains(id))
		{
			mPostOneFrameRendData *postOneFrameData = _rendFrameid_RendFrameData.value(id);
			if (postOneFrameData != nullptr)
			{
				delete postOneFrameData;
				postOneFrameData = nullptr;
			}
		}
		_rendFrameid_RendFrameData[id] = postOneFrameData;
		if (_rendFrameid_RendFrameData.size() == 1)
		{
			_allMinData = postOneFrameData->getOriginalMinData();
			_allMaxData = postOneFrameData->getOriginalMaxData();
		}
		else
		{
			if (postOneFrameData->getOriginalMinData() < _allMinData)
			{
				_allMinData = postOneFrameData->getOriginalMinData();
			}
			if (postOneFrameData->getOriginalMaxData() > _allMaxData)
			{
				_allMaxData = postOneFrameData->getOriginalMaxData();
			}
		}
		_allRendFrameids.insert(id);
	}

	mPostOneFrameRendData* mPostAnimationRendData::getRendOneFrameData(int id)
	{
		return _rendFrameid_RendFrameData.value(id);
	}

	void mPostAnimationRendData::deleteAnimationRendData()
	{
		//for (auto iter = _allRendFrameids.begin(); iter != _allRendFrameids.end(); ++iter)
		//{
		//	mPostOneFrameRendData *postOneFrameData = _rendFrameid_RendFrameData.value(*iter);
		//	if (postOneFrameData != nullptr)
		//	{
		//		delete postOneFrameData;
		//		postOneFrameData = nullptr;
		//	}
		//}
		_rendFrameid_RendFrameData.clear();
		_allRendFrameids.clear();

	}

	void mPostAnimationRendData::setAnimationFrameRange(bool isAgreement)
	{
		if (isAgreement)
		{
			QHashIterator<int, mPostOneFrameRendData*> iter(_rendFrameid_RendFrameData);
			while (iter.hasNext())
			{
				iter.next();
				iter.value()->setMinData(_allMinData);
				iter.value()->setMaxData(_allMaxData);
			}
		}
		else
		{
			QHashIterator<int, mPostOneFrameRendData*> iter(_rendFrameid_RendFrameData);
			while (iter.hasNext())
			{
				iter.next();
				iter.value()->resetMinMaxData();
			}
		}
	}
	void mPostAnimationRendData::setAnimationCustomFrameRange(float minData, float maxData)
	{
		QHashIterator<int, mPostOneFrameRendData*> iter(_rendFrameid_RendFrameData);
		while (iter.hasNext())
		{
			iter.next();
			iter.value()->setMinMaxData(maxData, minData);
		}
	}
}
