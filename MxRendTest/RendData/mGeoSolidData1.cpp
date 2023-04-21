#include "mGeoSolidData1.h"


namespace MDataGeo
{
	mGeoSolidData1::mGeoSolidData1(QString partName, int ID)
	{
		_isShow = true;
		_partName = partName;
		_ID = ID;
	}

	mGeoSolidData1::~mGeoSolidData1()
	{
	}

	void mGeoSolidData1::appendGeoSolidFace(int GeoFaceID)
	{
		_geoFaceIDs.insert(GeoFaceID);
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
