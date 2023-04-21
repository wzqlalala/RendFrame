#pragma once
#include "renddata_global.h"
#include <QVector3D>
#include <set>
#include <QString>

#include "mBasicStructor.h"
#include "SpaceTree.h"

namespace MDataGeo
{
	class RENDDATA_EXPORT mGeoPartData1
	{

	public:
		mGeoPartData1();

		mGeoPartData1(QString name,int ID);

		~mGeoPartData1();

		//设置部件尺寸的最值
		void setGeoPartSize(Space::AABB partSize);

		//设置部件的颜色
		void setPartColor(QVector3D color);

		//设置部件显隐
		void setPartVisual(bool isshow);

		//设置部件的类型
		void setGeoShapeType(int shapeType);

		//添加几何点ID
		void appendGeoPointID(int geoPointID);

		//添加几何线ID
		void appendGeoLineID(int geoLineID);

		//添加几何面ID
		void appendGeoFaceID(int geoFaceID);

		//添加几何实体ID
		void appendGeoSolidID(int geoSolidID);

		//获取部件类型
		int getGeoShapeType();

		//获取所有的几何点ID
		std::set<int> getGeoPointIDs();

		//获取所有的几何线ID
		std::set<int> getGeoLineIDs();

		//获取所有的几何面ID
		std::set<int> getGeoFaceIDs();

		//获取所有几何实体ID
		std::set<int> getGeoSolidIDs();

		//获取部件的颜色
		QVector3D getPartColor();

		//判断部件是否显示
		bool getPartVisual();

		//获取部件编号
		int getPartID();

		//获取部件尺寸的最值
		Space::AABB getGeoPartSize();

	private:

		static QList<QVector3D> Color;

		//部件属性
		QString _partName;
		int _partID;
		bool _isShow;
		QVector3D _partColor;
		int _shapeType;//0为混合，1为多Solid，2为单Solid，4为Face，6为Edge（Line），7为Vertex（Vertex）

		//部件的几何点信息
		std::set<int> _geoPointIDs;

		//部件的几何线信息
		std::set<int> _geoLineIDs;

		//部件的几何面信息
		std::set<int> _geoFaceIDs;

		//部件的几何体信息
		std::set<int> _geoSolidIDs;

		//部件的几何尺寸
		Space::AABB _partSize;

	};
}

