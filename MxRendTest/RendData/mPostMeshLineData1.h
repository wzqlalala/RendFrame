#pragma once
#include "renddata_global.h"

#include <QString>
#include <set>

namespace MDataPost
{
	class RENDDATA_EXPORT mPostMeshLineData1
	{

	public:
		mPostMeshLineData1(int ID, std::set<int> index, int meshID, int order, QString partName);

		~mPostMeshLineData1();

		//获取单元线的第一个节点的索引
		int getMeshLineNodeIndex1();

		//获取单元线的第二个节点的索引
		int getMeshLineNodeIndex2();

		//获取单元线的ID
		int getMeshLineID();

		//获取单元线所在的部件的名称
		QString getPartName();

		//获取单元线所在的单元ID
		int getMeshID();

		//获取序号
		short int getMeshOrder();
	private:
		//单元线ID
		int _ID;

		//节点索引1
		int _index1;

		//节点索引2
		int _index2;

		//单元线所在的网格ID1
		int _meshID1;
		short _order;

		//单元线所在的部件名称
		QString _partName;
	};
}
