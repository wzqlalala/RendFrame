#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>

#include <set>

namespace MDataPost
{

	struct PointCell
	{
		int addr = 0;
		int size = 0;
	};

	struct PointLine
	{
		int addr = 0;
		int size = 0;
	};

	class RENDDATA_EXPORT mPostMeshNodeData1
	{

	public:
		PointCell pointcell;

		PointLine pointline;

		mPostMeshNodeData1(int ID, QVector3D vertex);
		~mPostMeshNodeData1();

		//增加网格
		//void addMesh(int meshID);

		//减少网格
		bool deleteMesh(int meshID);

		//获取节点ID
		int getNodeID();

		//获取节点坐标
		QVector3D getNodeVertex();

		//设置节点坐标
		void setNodeVertex(int position, float value);

		//存储节点所在的网格ID
		//void setNodeInMeshIDs(const std::set<int>& ids);

		//获取节点所在的网格ID
		std::set<int> getNodeInMeshIDs();

		//获取节点所在的单元线ID
		std::set<int> getNodeInMeshLineIDs();

		//判断该节点是不是自由节点
		bool getNodeIsFreeNode();

	private:
		//节点ID
		int _nodeID;
		//节点坐标
		QVector3D _nodeVertex;


	};
}
