#include "mGeoLineData1.h"

#include "mContainers.h"


namespace MDataGeo
{
	mGeoLineData1::mGeoLineData1(QString partName, int ID)
	{
		_lineProperty = GeoLineProperty::IndependentEdge;
		_partName = partName;
		_ID = ID;
		MBasicFunction::_globalLineId = std::max(MBasicFunction::_globalLineId, ID);
	}

	mGeoLineData1::~mGeoLineData1()
	{
	}

	void mGeoLineData1::appendGeoLineData(int ID, QVector<QVector3D> vertex)
	{
		_ID = ID;
		_geoLineVertex = vertex;
	}

	void mGeoLineData1::setGeoLineProperty(GeoLineProperty lineProperty)
	{
		_lineProperty = lineProperty;
	}
	QVector<QVector3D> mGeoLineData1::getGeoLineVertex()
	{
		return _geoLineVertex;
	}
	GeoLineProperty mGeoLineData1::getGeoLineProperty()
	{
		return _lineProperty;
	}
	int mGeoLineData1::getGeoLineID()
	{
		return _ID;
	}

	QString mGeoLineData1::getPartName()
	{
		return _partName;
	}

	void mGeoLineData1::setPartName(const QString & name)
	{
		_partName = name;
	}
}
