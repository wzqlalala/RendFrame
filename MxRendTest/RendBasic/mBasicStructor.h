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
			depth = FLT_MAX;
		}
		void initial()
		{
			id = 0;
			depth = FLT_MAX;
		}
	};

	struct IDSetDepthBuffer
	{
		set<int> ids;
		float depth;
		IDSetDepthBuffer()
		{
			depth = FLT_MAX;
		}
		void initial()
		{
			ids.clear();
			depth = FLT_MAX;
		}
	};

	struct NameDepthBuffer
	{
		QString name;
		float depth;
		NameDepthBuffer()
		{
			name = "";
			depth = FLT_MAX;
		}
		void initial()
		{
			name = "";
			depth = FLT_MAX;
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
			depth = FLT_MAX;
		}
		void initial()
		{
			geoLineID = 0;
			vertex = QVector3D();
			depth = FLT_MAX;
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
			depth = FLT_MAX;
		}
		void initial()
		{
			geoFaceID = 0;
			vertex = QVector3D();
			depth = FLT_MAX;
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
			depth = FLT_MAX;
		}
		void initial()
		{
			id = 0;
			partName = "";
			depth = FLT_MAX;
		}
	};

	//部件尺寸
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
	* 工程文件自身及其包含的三个路径
	*/
	struct ProjectFilePath
	{
		//工程文件路径
		QString projPath;

		//几何文件
		QString projGeo;

		//结果文件
		QString projRes;
	};

	/*
	* mx文件的单元结构体信息
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
	* 后处理光照的结构体
	*/
	struct PostLight
	{
		//光源的位置
		QVector3D lightPosition;

		//光照的参数
		QVector3D ambient = QVector3D(0.2f, 0.2f, 0.2f);//环境光
		QVector3D diffuse = QVector3D(0.5f, 0.5f, 0.5f);//漫反射光
		QVector3D specular = QVector3D(0.6f, 0.6f, 0.6f);//镜面光
		float shiness = 32.0f;//镜面指数
	}; 
}
