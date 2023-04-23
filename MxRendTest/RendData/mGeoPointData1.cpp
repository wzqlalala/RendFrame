#include "mGeoPointData1.h"
#include "mGeoModelData1.h"

namespace MDataGeo
{
	mGeoPointData1::mGeoPointData1(mGeoModelData1 *geoModelData, QString partName, int ID)
	{
		_geoModelData = geoModelData;
		_pointProperty = IndependentPoint;
		_partName = partName;
		_ID = ID;
		geoModelData->appendGeoPointData(ID, this);
	}

	mGeoPointData1::~mGeoPointData1()
	{
		
	}

	void mGeoPointData1::setPointData(int PointID, QVector3D PointVertex)
	{
		_ID = PointID;
		_geoPointVertex = PointVertex;
	}

	void mGeoPointData1::setGeoPointProperty(PointProperty pointProperty)
	{
		_pointProperty = pointProperty;
	}
	int mGeoPointData1::getGeoPointID()
	{
		return _ID;
	}
	QVector3D mGeoPointData1::getGeoPointVertex()
	{
		return _geoPointVertex;
	}
	PointProperty mGeoPointData1::getGeoPointProperty()
	{
		return _pointProperty;
	}

	QString mGeoPointData1::getPartName()
	{
		return _partName;
	}
	void mGeoPointData1::setPartName(const QString & name)
	{
		_partName = name;
	}
}
