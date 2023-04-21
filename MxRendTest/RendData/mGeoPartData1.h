#pragma once
#include "renddata_global.h"
#include <QVector3D>
#include <set>
#include <QString>

#include "mBasicStructor.h"
#include "SpaceTree.h"

namespace MDataGeo
{
	class RENDDATA_EXPORT mGeoPartData1
	{

	public:
		mGeoPartData1();

		mGeoPartData1(QString name,int ID);

		~mGeoPartData1();

		//���ò����ߴ����ֵ
		void setGeoPartSize(Space::AABB partSize);

		//���ò�������ɫ
		void setPartColor(QVector3D color);

		//���ò�������
		void setPartVisual(bool isshow);

		//���ò���������
		void setGeoShapeType(int shapeType);

		//��Ӽ��ε�ID
		void appendGeoPointID(int geoPointID);

		//��Ӽ�����ID
		void appendGeoLineID(int geoLineID);

		//��Ӽ�����ID
		void appendGeoFaceID(int geoFaceID);

		//��Ӽ���ʵ��ID
		void appendGeoSolidID(int geoSolidID);

		//��ȡ��������
		int getGeoShapeType();

		//��ȡ���еļ��ε�ID
		std::set<int> getGeoPointIDs();

		//��ȡ���еļ�����ID
		std::set<int> getGeoLineIDs();

		//��ȡ���еļ�����ID
		std::set<int> getGeoFaceIDs();

		//��ȡ���м���ʵ��ID
		std::set<int> getGeoSolidIDs();

		//��ȡ��������ɫ
		QVector3D getPartColor();

		//�жϲ����Ƿ���ʾ
		bool getPartVisual();

		//��ȡ�������
		int getPartID();

		//��ȡ�����ߴ����ֵ
		Space::AABB getGeoPartSize();

	private:

		static QList<QVector3D> Color;

		//��������
		QString _partName;
		int _partID;
		bool _isShow;
		QVector3D _partColor;
		int _shapeType;//0Ϊ��ϣ�1Ϊ��Solid��2Ϊ��Solid��4ΪFace��6ΪEdge��Line����7ΪVertex��Vertex��

		//�����ļ��ε���Ϣ
		std::set<int> _geoPointIDs;

		//�����ļ�������Ϣ
		std::set<int> _geoLineIDs;

		//�����ļ�������Ϣ
		std::set<int> _geoFaceIDs;

		//�����ļ�������Ϣ
		std::set<int> _geoSolidIDs;

		//�����ļ��γߴ�
		Space::AABB _partSize;

	};
}

