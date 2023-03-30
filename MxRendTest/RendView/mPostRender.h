#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QTimer>

//MViewBasic
#include "mMeshViewEnum.h"

//MBasicFunction
#include "mBasicEnum.h"
#include "mBasicStructor.h"

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Texture;
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
	class mPostAnimationRendData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace MBasicFunction;
using namespace MBaseRend;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostOneFrameRender;
	class mPostAnimationRender;
	class RENDVIEW_EXPORT mPostRender :public mBaseRender
	{
		Q_OBJECT
	public:

		mPostRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent);

		void setPostData(shared_ptr<mDataPost1> dataPost) { _dataPost = dataPost; };

		shared_ptr<mPostOneFrameRender> getOneFrameRender() { return  _oneFrameRender; };

		//更新单个部件操作
		void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//更新全部部件操作
		void updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate);

		//设置后处理渲染模式
		void setPostMode(PostMode postMode);
		
		//清除全部的渲染
		void clearRender();

		//设置当前帧的渲染数据
		void setRendCurrentFrameData(mPostOneFrameRendData *postOneFrameRendData);
		
		//设置当前的显示模式	
		void setShowFuntion(ShowFuntion shoFuntion);
		
		//设置离散开关	
		void setDispersed(bool isdispersed);
		
		//设置变形倍数	
		void setDeformationScale(QVector3D deformationScale);

		//设置是否显示初始构型的透明图形
		void setIsShowInitialShape(bool isShowInitialShape);
	
		//设置纹理坐标系数	
		void setTextureCoordScale(float textureCoordScale);
	
		//设置最大最小值	
		void setMinMaxData(float maxValue, float minValue);
	
		//设置渲染范围(0-所有部件（自定义）；1-当前渲染部件)	
		void setRangeType(int index);

		//设置离散类型，是否等差	
		void setDispersIsEquivariance(bool isEquivariance);

		//设置点的大小
		void setPointSize(int size);

		//设置边界线的宽度
		void setEdgeLineWidth(float size);

		//设置线的颜色
		void setFaceLineColor(QVector4D color);

		/*********************************切面图*******************************************/
		
		//删除切面图	
		void deleteCuttingPlane(int num);
	
		//切面图法向量取反
		void reverseCuttingPlaneNormal(int num);

		//仅仅只显示切面的图，不显示其他单元的云图
		void setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane);


		//设置是否开启某一个切面或者切割图
		void setIsShowCuttingPlane(int num, bool isShow);

		// 生成切面图
		void createCuttingPlane(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);

		// 实时传递切割的那个平面信息
		void setPlaneData(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);

		//设置是否实时显示平面
		void setIsShowPlane(bool isShow);

		/**********************************动画*********************************************/

		//设置瞬态动画的渲染数据	
		void setRendAnimationFrame(mPostAnimationRendData *allFrameRendData);

		//生成单帧动画
		void createLinearAnimation(PostMode postMode);

		//删除动画
		void deleteAnimation();
		
		//控制计时器开关		
		void setTimerOn(bool ison);


		~mPostRender();

		void updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView) override;

	private:

		//更新切面的uniform值
		void updateCuttingPlaneUniform();

		//
	private slots:

		//计时器更新	
		void slot_aniTimer();

	private:
		shared_ptr<mDataPost1> _dataPost;

		shared_ptr<mPostRendStatus> _rendStatus;

		//shared_ptr<mPostAnimationRender> _animationRender;//动画帧

		//动画帧的渲染
		QHash<int, shared_ptr<mPostOneFrameRender>> _animationRender;//动画帧

		shared_ptr<mPostOneFrameRender> _oneFrameRender;//单帧

		shared_ptr<mxr::Texture> _texture;

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

		//计时器
		QTimer* _aniTimer;

		//动画渲染帧的当前id;
		int _animationId;
		
	};
}
