#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QHash>
#include <QVector3D>
#include "Array.h"

#include "SpaceTree.h"

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

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
	class mDataPost1;
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshFaceData1;
	class mPostMeshLineData1;
}
using namespace MViewBasic;
namespace MPostRend
{
	//面网格
	class RENDVIEW_EXPORT mPostPartFaceRender
	{
	public:
		//0 - 原始坐标；1 - 部件颜色；2 - 部件颜色、纹理值0, 1；3 - 变量值；
		//4 - 位移值； 5 - 是否有值，没有值的话显示灰色，有值的话按照IsColor判断；6 - 法向量, 7 -ID
		//QVector3D, GLubyte, GLubyte, float, QVector3D, GLubyte, QVector3D, float
		std::shared_ptr<mxr::Vec3Array>  _vertex0;
		std::shared_ptr<mxr::Vec3Array>  _vertex1;
		std::shared_ptr<mxr::FloatArray> _vertex2;
		std::shared_ptr<mxr::FloatArray> _vertex3;
		std::shared_ptr<mxr::Vec3Array>  _vertex4;
		std::shared_ptr<mxr::FloatArray> _vertex5;
		std::shared_ptr<mxr::Vec3Array>  _vertex6;
		//std::shared_ptr<mxr::FloatArray> _vertex8;
		mPostPartFaceRender(std::shared_ptr<mxr::Group> parent);
		~mPostPartFaceRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;

	};

	//透明的面无变形的面
	class mPostPartFaceTransparentNodeformationRender
	{
	public:
		//meshline stateset
		std::shared_ptr<mxr::Vec3Array>  _vertex0;
		std::shared_ptr<mxr::Vec3Array>  _vertex1;
		mPostPartFaceTransparentNodeformationRender(std::shared_ptr<mxr::Group> parent);
		~mPostPartFaceTransparentNodeformationRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;

	};

	//显示的网格线
	class mPostPartFaceLineRender
	{
	public:
		//meshline stateset
		std::shared_ptr<mxr::Vec3Array>   _vertex0;
		std::shared_ptr<mxr::Vec3Array>   _vertex1;
		std::shared_ptr<mxr::FloatArray>  _vertex2;
		//std::shared_ptr<mxr::FloatArray>  _vertex3;
		mPostPartFaceLineRender(std::shared_ptr<mxr::Group> parent);
		~mPostPartFaceLineRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;

	};

	//线网格
	class RENDVIEW_EXPORT mPostPartLineRender
	{
	public:
		//0 - 原始坐标；1 - 部件颜色；2 - 部件颜色、纹理值0, 1；3 - 变量值；
		//4 - 位移值； 5 - 是否有值，没有值的话显示灰色，有值的话按照IsColor判断
		//QVector3D, GLubyte, GLubyte, float, QVector3D, GLubyte, QVector3D, float
		std::shared_ptr<mxr::Vec3Array>  _vertex0;
		std::shared_ptr<mxr::Vec3Array>  _vertex1;
		std::shared_ptr<mxr::FloatArray> _vertex2;
		std::shared_ptr<mxr::FloatArray> _vertex3;
		std::shared_ptr<mxr::Vec3Array>  _vertex4;
		std::shared_ptr<mxr::FloatArray> _vertex5;
		mPostPartLineRender(std::shared_ptr<mxr::Group> parent);
		~mPostPartLineRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;

	};

	//点网格
	class RENDVIEW_EXPORT mPostPartPointRender
	{
	public:
		//0 - 原始坐标；1 - 部件颜色；2 - 部件颜色、纹理值0, 1；3 - 变量值；
		//4 - 位移值； 5 - 是否有值，没有值的话显示灰色，有值的话按照IsColor判断
		//QVector3D, GLubyte, GLubyte, float, QVector3D, GLubyte, QVector3D, float
		std::shared_ptr<mxr::Vec3Array>  _vertex0;
		std::shared_ptr<mxr::Vec3Array>  _vertex1;
		std::shared_ptr<mxr::FloatArray> _vertex2;
		std::shared_ptr<mxr::FloatArray> _vertex3;
		std::shared_ptr<mxr::Vec3Array>  _vertex4;
		std::shared_ptr<mxr::FloatArray> _vertex5;
		mPostPartPointRender(std::shared_ptr<mxr::Group> parent);
		~mPostPartPointRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;

	};

	//显示的边界线
	class mPostPartEdgeLineRender
	{
	public:
		//meshline stateset
		std::shared_ptr<mxr::Vec3Array>   _vertex0;
		std::shared_ptr<mxr::Vec3Array>   _vertex1;
		std::shared_ptr<mxr::FloatArray>   _vertex2;
		mPostPartEdgeLineRender(std::shared_ptr<mxr::Group> parent);
		~mPostPartEdgeLineRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;

	};

	class RENDVIEW_EXPORT mPostPartRender
	{
	public:

		float _minValue, _maxValue;

		mPostPartRender(std::shared_ptr<mxr::Group> parent, MDataPost::mOneFrameData1 *oneFrameData, MDataPost::mPostMeshPartData1 *part);

		~mPostPartRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		void calculateSpaceTreeThread(const QHash<int, QVector3D> &dis);
		Space::SpaceTree* getPartSpaceTree();
		MDataPost::mPostMeshPartData1 *getpartData() {return _partData; };

		void ImportPart(QVector3D deformationScale, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void UpdatePartVariableValue(QHash<int, float> &values, MViewBasic::NodeOrElement type);

		void setShowFuntion(ShowFuntion showFuntion);
		void setIsShowInitialShape(bool isShowInitialShape);//设置是否显示初始构型的透明图形
		std::shared_ptr<mxr::Drawable> getFaceDrawable() { return _facerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getPointDrawable() { return _pointrend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getFaceTransparentNodeformationDrawable() { return _facetransparentnodeformationrend->getDrawable(); };
		std::shared_ptr<mxr::Geode> getGeode() { return _geode; }

	private:
		void AppendMesh0(MDataPost::mPostMeshData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMesh1(MDataPost::mPostMeshData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMesh2(MDataPost::mPostMeshData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMesh3(MDataPost::mPostMeshFaceData1 *mesh, QHash<int, float> &values, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);
		void AppendMeshLine(MDataPost::mPostMeshLineData1 * mesh, QHash<int, QVector3D> &dis, MViewBasic::NodeOrElement type);

		void GetVariableValue_Mesh0(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
		void GetVariableValue_Mesh1(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
		void GetVariableValue_Mesh2(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshData1 * mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
		void GetVariableValue_Mesh3(QVector<float> &meshvalues, QVector<float> &meshhasvalues, MDataPost::mPostMeshFaceData1 *mesh, QHash<int, float> &values, MViewBasic::NodeOrElement type);
	private:
		shared_ptr<mxr::Group> _parent;//父节点
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		//std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态
		//std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//渲染透明面的状态
		//std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态
		//std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态

		std::shared_ptr<mPostPartFaceRender>  _facerend;//面
		std::shared_ptr<mPostPartFaceTransparentNodeformationRender> _facetransparentnodeformationrend;//变形前透明的面
		std::shared_ptr<mPostPartEdgeLineRender>  _edgelinerend;//边界线
		std::shared_ptr<mPostPartFaceLineRender>  _facelinerend;//面线
		std::shared_ptr<mPostPartLineRender>  _linerend;//线
		std::shared_ptr<mPostPartPointRender>  _pointrend;//点

		QVector3D deformationScale;
		MDataPost::mOneFrameData1 *_oneFrameData;
		MDataPost::mPostMeshPartData1 *_partData;

		Space::SpaceTree *_spaceTree;
	};
}
