#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QString>
#include <set>

namespace MDataGeo
{
	class RENDDATA_EXPORT mGeoSolidData1
	{
	public:
		mGeoSolidData1(QString partName, int ID);

		~mGeoSolidData1();

		//��Ӽ���ʵ���ϵļ�����ID
		void appendGeoSolidFace(int GeoFaceID);

		//��ȡ����ʵ���ϵ����м�����ID
		std::set<int> getGeoSolidFaceIDs();

		//��ȡ����ʵ���ID
		int getGeoSolidID();

		//��ȡ���ε����ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);

	private:
		//����ʵ���ID
		int _ID;

		//����ʵ�����ڵĲ���
		QString _partName;

		//����
		bool _isShow;

		//����ʵ���ϵ����м�����ID
		std::set<int> _geoFaceIDs;
	};
}

