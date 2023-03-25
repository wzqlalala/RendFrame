#pragma once
#include "renddata_global.h"

#include <QString>
#include <QSet>
#include <QPair>

namespace MDataPost
{
	class RENDDATA_EXPORT mPostMeshFaceData1
	{

	public:
		//复制构造函数
		mPostMeshFaceData1(int ID, QVector<int> index, int meshID, int order, QString partName);

		//析构函数
		~mPostMeshFaceData1();

		//添加第二对网格ID
		void appendMeshID(int meshID, int order, int startIndex);

		//获取第二对网格面的节点索引
		QVector<int> getNodeIndex();

		//获取对应网格的网格面的节点索引
		QVector<int> getNodeIndex(int meshID);

		//获取第一对网格
		QPair<int, int> getFirstMesh();

		//获取所在的部件名称
		QString getPartName();

		//获取网格ID1
		int getMeshID1();

		//获取序号1
		short int getMeshOrder1();

		//获取网格ID2
		int getMeshID2();

		//获取序号2
		short int getMeshOrder2();

		//获取单元面的ID
		int getMeshFaceID();

		//删除单元调用，然后判断是否删除单元面
		bool deleteMesh(int meshID);

		//获取是否渲染
		bool getVisual();

		//设置是否渲染
		void setVisual(bool isShow);

	private:
		//单元面ID
		int _ID;
		//单元面上的节点索引
		int _index[4] = { -1 };

		//第一对网格ID
		int _meshID1;	
		//第二对网格ID
		int _meshID2;

		//第二对网格ID的起始索引位置
		uint8_t _startIndex;
		
		uint8_t _order1;
		uint8_t _order2;
		//是否渲染
		bool _isShow;
	};
}
