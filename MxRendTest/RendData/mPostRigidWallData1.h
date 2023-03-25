#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

//MBasicFunction
#include "mPostEnum.h"
#include "mBasicStructor.h"

//Qt
#include <QVector3D>
#include <QVector>

using namespace MBasicFunction;
namespace MDataPost
{
	class RENDDATA_EXPORT mPostRigidWallDataBase
	{
	public:

		/*
		* 构造函数
		*/
		mPostRigidWallDataBase(int ID, PostRigidWallType rigidWallType, double *normalStartEndPos);

		/*
		* 析构函数
		*/
		~mPostRigidWallDataBase() {};

		/*
		* 获取刚性墙ID
		*/
		int getRigidWallID();

		/*
		* 获取刚性墙类型
		*/
		PostRigidWallType getRigidWallType();

		/*
		* 获取刚性墙法线起始坐标
		*/
		QVector3D getRigidWallStartPos();

		/*
		* 获取刚性墙法线终止坐标
		*/
		QVector3D getRigidWallEndPos();

		/*
		* 设置刚性墙的三角形面坐标
		*/
		void setFaceVertex(QVector<QVector3D> vertexs);

		/*
		* 获取刚性墙的三角形面坐标
		*/
		virtual QVector<QVector3D> getFaceVertex() { return QVector<QVector3D>(); };

		/*
		* 获取刚性墙的边界线坐标
		*/
		virtual QVector<QVector3D> getLineVertex() { return QVector<QVector3D>(); };

		/*
		* 获取刚性墙的尺寸
		*/
		virtual ModelSize getRigidWallSize() { return _modelSize; };

	protected:

		//计算刚性墙的原始包围盒
		void caculateRigidWallSize();

	protected:

		//刚性墙ID
		int _rigidWallID;

		//刚性墙类型
		PostRigidWallType _rigidWallType;

		//法线起始坐标
		QVector3D _normalStartPos;

		//法线终止坐标
		QVector3D _normalEndPos;

		//边界线坐标
		QVector<QVector3D> _lineVertexs;

		//表面三角形坐标
		QVector<QVector3D> _faceVertexs;

		//刚性墙尺寸
		ModelSize _modelSize;
	};

	class RENDDATA_EXPORT mPostInfinitelyLargePlaneRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* 构造函数
		*/
		mPostInfinitelyLargePlaneRigidWall(int ID, double *normalStartEndPos);

		/*
		* 析构函数
		*/
		~mPostInfinitelyLargePlaneRigidWall() {};

		//获取面坐标
		QVector<QVector3D> getFaceVertex();

		//获取线坐标
		QVector<QVector3D> getLineVertex();

	};

	class RENDDATA_EXPORT mPostLimitedLargePlaneRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* 构造函数
		*/
		mPostLimitedLargePlaneRigidWall(int ID, double *normalStartEndPos);

		/*
		* 析构函数
		*/
		~mPostLimitedLargePlaneRigidWall() {};

		//获取面坐标
		QVector<QVector3D> getFaceVertex();

		//获取线坐标
		QVector<QVector3D> getLineVertex();

		//获取刚性墙的尺寸
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

	public:
		//刚性墙边向量终止坐标
		double _lineEndVector[3];

		//边长L
		double _L;

		//边长M
		double _M;

	};

	class RENDDATA_EXPORT mPostRectangularRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* 构造函数
		*/
		mPostRectangularRigidWall(int ID, double *normalStartEndPos);

		/*
		* 析构函数
		*/
		~mPostRectangularRigidWall() {};

		//获取面坐标
		QVector<QVector3D> getFaceVertex();

		//获取线坐标
		QVector<QVector3D> getLineVertex();

		//获取刚性墙的尺寸
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

	public:
		//刚性墙边向量终止坐标
		double _lineEndVector[3];

		//边长L
		double _L;

		//边长M
		double _M;

		//边长P
		double _P;

	};

	class RENDDATA_EXPORT mPostCylindricalRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* 构造函数
		*/
		mPostCylindricalRigidWall(int ID, double *normalStartEndPos);

		/*
		* 析构函数
		*/
		~mPostCylindricalRigidWall() {};

		//获取面坐标
		QVector<QVector3D> getFaceVertex();

		//获取线坐标
		QVector<QVector3D> getLineVertex();

		//获取刚性墙的尺寸
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

	public:
		//半径
		double _radius;

		//轴向长度
		double _length;

	};

	class RENDDATA_EXPORT mPostSphericalRigidWall : public mPostRigidWallDataBase
	{
	public:
		/*
		* 构造函数
		*/
		mPostSphericalRigidWall(int ID, double *normalStartEndPos);

		/*
		* 析构函数
		*/
		~mPostSphericalRigidWall() {};

		//获取面坐标
		QVector<QVector3D> getFaceVertex();

		//获取线坐标
		QVector<QVector3D> getLineVertex();

		//获取刚性墙的尺寸
		ModelSize getRigidWallSize();

	private:

		void calculateVertex();

		//
		float distSquare(QVector3D a, QVector3D b);

		//计算角平分线和球面的交点
		QVector3D midArcPoint(QVector3D a,QVector3D b);

	public:
		//半径
		double _radius;

	};

}
