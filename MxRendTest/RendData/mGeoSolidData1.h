#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QString>
#include <set>

#include "SpaceTree.h"

namespace MDataGeo
{
	class mGeoModelData1;
	class mGeoFaceData1;
	class RENDDATA_EXPORT mGeoSolidData1
	{
	public:
		mGeoSolidData1(mGeoModelData1 *geoModelData, QString partName, int ID);

		~mGeoSolidData1();

		//添加几何实体上的几何面ID
		void appendGeoSolidFace(int GeoFaceID);

		//获取几何实体上的所有几何面ID
		std::set<int> getGeoSolidFaceIDs();

		//获取几何实体的ID
		int getGeoSolidID();

		//获取几何点所在的部件
		QString getPartName();

		//设置部件名称
		void setPartName(const QString &name);

		//获取几何实体的最值
		Space::AABB getGeoSolidAABB() { return _aabb; };

	private:
		mGeoModelData1 *_geoModelData;

		//几何实体的ID
		int _ID;

		//几何实体所在的部件
		QString _partName;

		//显隐
		bool _isShow;

		//几何实体上的所有几何面ID
		std::set<int> _geoFaceIDs;

		//几何实体的包围盒
		Space::AABB _aabb;
	};
}

