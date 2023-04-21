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

		//���ü����ߵ�����
		void appendGeoLineData(int ID, QVector<QVector3D> vertex);

		//���ü����ߵ�����
		void setGeoLineProperty(GeoLineProperty lineProperty);

		//��ȡ�����ߵ���������
		QVector<QVector3D> getGeoLineVertex();

		//��ȡ�����ߵ�����
		GeoLineProperty getGeoLineProperty();

		//��ȡ�����ߵ�ID
		int getGeoLineID();

		//��ȡ���ε����ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);
	private:
		int _ID;

		//���������ڵĲ���
		QString _partName;

		//����
		bool _isShow;

		//�����ߵ�����
		GeoLineProperty _lineProperty;

		//�����ߵ�����
		QVector<QVector3D> _geoLineVertex;
	};
}

