#pragma once
#include <QString>
#include <QVector3D>
#include <set>
#include "mBasicEnum.h"
#include <float.h>
namespace MBasicFunction
{
	using namespace std;
	struct IDDepthBuffer
	{
		int id;
		float depth;
		IDDepthBuffer()
		{
			id = 0;
			depth = 1;
		}
		void initial()
		{
			id = 0;
			depth = 1;
		}
	};

	struct IDSetDepthBuffer
	{
		set<int> ids;
		float depth;
		IDSetDepthBuffer()
		{
			depth = 1;
		}
		void initial()
		{
			ids.clear();
			depth = 1;
		}
	};

	struct NameDepthBuffer
	{
		QString name;
		float depth;
		NameDepthBuffer()
		{
			name = "";
			depth = 1;
		}
		void initial()
		{
			name = "";
			depth = 1;
		}
	};

	struct VertexOnGeoLineDepthBuffer
	{
		int geoLineID;
		QVector3D vertex;
		float depth;
		VertexOnGeoLineDepthBuffer()
		{
			geoLineID = 0;
			vertex = QVector3D();
			depth = 1;
		}
		void initial()
		{
			geoLineID = 0;
			vertex = QVector3D();
			depth = 1;
		}
	};

	struct VertexOnGeoFaceDepthBuffer
	{
		int geoFaceID;
		QVector3D vertex;
		float depth;
		VertexOnGeoFaceDepthBuffer()
		{
			geoFaceID = 0;
			vertex = QVector3D();
			depth = 1;
		}
		void initial()
		{
			geoFaceID = 0;
			vertex = QVector3D();
			depth = 1;
		}
	};

	struct IDQStringDepthBuffer
	{
		int id;
		QString partName;
		float depth;
		IDQStringDepthBuffer()
		{
			id = 0;
			partName = "";
			depth = 1;
		}
		void initial()
		{
			id = 0;
			partName = "";
			depth = 1;
		}
	};

	//�����ߴ�
	struct ModelSize
	{
		float xmin;
		float ymin;
		float zmin;
		float xmax;
		float ymax;
		float zmax;

		ModelSize()
		{
                        xmin = FLT_MAX;
			ymin = FLT_MAX;
			zmin = FLT_MAX;
			xmax = -FLT_MAX;
			ymax = -FLT_MAX;
			zmax = -FLT_MAX;
		}
		ModelSize(float XMIN, float YMIN, float ZMIN, float XMAX, float YMAX, float ZMAX)
		{
			xmin = XMIN;
			ymin = YMIN;
			zmin = ZMIN;
			xmax = XMAX;
			ymax = YMAX;
			zmax = ZMAX;
		}
	};

	/*
	* �����ļ����������������·��
	*/
	struct ProjectFilePath
	{
		//�����ļ�·��
		QString projPath;

		//�����ļ�
		QString projGeo;

		//����ļ�
		QString projRes;
	};

	/*
	* mx�ļ��ĵ�Ԫ�ṹ����Ϣ
	*/
	struct MxElementMessage
	{
		ElementType elementType;
		QString elementString;
		QString partName;
		QString sectionname;
		qint64 start;
		qint64 end;
		qint64 _globalpartID;
		qint64 _globalmeshFaceID;
		qint64 _globalgeoFaceID;
		qint64 _globalgeoLineID;
	};

	/*
	* ������յĽṹ��
	*/
	struct PostLight
	{
		//��Դ��λ��
		QVector3D lightPosition;

		//���յĲ���
		QVector3D ambient = QVector3D(0.2f, 0.2f, 0.2f);//������
		QVector3D diffuse = QVector3D(0.5f, 0.5f, 0.5f);//�������
		QVector3D specular = QVector3D(0.6f, 0.6f, 0.6f);//�����
		float shiness = 32.0f;//����ָ��
	}; 
}
