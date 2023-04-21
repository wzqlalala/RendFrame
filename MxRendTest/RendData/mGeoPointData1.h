#pragma once
#include "renddata_global.h"
#include <QVector3D>
#include <QString>

namespace MDataGeo
{
	enum PointProperty
	{
		IndependentPoint,//独立点
		PointOnEdge//边上的点
	};
	class RENDDATA_EXPORT mGeoPointData1
	{
	public:
		mGeoPointData1(QString partName, int ID);

		~mGeoPointData1();

		//设置几何点数据
		void setPointData(int PointID,QVector3D PointVertex);

		//设置几何点的属性
		void setGeoPointProperty(PointProperty pointProperty);

		//获取几何点的ID
		int getGeoPointID();

		//获取几何点的坐标
		QVector3D getGeoPointVertex();

		//获取几何点的属性
		PointProperty getGeoPointProperty();

		//获取几何点所在的部件
		QString getPartName();

		//设置部件名称
		void setPartName(const QString &name);

	private:
		//几何点ID
		int _ID;

		//几何点所在的部件
		QString _partName;

		//几何点的属性
		PointProperty _pointProperty;

		//几何点的坐标
		QVector3D _geoPointVertex;
	};
}

