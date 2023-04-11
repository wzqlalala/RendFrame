#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

//MBasicFunction
#include "mBasicStructor.h"
#include "mBasicEnum.h"

//Qt
#include <QVector>
#include <QVector4D>

namespace MDataPost
{
	using namespace std;
	using namespace MBasicFunction;
	class RENDDATA_EXPORT mPostMeshPickData
	{
	public:
		mPostMeshPickData();

		~mPostMeshPickData();

		//设置添加拾取还是减少拾取
		void setMeshPickFunction(int pickfunction);//0加1减

		//拾取时调用
		void setSoloPickNodeData(int nodeid, float depth);
		void setSoloPickMeshData(int meshid, float depth);
		void setSoloPickMeshFaceData(int meshfaceid, float depth);
		void setSoloPickMeshPartData(QString partName, float depth);
		void setSoloPickNodeByPartData(set<int> nodeids, float depth);
		void setSoloPickMeshByPartData(set<int> meshids, float depth);
		void setSoloPickMeshDataByAngle(int meshid, QString partName, float depth);
		void setSoloPickMeshLineDataByAngle(int meshlineid, QString partName, float depth);
		void setSoloPickMeshFaceDataByAngle(int meshfaceid, QString partName, float depth);

		void setMultiplyPickNodeData(set<int> nodeids);
		void setMultiplyPickMeshData(set<int> meshids);
		void setMultiplyPickMeshFaceData(set<int> meshfaceids);
		void setMultiplyPickMeshPartData(QString partNames);

		//一次性高亮，清除上一次高亮
		void setAllPickNodeData(set<int> nodeids);
		void setAllPickMeshData(set<int> meshids);
		void setAllPickMeshFaceData(set<int> meshfaceids);
		void setAllPickMeshPartData(set<QString> partNames);
		void setAllPickAnyPosition(QVector3D vertex, float value);

		//累积高亮
		void setAddPickNodeData(set<int> nodeids);
		void setAddPickMeshData(set<int> meshids);
		void setAddPickMeshFaceData(set<int> meshfaceids);
		void setAddPickMeshPartData(set<QString> partNames);
		void setAddPickAnyPosition(QVector3D vertex, float value);

		//减少高亮
		void setReducePickNodeData(set<int> nodeids);
		void setReducePickMeshData(set<int> meshids);
		void setReducePickMeshFaceData(set<int> meshfaceids);
		void setReducePickMeshPartData(set<QString> partNames);

		//单选不保留顺序拾取完成后调用
		void setSoloPickData();

		//单选保留顺序时完成后调用
		void setSoloOrderPickData();

		//获取拾取后的节点编号
		set<int> getPickNodeIDs();

		//获取拾取后的节点编号(保留拾取顺序)
		QVector<int> getPickNodeIDsOrder();

		//获取拾取后的单元编号
		set<int> getPickMeshIDs();

		//获取拾取后的单元面编号
		set<int> getPickMeshFaceIDs();

		//获取拾取后的网格部件名称
		set<QString> getPickMeshPartNames();

		//获取任意位置的值
		QVector<QVector4D> getAnyPositionValue();

		//清空全部拾取
		void clearAllPickData();

		void clearPickNodeData();
		void clearPickMeshData();
		void clearPickMeshLineData();
		void clearPickMeshFaceData();
		void clearPickMeshPartData();

		//通过角度拾取
		QPair<PickObjectType, QPair<QString, int>> getSoloPickNodeDataByLineAngle();//拾取到的对象和其编号
		QPair<PickObjectType, QPair<QString, int>> getSoloPickNodeDataByFaceAngle();//拾取到的对象和其编号
		QPair<QString, int> getSoloPickMeshDataByAngle();
		QPair<QString, int> getSoloPickMeshLineDataByAngle();
		QPair<QString, int> getSoloPickMeshFaceDataByAngle();

	private:

		int _pickFunction;//0加1减

		IDDepthBuffer _nodeBuffer;//当前节点编号

		IDDepthBuffer _meshBuffer;//当前单元编号

		IDDepthBuffer _meshLineBuffer;//当前单元线编号

		IDDepthBuffer _meshFaceBuffer;//当前单元面编号

		NameDepthBuffer _partBuffer;//当前部件名称

		IDSetDepthBuffer _nodeBuffers;//按部件拾取节点的编号

		IDSetDepthBuffer _meshFaceBuffers;//按部件拾取单元面的编号

		IDSetDepthBuffer _meshBuffers;//按部件拾取单元的编号

		IDQStringDepthBuffer _meshPartNameBuffers;//按角度拾取单元的编号

		IDQStringDepthBuffer _meshLinePartNameBuffers;//按角度拾取单元线的编号

		IDQStringDepthBuffer _meshFacePartNameBuffers;//按角度拾取单元面的编号

		set<int> _pickNodes;//最终拾取到的节点
		QVector<int> _pickNodesOrder;//最终拾取到的节点（保留拾取顺序）
		set<int> _pickMeshs;//最终拾取到的单元
		set<int> _pickMeshFaces;//最终拾取到的单元面
		set<QString> _pickParts;//最终拾取到的部件
		QVector<QVector4D> _pickAnyPosition;//最终的任意位置的点
	};
}

