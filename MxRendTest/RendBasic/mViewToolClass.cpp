#include "mViewToolClass.h"
#include <math.h>
#include <algorithm>

namespace MViewBasic
{
	mViewToolClass::mViewToolClass()
	{
	}

	mViewToolClass::~mViewToolClass()
	{
	}
	//工具函数
	QVector3D mViewToolClass::GetGeoCenterPosition(QVector<QVector3D> vertex)
	{
		if (vertex.size() == 0)
		{
			return QVector3D(0, 0, 0);
		}
		float xmin = vertex.at(0).x(), ymin = vertex.at(0).y(), zmin = vertex.at(0).z(), xmax = vertex.at(0).x(), ymax = vertex.at(0).y(), zmax = vertex.at(0).z();

		//找出坐标中的x y z的最大值和最小值(在物体坐标系下)	
		for (int i = 0; i < vertex.size(); i++)
		{
			xmin = std::min(xmin, vertex.at(i).x());
			xmax = std::max(xmax, vertex.at(i).x());
			ymin = std::min(ymin, vertex.at(i).y());
			ymax = std::max(ymax, vertex.at(i).y());
			zmin = std::min(zmin, vertex.at(i).z());
			zmax = std::max(zmax, vertex.at(i).z());

		}
		QVector3D Center = QVector3D((xmin + xmax) / 2, (ymin + ymax) / 2, (zmin + zmax) / 2);
		return Center;
	}
	float mViewToolClass::GetMaxRadius(QVector<QVector3D> vertex, QVector3D center)
	{
		//寻找模型节点与旋转中心的最大距离
		float MaxRadius = 0.0;
		for (int i = 0; i < vertex.size(); i++)
		{
			float lengh_x = (vertex.at(i).x() - center.x());
			float lengh_y = (vertex.at(i).y() - center.y());
			float lengh_z = (vertex.at(i).z() - center.z());
			float radius = sqrt((lengh_x*lengh_x + lengh_y * lengh_y + lengh_z * lengh_z));
			if (radius > MaxRadius)
			{
				MaxRadius = radius;
			}
		}
		return MaxRadius;

	}
	float mViewToolClass::GetMaxRadius(float left, float right, float bottom, float top, float back, float front, QVector3D center)
	{
		float maxlength_x = std::max(abs(right - center.x()), abs(left - center.x()));
		float maxlength_y = std::max(abs(bottom - center.y()), abs(top - center.y()));
		float maxlength_z = std::max(abs(back - center.z()), abs(front - center.z()));
		return sqrt(pow(maxlength_x, 2) + pow(maxlength_y, 2) + pow(maxlength_z, 2));

	}
	float mViewToolClass::GetMaxDistance(QVector<QVector3D> vertex)
	{
		float MaxDistance = 0.0;
		for (int i = 0; i < vertex.size(); i++)
		{
			for (int j = i+1; j < vertex.size(); j++)
			{
				if (vertex[i].distanceToPoint(vertex[j]) > MaxDistance)
				{
					MaxDistance = vertex[i].distanceToPoint(vertex[j]);
				}
			}
		}
		return MaxDistance;
	}
	QVector3D mViewToolClass::NormToModelPosition(QVector3D normPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	{
		//载荷视图中的观察矩阵和投影矩阵
		QVector3D Positon_norm = normPos;
		QVector4D temp = model.inverted()*(view.inverted()*(projection.inverted()*QVector4D(Positon_norm, 1.0)));
		QVector3D Positon_model = QVector3D(temp.x(), temp.y(), temp.z());
		return Positon_model;
	}
	QVector3D mViewToolClass::ModelToNormPosition(QVector3D modelPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	{
		QVector3D Positon_model = modelPos;
		QVector4D temp = projection * (view * (model*QVector4D(Positon_model, 1.0)));
		QVector3D Positon_norm = QVector3D(temp.x(), temp.y(), temp.z());
		return Positon_norm;
	}
	QVector3D mViewToolClass::ModelToViewPosition(QVector3D modelPos, QMatrix4x4 view, QMatrix4x4 model)
	{
		QVector4D temp = (view * (model*QVector4D(modelPos, 1.0)));
		QVector3D Positon_view = QVector3D(temp.x(), temp.y(), temp.z());
		return Positon_view;

	}
	QVector3D mViewToolClass::ModelToModelPosition(QVector3D modelPos1, QMatrix4x4 projection1, QMatrix4x4 view1, QMatrix4x4 model1, QMatrix4x4 projection2, QMatrix4x4 view2, QMatrix4x4 model2)
	{
		QVector3D Positon_norm = ModelToNormPosition(modelPos1, projection1, view1, model1);
		QVector3D Positon_Model2 = NormToModelPosition(Positon_norm, projection2, view2, model2);
		return Positon_Model2;
	}
	QVector3D mViewToolClass::PixelToNormPosition(int Posx, int Posy, int scr_width, int scr_height, float depth)
	{
		float norm_x = (Posx - scr_width / 2.0)*(2.0 / scr_width);
		float norm_y = -(Posy - scr_height / 2.0)*(2.0 / scr_height);
		float norm_z = depth;
		QVector3D normPos = QVector3D(norm_x, norm_y, norm_z);
		return normPos;
	}
	QVector3D mViewToolClass::PixelToModelPosition(int Posx, int Posy, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model, float scr_width, int scr_height,float depth)
	{
		QVector3D normPos = PixelToNormPosition(Posx, Posy, scr_width, scr_height, depth);
		QVector3D modelPos = NormToModelPosition(normPos, projection, view, model);
		return modelPos;
	}
	QVector2D mViewToolClass::PixelToOrthoPosition(int Posx, int Posy, float Left, float Right, float Bottom, float Top, float scr_width, int scr_height)
	{
		float X = ((Right + Left) / 2) + (Posx - scr_width / 2)* (Right - Left) / scr_width;
		float Y = ((Top + Bottom) / 2) - (Posy - scr_height / 2)* (Top - Bottom) / scr_height;
		return QVector2D(X, Y);
	}
	QVector2D mViewToolClass::ModelToPixelPositon(QVector3D modelPos, QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model, float scr_width, int scr_height)
	{
		QVector3D Positon_norm = ModelToNormPosition(modelPos, projection, view, model);
		int X = static_cast<int>(Positon_norm.x()*scr_width / 2 + scr_width / 2);
		int Y = static_cast<int>(-Positon_norm.y()*scr_height / 2 + scr_height / 2);
		return QVector2D(X, Y);
	}
	QVector3D mViewToolClass::getNormVector(QVector3D vec)
	{
		srand(0);

		QVector3D vecn = vec.normalized();
		//得到一个随机向量
		QVector3D randVec = {0,0,1};
		while (vecn == randVec || vecn == -randVec) //重合
		{
			randVec.setX(rand());
			randVec.setY(rand());
			randVec.setZ(rand());
			randVec.normalized();
		}
		//两个向量叉乘求出垂直向量
		QVector3D res = QVector3D::crossProduct(vecn, randVec).normalized();
		return res;
	}

	QVector<QVector3D> mViewToolClass::getFourNormVector(QVector3D vec)
	{
		QVector<QVector3D> res;
		QVector3D oneVec = getNormVector(vec);
		res.push_back(oneVec);
		for (int angle = 90; angle < 360; angle += 90)
		{
			QMatrix4x4 mt;
			mt.setToIdentity();
			mt.rotate(angle, vec);
			//旋转
			QVector4D next = QVector4D(oneVec, 1.0)*mt;
			res.push_back(QVector3D(next.x(), next.y(), next.z()));
		}
		return res;	
	}
}