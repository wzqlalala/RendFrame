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
#include <set>
#include <QString>

#include "SpaceTree.h"

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
using namespace MBasicFunction;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostPartRender;
	class RENDVIEW_EXPORT mPostModelRender
	{
	public:

		mPostModelRender(shared_ptr<mxr::Group> parent, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData);

		~mPostModelRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		//更新缓存
		void UpdateModelBuffer();

		//更新单个部件操作
		void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//更新全部部件操作
		void updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate);

		//void setDistancePlane(std::vector<QVector4D> cutplane);
		//void setModelView(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
		void setDeformationScale(QVector3D deformationScale);

		//void setLight(PostLight light, QVector3D viewpos, int lightIsOn);
		void setTextureArgument(int isEquivariance, float minValue, float maxValue, float textureCoordRatio);


		void setShowFuntion(ShowFuntion showFuntion);
		
		//设置是否显示初始构型的透明图形
		void setIsShowInitialShape(bool isShowInitialShape);

		Space::AABB getModelAABB();

		//void append

		//void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);

	private:
		shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;
		mPostOneFrameRendData *_oneFrameRendData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态

		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//渲染透明面的状态

		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//渲染模型透明面无变形的状态

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态

		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态

		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态

		//零件的顺序
		QList<QString> _partOrder;
		QHash<QString, std::shared_ptr<mPostPartRender>> _partRenders;
	};
}
