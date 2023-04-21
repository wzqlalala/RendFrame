#include "mGeoPartData1.h"

#include <QList>

namespace MDataGeo
{

	QList<QVector3D> mGeoPartData1::Color = QList<QVector3D>{ QVector3D(1.00f, 0.00f, 0.00f),
										   QVector3D(0.00f, 0.00f, 1.00f),
										  QVector3D(0.65f, 0.59f, 0.0f),
										  QVector3D(0.84f, 0.19f, 0.46f),
										  QVector3D(0.57f, 0.43f, 0.85f),
										  QVector3D(0.91f, 0.67f, 0.65f),
										  QVector3D(0.50f, 0.24f, 0.12f),
										  QVector3D(0.05f, 0.94f, 0.04f) };

	mGeoPartData1::mGeoPartData1()
	{
		_partName = "";
		_isShow = true;
		_partID = 0;
		int num = (_partID - 1) % Color.size();
		_partColor = Color.at(num);
	}

	mGeoPartData1::mGeoPartData1(QString name,int ID)
	{
		_partName = name;
		_isShow = true;
		_partID = ID;
		int num = (_partID - 1) % Color.size();
		_partColor = Color.at(num);
	}

	mGeoPartData1::~mGeoPartData1()
	{
	}

	void mGeoPartData1::setGeoPartSize(Space::AABB partSize)
	{
		_partSize = partSize;
	}

	void mGeoPartData1::setPartColor(QVector3D color)
	{
		_partColor = color/255.0;
	}

	void mGeoPartData1::setPartVisual(bool isshow)
	{
		_isShow = isshow;
	}

	void mGeoPartData1::setGeoShapeType(int shapeType)
	{
		_shapeType = shapeType;
	}

	void mGeoPartData1::appendGeoPointID(int geoPointID)
	{
		_geoPointIDs.insert(geoPointID);
	}

	void mGeoPartData1::appendGeoLineID(int geoLineID)
	{
		_geoLineIDs.insert(geoLineID);
	}

	void mGeoPartData1::appendGeoFaceID(int geoFaceID)
	{
		_geoFaceIDs.insert(geoFaceID);
	}

	void mGeoPartData1::appendGeoSolidID(int geoSolidID)
	{
		_geoSolidIDs.insert(geoSolidID);
	}
	int mGeoPartData1::getGeoShapeType()
	{
		return _shapeType;
	}
	std::set<int> mGeoPartData1::getGeoPointIDs()
	{
		return _geoPointIDs;
	}
	std::set<int> mGeoPartData1::getGeoLineIDs()
	{
		return _geoLineIDs;
	}
	std::set<int> mGeoPartData1::getGeoFaceIDs()
	{
		return _geoFaceIDs;
	}
	std::set<int> mGeoPartData1::getGeoSolidIDs()
	{
		return _geoSolidIDs;
	}
	QVector3D mGeoPartData1::getPartColor()
	{
		return _partColor;
	}
	bool mGeoPartData1::getPartVisual()
	{
		return _isShow;
	}
	int mGeoPartData1::getPartID()
	{
		return _partID;
	}

	Space::AABB mGeoPartData1::getGeoPartSize()
	{
		return _partSize;
	}
}
