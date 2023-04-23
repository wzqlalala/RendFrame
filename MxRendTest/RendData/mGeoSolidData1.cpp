#include "mGeoSolidData1.h"
#include "mGeoModelData1.h"


namespace MDataGeo
{
	mGeoSolidData1::mGeoSolidData1(mGeoModelData1 *geoModelData, QString partName, int ID)
	{
		_geoModelData = geoModelData;
		_isShow = true;
		_partName = partName;
		_ID = ID;
		_aabb = Space::AABB();
		geoModelData->appendGeoSolidData(ID, this);
	}

	mGeoSolidData1::~mGeoSolidData1()
	{
	}

	void mGeoSolidData1::appendGeoSolidFace(int GeoFaceID)
	{
		if (_geoFaceIDs.insert(GeoFaceID).second)//³É¹¦²åÈë
		{
			_aabb.push(_geoModelData->getGeoFaceAABBByID(GeoFaceID));
		}
	}
	std::set<int> mGeoSolidData1::getGeoSolidFaceIDs()
	{
		return _geoFaceIDs;
	}
	int mGeoSolidData1::getGeoSolidID()
	{
		return _ID;
	}

	QString mGeoSolidData1::getPartName()
	{
		return _partName;
	}
	void mGeoSolidData1::setPartName(const QString & name)
	{
		_partName = name;
	}
}
