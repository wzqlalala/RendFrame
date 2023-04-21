#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>
#include <QString>

#include "mBasicEnum.h"

using namespace MBasicFunction;
namespace MDataGeo
{
	class RENDDATA_EXPORT mGeoLineData1
	{
	public:
		mGeoLineData1(QString partName, int ID);

		~mGeoLineData1();

		//设置几何线的坐标
		void appendGeoLineData(int ID, QVector<QVector3D> vertex);

		//设置几何线的属性
		void setGeoLineProperty(GeoLineProperty lineProperty);

		//获取几何线的所有坐标
		QVector<QVector3D> getGeoLineVertex();

		//获取几何线的属性
		GeoLineProperty getGeoLineProperty();

		//获取几何线的ID
		int getGeoLineID();

		//获取几何点所在的部件
		QString getPartName();

		//设置部件名称
		void setPartName(const QString &name);
	private:
		int _ID;

		//几何线所在的部件
		QString _partName;

		//显隐
		bool _isShow;

		//几何线的属性
		GeoLineProperty _lineProperty;

		//几何线的坐标
		QVector<QVector3D> _geoLineVertex;
	};
}

