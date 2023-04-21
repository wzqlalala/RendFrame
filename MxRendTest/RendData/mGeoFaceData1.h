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

		//���ü���������
		void appendGeoFaceData(int ID, QVector<QVector3D> vertex, QVector<QVector3D> normal = QVector<QVector3D>{});

		//��ȡ���������������
		QVector<QVector3D> getGeoFaceVertex();

		//��ȡ����������з�����
		QVector<QVector3D> getGeoFaceNormal();

		//��ȡ�������ID
		int getGeoFaceID();

		//��ȡ���������ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);

	private:
		int _ID;

		//���������ڵĲ���
		QString _partName;

		bool _isShow;

		QVector<QVector3D> _geoFaceVertex;
		QVector<QVector3D> _geoFaceNormal;
	};
}

