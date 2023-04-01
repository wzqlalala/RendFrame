#include "mPostMeshLineData1.h"
namespace MDataPost
{
	mPostMeshLineData1::mPostMeshLineData1(int ID, std::set<int> index, int meshID,int order, QString partName)
	{
		_ID = ID;
		_index1 = *index.begin();
		_index2 = *(++index.begin());
		_meshID1 = meshID;
		_order = order;
		_partName = partName;
	}

	mPostMeshLineData1::~mPostMeshLineData1()
	{
	}
	int mPostMeshLineData1::getMeshLineNodeIndex1()
	{
		return _index1;
	}

	int mPostMeshLineData1::getMeshLineNodeIndex2()
	{
		return _index2;
	}
	int mPostMeshLineData1::getMeshLineID()
	{
		return _ID;
	}
	QString mPostMeshLineData1::getPartName()
	{
		return _partName;
	}
	int mPostMeshLineData1::getMeshID()
	{
		return _meshID1;
	}

	short mPostMeshLineData1::getMeshOrder()
	{
		return _order;
	}
}
