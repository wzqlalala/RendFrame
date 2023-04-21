#pragma once
#include "renddata_global.h"
#include <QVector3D>
#include <QString>

namespace MDataGeo
{
	enum PointProperty
	{
		IndependentPoint,//������
		PointOnEdge//���ϵĵ�
	};
	class RENDDATA_EXPORT mGeoPointData1
	{
	public:
		mGeoPointData1(QString partName, int ID);

		~mGeoPointData1();

		//���ü��ε�����
		void setPointData(int PointID,QVector3D PointVertex);

		//���ü��ε������
		void setGeoPointProperty(PointProperty pointProperty);

		//��ȡ���ε��ID
		int getGeoPointID();

		//��ȡ���ε������
		QVector3D getGeoPointVertex();

		//��ȡ���ε������
		PointProperty getGeoPointProperty();

		//��ȡ���ε����ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);

	private:
		//���ε�ID
		int _ID;

		//���ε����ڵĲ���
		QString _partName;

		//���ε������
		PointProperty _pointProperty;

		//���ε������
		QVector3D _geoPointVertex;
	};
}

