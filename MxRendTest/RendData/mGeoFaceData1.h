#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>
#include <QString>

namespace MDataGeo
{
	class RENDDATA_EXPORT mGeoFaceData1
	{
	public:
		mGeoFaceData1(QString partName,int ID);

		~mGeoFaceData1();

		//设置几何面数据
		void appendGeoFaceData(int ID, QVector<QVector3D> vertex, QVector<QVector3D> normal = QVector<QVector3D>{});

		//获取几何面的所有坐标
		QVector<QVector3D> getGeoFaceVertex();

		//获取几何面的所有法向量
		QVector<QVector3D> getGeoFaceNormal();

		//获取几何面的ID
		int getGeoFaceID();

		//获取几何面所在的部件
		QString getPartName();

		//设置部件名称
		void setPartName(const QString &name);

	private:
		int _ID;

		//几何面所在的部件
		QString _partName;

		bool _isShow;

		QVector<QVector3D> _geoFaceVertex;
		QVector<QVector3D> _geoFaceNormal;
	};
}

