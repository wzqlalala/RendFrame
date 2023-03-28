#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "rendview_global.h"

#include <memory>

#include <QVector3D>
#include <QVector4D>
#include <QVector>

#include "mMeshViewEnum.h"



namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mCommonView;
}
namespace MDataPost
{
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace std;
namespace MPostRend
{
	class RENDVIEW_EXPORT mPostCuttingPlaneData 
	{
	public:
		mPostCuttingPlaneData(mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData = nullptr, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);
		~mPostCuttingPlaneData();

		void reverseCuttingPlaneNormal();

		void setCuttingData(QVector3D normal, QVector3D vertex);

		QVector4D getCuttingPlane();

		QVector<QVector3D> getTriVertex();

		QVector<float> getTriTexture();

		QVector<QVector3D> getTriVectorValue();

		void setPlaneData(QVector3D normal, QVector3D centervertex, float maxR);

		QVector3D getCuttingPlaneVertex();

		QVector3D getCuttingPlaneNormal();

		/*
		* 获得切面图上的点坐标和矢量值，没有排序也没有三角化（和切面图上的点不对应）
		*/
		QPair<QVector<QVector3D>, QVector<QVector3D>> getCuttingNodeData();

		/*
		* 通过指定标量变量进行切面计算
		*/
		QPair<QVector<QVector3D>, QVector<double>> getCuttingNodeData(QHash<int, double> values, MViewBasic::NodeOrElement nodeOrElement);

		/*
		* 通过指定矢量变量进行切面计算
		*/
		QPair<QVector<QVector3D>, QVector<QVector3D>> getCuttingNodeData(QHash<int, QVector3D> vectorValues, MViewBasic::NodeOrElement nodeOrElement);

		//是否切
		bool isCutting;
		//平面数据
		float theta;//角度
		QVector3D axis;//旋转轴
		QVector3D centerVertex;//中心点
		QVector<QVector3D> _planeVertex;//平面四个点的坐标
	private:

		//计算剪切平面
		void caculateCuttingPlaneData();

		//对于节点标量，判断空间直线和平面的交点，vertex代表直线的两个端点，value代表两个端点的值，isInVertex代表相交的点或者重合的两个端点
		template <class T>
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<T> value, QVector<QPair<QVector3D, T>> &isInVertex)
		{
			//平面法向量
			QVector3D vp = _normal;

			//平面上任意一点
			QVector3D n = _vertex;

			//线段方向向量
			QVector3D v = vertex.last() - vertex.first();

			//线段起始点
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			T t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				QVector3D jiaodian = m + v * t;
				T jiaodianvalue = value.first() + (value.last() - value.first()) * t;
				isInVertex.append(QPair<QVector3D, T>{jiaodian, jiaodianvalue});
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains({ vertex.first(),value.first() }))
				{
					isInVertex.append({ vertex.first(),value.first() });
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains({ vertex.last(),value.last() }))
				{
					isInVertex.append({ vertex.last(),value.last() });
				}
			}
			return false;
		};

		//对于节点矢量，判断空间直线和平面的交点，vertex代表直线的两个端点，value代表两个端点的值,vectorValue代表两个端点的矢量值，isInVertex代表相交的点或者重合的两个端点（计算标量值）
		template <class T>
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<T> value, QVector<QVector3D> vectorValue, QVector<QPair<QVector3D, T>> &isInVertex, QVector<QVector3D> &isInvectorValue)
		{
			//平面法向量
			QVector3D vp = _normal;

			//平面上任意一点
			QVector3D n = _vertex;

			//线段方向向量
			QVector3D v = vertex.last() - vertex.first();

			//线段起始点
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			T t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				QVector3D jiaodian = m + v * t;
				T jiaodianvalue = value.first() + (value.last() - value.first()) * t;
				QVector3D jiaodianvector = vectorValue.first() + (vectorValue.last() - vectorValue.first()) * t;

				isInVertex.append(QPair<QVector3D, T>{jiaodian, jiaodianvalue});
				isInvectorValue.append(jiaodianvector);
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains({ vertex.first(),value.first() }))
				{
					isInVertex.append({ vertex.first(),value.first() });
					isInvectorValue.append(vectorValue.first());
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains({ vertex.last(),value.last() }))
				{
					isInVertex.append({ vertex.last(),value.last() });
					isInvectorValue.append(vectorValue.last());
				}
			}
			return false;
		};

		//对于节点矢量，判断空间直线和平面的交点，vertex代表直线的两个端点，vectorValue代表两个端点的矢量值，isInVertex代表相交的点或者重合的两个端点（不计算标量值）
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<QVector3D> vectorValue, QVector<QVector3D> &isInVertex, QVector<QVector3D> &isInvectorValue)
		{
			//平面法向量
			QVector3D vp = _normal;

			//平面上任意一点
			QVector3D n = _vertex;

			//线段方向向量
			QVector3D v = vertex.last() - vertex.first();

			//线段起始点
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			float t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				QVector3D jiaodian = m + v * t;
				QVector3D jiaodianvector = vectorValue.first() + (vectorValue.last() - vectorValue.first()) * t;

				isInVertex.append(jiaodian);
				isInvectorValue.append(jiaodianvector);
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains(vertex.first()))
				{
					isInVertex.append(vertex.first());
					isInvectorValue.append(vectorValue.first());
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains(vertex.last()))
				{
					isInVertex.append(vertex.last());
					isInvectorValue.append(vectorValue.last());
				}
			}

			return false;
		};

		//对于单元标量，判断空间直线和平面的交点，vertex代表直线的两个端点，isInVertex代表相交的点或者重合的两个端点
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<QVector3D> &isInVertex)
		{
			QVector3D jiaodian;

			//平面法向量
			QVector3D vp = _normal;

			//平面上任意一点
			QVector3D n = _vertex;

			//线段方向向量
			QVector3D v = vertex.last() - vertex.first();

			//线段起始点
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			float t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				jiaodian = m + v * t;
				isInVertex.append(jiaodian);
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains(vertex.first()))
				{
					isInVertex.append(vertex.first());
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains(vertex.last()))
				{
					isInVertex.append(vertex.last());
				}
			}
			return false;
		};

		//计算多边形的顶点顺序，a和b代表多边形的两个点
		bool judgeVertexOrder(QVector3D a, QVector3D b, QVector3D center, QVector3D OO)
		{
			QVector3D OA = a - center;
			QVector3D OB = b - center;
			//向量OA和向量OB的叉积
			QVector3D voa = QVector3D::crossProduct(a - center, OO);
			QVector3D vob = QVector3D::crossProduct(b - center, OO);

			float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
			float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

			if (QVector3D::dotProduct(voa, _normal) < 0)
			{
				cosA = -(cosA + 2);
			}
			if (QVector3D::dotProduct(vob, _normal) < 0)
			{
				cosB = -(cosB + 2);
			}
			return cosA < cosB;
		};
	private:
		mPostOneFrameRendData *_currentFrameRendData;
		bool _isInitial;

		//判断是否计算矢量数据
		bool _hasVector;

		QVector4D _cuttingPlane;
		QVector3D _normal;
		QVector3D _vertex;

		//三角形
		QVector<QVector3D> _triPlane;
		QVector<float> _triValue;
		QVector<float> _triTexture;
		QVector<QVector3D> _triVectorValue;
		QVector<float> _triMeshID;
		//模型数据
		MDataPost::mOneFrameData1* _oneFrameData;

	};
	class RENDVIEW_EXPORT mPostCuttingPlaneRender
	{
	public:

		mPostCuttingPlaneRender(shared_ptr<mxr::Group> parent, std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet);

		~mPostCuttingPlaneRender();

		//切面图的序号，法向量，点，是否计算矢量值
		void createCuttingPlane(mOneFrameData1 *oneFramData, mPostOneFrameRendData* currentFrameRendData,int index, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);
		void setPlaneData(QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);
		QVector4D getCuttingPlane();
		void setIsShowCuttingPlane(bool isShow);
		void reverseCuttingPlaneNormal();
		void updateCuttingPlaneIndex(int index);

	private:
		void UpdateCuttingPlane(int index);

	private:

		mPostCuttingPlaneData* _postCuttingPlaneData;
		std::shared_ptr<mxr::Group> _parent;

		//切面图
		std::shared_ptr<mxr::Drawable> _cuttingPlaneDrawable;
		std::shared_ptr<mxr::StateSet> _cuttingPlaneStateSet;

		//透明平面
		std::shared_ptr<mxr::Drawable> _transparentPlaneDrawable;
		std::shared_ptr<mxr::StateSet> _transparentPlaneStateSet;

		bool _isShowCuttingPlane;
	};
}
