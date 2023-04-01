#pragma once
#include "renddata_global.h"

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QList>
#include <set>
#include "SpaceTree.h"
//MBasicFunction
#include "mBasicStructor.h"

namespace MDataPost
{
	class RENDDATA_EXPORT mPostMeshPartData1
	{
	public:
		//Space::SpaceTree *spaceTree;
		mPostMeshPartData1();

		mPostMeshPartData1(QString name,int ID);

		~mPostMeshPartData1();

		//设置部件颜色
		void setPartColor(QVector3D color);

		//设置部件显隐
		void setPartVisual(bool isshow);

		//设置0维网格ID
		void setMeshID0(const std::set<int>& ids);

		//设置一维网格ID
		void setMeshID1(const std::set<int>& ids);

		//设置二维网格ID
		void setMeshID2(const std::set<int>& ids);

		//设置三维网格ID
		void setMeshID3(const std::set<int>& ids);

		//设置网格线ID
		void setMeshLineID(const std::set<int>& ids);

		//设置网格面ID
		void setMeshFaceID(const std::set<int>& ids);


		//添加0维网格
		void appendMesh0(mPostMeshData1* meshID);

		//添加1维网格
		void appendMesh1(mPostMeshData1* meshID);

		//添加2维网格
		void appendMesh2(mPostMeshData1* meshID);

		//添加三维网格
		void appendMesh3(mPostMeshData1* meshID);

		//添加网格线ID
		void appendMeshLineID(int meshLineID);

		//添加几何面ID
		void appendMeshFaceID(int meshFaceID);

		//删除0维网格ID
		void deleteMeshID0(int meshID);

		//删除1维网格ID
		void deleteMeshID1(int meshID);

		//删除2维网格ID
		void deleteMeshID2(int meshID);

		//删除3维网格ID
		void deleteMeshID3(int meshID);

		//删除网格线ID
		void deleteMeshLineID(int meshLineID);

		//删除几何面ID
		void deleteMeshFaceID(int meshFaceID);

		//获取所有的0维网格ID
		std::set<int> getMeshIDs0();

		//获取所有的1维网格ID
		std::set<int> getMeshIDs1();

		//获取所有的2维网格ID
		std::set<int> getMeshIDs2();

		//获取所有的3维网格ID
		std::set<int> getMeshIDs3();

		//获取所有的0维网格指针
		QVector<mPostMeshData1*> getMeshDatas0();

		//获取所有的1维网格指针
		QVector<mPostMeshData1*> getMeshDatas1();

		//获取所有的2维网格指针
		QVector<mPostMeshData1*> getMeshDatas2();

		//获取所有的3维网格指针
		QVector<mPostMeshData1*> getMeshDatas3();

		//获取该部件网格数
		int getMeshCount();

		//获取所有的网格线ID
		std::set<int> getMeshLineIDs();

		//获取所有的网格面ID
		std::set<int> getMeshFaceIDs();

		//获取所有的网格面指针
		std::set<mPostMeshFaceData1*> getMeshFaces();

		//获取部件的颜色
		QVector3D getPartColor();

		//判断部件是否显示
		bool getPartVisual();

		//获取部件编号
		int getPartID();

		//获取部件名称
		QString getPartName();

		//添加表面网格数据
		void appendMeshFaceData(const QVector<mPostMeshFaceData1*> &face);

		//设置表面网格数据
		void setMeshFaceData(const QVector<mPostMeshFaceData1*> &face);

		//清空表面网格数据
		void clearMeshFaceData();

		//获取表面网格数据
		QVector<mPostMeshFaceData1*> getMeshFaceData();

	private:
		//static int ID;
		static QList <QVector3D> Color;

		//部件属性
		QString _partName;
		int _partID;
		bool _isShow;
		QVector3D _partColor;

		//部件的单元线信息
		std::set<int> _meshLineID;
		
		//所有的网格信息
		QVector<mPostMeshData1*> _meshData3;
		QVector<mPostMeshData1*> _meshData2;
		QVector<mPostMeshData1*> _meshData1;
		QVector<mPostMeshData1*> _meshData0;

		//表面网格数据
		QVector<mPostMeshFaceData1*> _faceData;

	};
}

