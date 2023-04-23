#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"
#include <QObject>
#include <set>
#include <QHash>

//MBasicFunction
#include "mBasicStructor.h"


namespace MDataGeo
{
	using namespace std;
	using namespace MBasicFunction;
	class RENDDATA_EXPORT mGeoPickData1
	{

	public:

		//设置添加拾取还是减少拾取
		void setGeoPickFunction(int pickfunction);//0加1减

		//拾取时调用
		void setSoloPickGeoPointData(int Pointid, float depth);
		void setSoloPickGeoLineData(int Lineid, float depth);
		void setSoloPickGeoFaceData(int Faceid, float depth);
		void setSoloPickGeoSolidData(int Solidid, float depth);
		void setSoloPickVertexOnGeoLineData(int geoLineID, QVector3D vertex, float depth);
		void setSoloPickVertexOnGeoFaceData(int geoFaceID, QVector3D vertex, float depth);
		void setSoloPickVertexOnScreenData(QVector3D vertex);
		void setSoloPickGeoPointByPartData(set<int> Pointids, float depth);
		void setSoloPickGeoLineByPartData(set<int> Lineids, float depth);
		void setSoloPickGeoFaceByPartData(set<int> Faceids, float depth);
		void setSoloPickGeoSolidByPartData(set<int> Solidids, float depth);
		void setSoloPickGeoPartData(QString partName, float depth);


		void setSoloPickGeoSolidByPartData(QString PartName, float depth);

		void setMutiplyPickGeoPointData(set<int> Pointids);
		void setMutiplyPickGeoLineData(set<int> Lineids);
		void setMutiplyPickGeoFaceData(set<int> Faceids);
		void setMutiplyPickGeoSolidData(set<int> Solidids);
		void setMutiplyPickGeoPartData(QString partName);

		//一次性高亮，清除上一次高亮
		void setAllPickGeoPointData(set<int> Pointids);
		void setAllPickGeoLineData(set<int> Lineids);
		void setAllPickGeoFaceData(set<int> Faceids);
		void setAllPickGeoSolidData(set<int> Solidids);
		void setAllPickGeoPartData(set<QString> partNames);

		//累积高亮
		void setAddPickGeoPointData(set<int> Pointids);
		void setAddPickGeoLineData(set<int> Lineids);
		void setAddPickGeoFaceData(set<int> Faceids);
		void setAddPickGeoSolidData(set<int> Solidids);
		void setAddPickGeoPartData(set<QString> partNames);

		//减少高亮
		void setReducePickGeoPointData(set<int> Pointids);
		void setReducePickGeoLineData(set<int> Lineids);
		void setReducePickGeoFaceData(set<int> Faceids);
		void setReducePickGeoSolidData(set<int> Solidids);
		void setReducePickGeoPartData(set<QString> partNames);

		//单选拾取完成后调用
		void setSoloPickData();

		//获取拾取后的几何点编号
		set<int> getPickPointIDs();

		//获取拾取后的几何线编号
		set<int> getPickLineIDs();

		//获取拾取后的几何面编号
		set<int> getPickFaceIDs();

		//获取拾取后的几何体编号
		set<int> getPickSolidIDs();

		//获取拾取后的几何部件名称
		set<QString> getPickGeoPartNames();

		//获取拾取后的几何线上的任意点名称
		QHash<int, QVector<QVector3D>> getPickVertexOnGeoLineDatas();

		//获取拾取后的几何面上的任意点名称
		QHash<int, QVector<QVector3D>> getPickVertexOnGeoFaceDatas();

		//获取拾取后的屏幕上的任意点坐标
		QVector<QVector3D> getPickVertexOnScreenDatas();

		//清除所有几何的高亮
		void clearAllPickData();

		//默认构造函数
		mGeoPickData1();

		//析构函数
		~mGeoPickData1();


	private:

		int _pickFunction;//0加1减

		IDDepthBuffer _solidBuffer;//当前几何体编号

		IDDepthBuffer _faceBuffer;//当前几何面编号

		IDDepthBuffer _lineBuffer;//当前几何线编号

		IDDepthBuffer _pointBuffer;//当前几何点编号

		NameDepthBuffer _partBuffer;//当前部件编号

		VertexOnGeoLineDepthBuffer _vertexOnLineBuffer;//当前点在几何线上的坐标

		VertexOnGeoFaceDepthBuffer _vertexOnFaceBuffer;//当前点在几何面上的坐标

		IDSetDepthBuffer _solidBuffers;//按部件拾取几何体的编号

		IDSetDepthBuffer _faceBuffers;//按部件拾取几何面的编号

		IDSetDepthBuffer _lineBuffers;//按部件拾取几何线的编号

		IDSetDepthBuffer _pointBuffers;//按部件拾取几何点的编号

		NameDepthBuffer _partSolidBuffer;//当前部件几何体编号

		NameDepthBuffer _partFaceBuffer;//当前部件几何面编号

		NameDepthBuffer _partLineBuffer;//当前部件几何线编号

		NameDepthBuffer _partPointBuffer;//当前部件几何点编号

		set<int> _pickPoints;//最终拾取到的几何点
		set<int> _pickLines;//最终拾取到的几何线
		set<int> _pickFaces;//最终拾取到的几何面
		set<int> _pickSolids;//最终拾取到的几何体
		set<QString> _pickParts;//最终拾取到的几何部件
		QHash<int, QVector<QVector3D>> _pickVertexsOnLines;//最终拾取到的几何边上的任意点
		QHash<int, QVector<QVector3D>> _pickVertexsOnFaces;//最终拾取到的几何面上的任意点
		QVector<QVector3D> _pickVertexsOnScreen;//最终拾取到的屏幕上的点
	};
}
