#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
//#define TINYOBJLOADER_IMPLEMENTATION
#include <QObject>
#include <set>

#include "mBasicEnum.h"
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
	class Viewer;
}
namespace MViewBasic
{
	class mModelView;
	class mCommonView;
}
namespace MDataPost
{
	class mDataPost1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostModelRender;
	class mPostCuttingPlaneRender;
	class RENDVIEW_EXPORT mPostOneFrameRender
	{
	public:

		mPostOneFrameRender(shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData);

		~mPostOneFrameRender();

		void bufferThisFrame();

		void deleteThieFrame();

		mPostOneFrameRendData *getOneFrameRendData() { return _oneFrameRendData; };

		std::shared_ptr<mPostModelRender> getModelRender() {return _modelRender;};

		void updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView);

		//void UpdateModelBuffer();

		//更新单个部件操作
		void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//更新全部部件操作
		void updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate);

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		void setTexture(std::shared_ptr<mxr::Texture> texture);

		//设置变形倍数	
		void setDeformationScale(QVector3D deformationScale);

		//设置纹理坐标系数	
		void setTextureCoordScale(float textureCoordScale);

		//设置最大最小值	
		void setMinMaxData(float maxValue, float minValue);

		/*********************************切面图*******************************************/

		//删除切面图	
		bool deleteCuttingPlane(int num);

		//切面图法向量取反
		void reverseCuttingPlaneNormal(int num);

		//仅仅只显示切面的图，不显示其他单元的云图
		void setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane);

		//设置是否开启某一个切面或者切割图
		void setIsShowCuttingPlane(int num, bool isShow);

		// 生成切面图
		bool createCuttingPlane(std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet, int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);

		// 实时传递切割的那个平面信息
		void setPlaneData(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);

		//设置是否实时显示平面
		void setIsShowPlane(bool isShow);

	private:

		void initial();
	private:
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;//模型数据
		mPostOneFrameRendData *_oneFrameRendData;//当前帧变量数据

		std::shared_ptr<mPostModelRender> _modelRender;	//模型渲染

		QVector<std::shared_ptr<mPostCuttingPlaneRender>> _cuttingPlaneRenders;//切面渲染

		std::shared_ptr<mxr::Texture> _texture;//颜色表纹理

		/*********************************模型*****************************************************/
		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态
		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//渲染透明面的状态
		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//渲染模型透明面无变形的状态
		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态
		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态
		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态
		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		/***********************************切面******************************************************/
		std::shared_ptr<mxr::StateSet> _cuttingPlaneStateSet;//渲染面的状态
		std::shared_ptr<mxr::StateSet> _transparentPlaneStateSet;//渲染透明面的状态

		/*************************************当前帧渲染********************************************************/
		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
