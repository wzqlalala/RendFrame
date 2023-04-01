#include "mPostModelRender.h"
#include "mPostPartRender.h"
#include <renderpch.h>
#include "Space.h"

#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostMeshPartData1.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mPostRendStatus.h"

// ”Õº¿‡
#include "mModelView.h"
#include "mCommonView.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>

using namespace mxr;
using namespace std;
using namespace MBasicFunction;
namespace MPostRend
{
	mPostModelRender::mPostModelRender(std::shared_ptr<mxr::Group> parent, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData):
		_parent(parent), _rendStatus(rendStatus), _oneFrameData(oneFrameData), _oneFrameRendData(oneFrameRendData)
	{
		_geode = MakeAsset<Geode>();
		_parent->addChild(_geode);	

		//updateAllModelOperate(ImportOperate);
	}

	void mPostModelRender::setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet)
	{
		_faceStateSet = faceStateSet;
		//_facerend->setStateSet(_faceStateSet);
	}
	void mPostModelRender::setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet)
	{
		_faceTransparentStateSet = faceTransparentStateSet;
	}
	void mPostModelRender::setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet)
	{
		_faceTransparentNodeformationStateSet = faceTransparentNoDeformationStateSet;
	}
	void mPostModelRender::setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet)
	{
		_edgelineStateSet = edgelineStateSet;
		//_meshlinerend->setStateSet(_edgelineStateSet);
	}
	void mPostModelRender::setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet)
	{
		_facelineStateSet = facelineStateSet;
		//_facelinerend->setStateSet(_facelineStateSet);
	}
	void mPostModelRender::setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet)
	{
		_lineStateSet = lineStateSet;
	}
	void mPostModelRender::setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet)
	{
		_pointStateSet = pointStateSet;
	}
	mPostModelRender::~mPostModelRender()
	{
		_parent->removeChild(_geode);
		_partRenders.clear();
	}
	void mPostModelRender::UpdateModelBuffer()
	{
		QHash<int, float> values = _oneFrameRendData->getRendValue();
		QHash<int, QVector3D> dis = _oneFrameRendData->getNodeDisplacementData();
		QVector3D deformationScale = _oneFrameRendData->getDeformationScale();
		std::set<QString> hidePartNames;
		//auto iter = _oneFrameData->getMeshPartIterator();
		_partOrder = _oneFrameData->getAllPartNameList();
		for (auto partName : _partOrder)
		{
			mPostMeshPartData1 *partData = _oneFrameData->getOnePartDataByName(partName);
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				hidePartNames.insert(partName);
			}
			asset_ref<mPostPartRender> part = MakeAsset<mPostPartRender>(_geode, _oneFrameData, partData);
			part->setFaceStateSet(_faceStateSet);
			part->setFaceTransparentNodeformationStateSet(_faceTransparentNodeformationStateSet);
			part->setEdgeLineStateSet(_edgelineStateSet);
			part->setFaceLineStateSet(_facelineStateSet);
			part->setLineStateSet(_lineStateSet);
			part->setPointStateSet(_pointStateSet);
			part->ImportPart(deformationScale, values, dis, _oneFrameRendData->getNodeOrElement());
			part->calculateSpaceTreeThread(dis);

			_partRenders.insert(partName, part);
			_oneFrameRendData->setPartMinMaxData(partName, part->_minValue, part->_maxValue);
		}
		updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(HideOnePartOperate, hidePartNames));
	}
	void mPostModelRender::updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates)
	{
		PostModelOperateEnum postModelOperate = postModelOperates.first;
		set<QString> partNames = postModelOperates.second;
		if (postModelOperate == HideOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->getGeode()->setNodeMask(1);
			}
		}
		else if (postModelOperate == ShowOnePartOperate)
		{
			for (QString partName : partNames)
			{
				//_partRenders[partName]->getGeode()->setNodeMask(0);
				_partRenders[partName]->setShowFuntion(_rendStatus->_showFunction);
				_partRenders[partName]->setIsShowInitialShape(_rendStatus->_isShowInitialShape);
				//parts[partName]->getFaceDrawable()->setNodeMask(0);
			}
			//setShowFuntion(_rendStatus->_showFunction);
			//setIsShowInitialShape(_rendStatus->_isShowInitialShape);
		}
		else if (postModelOperate == ColorOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->setFaceStateSet(_faceStateSet);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(2);
				array->updata(0, array->size() * 4, QVector<float>(array->size(), 0).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(2);
				array->updata(0, array->size() * 4, QVector<float>(array->size(), 0).data());
				//array->updata(0, array->size() * 4, QVector<float>(array->size() * 2, 0).data());
				//array->updata()
				//GLenum error =  QOpenGLContext::currentContext()->functions()->glGetError();
				//qDebug() << error;
			}
		}
		else if (postModelOperate == TextureOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->setFaceStateSet(_faceStateSet);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(2);
				array->updata(0, array->size() * 4, QVector<float>(array->size(), 1).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(2);
				array->updata(0, array->size() * 4, QVector<float>(array->size(), 1).data());
			}
		}
		else if (postModelOperate == TransparentOnePartOperate)
		{
			for (QString partName : partNames)
			{
				_partRenders[partName]->setFaceStateSet(_faceTransparentStateSet);
			}
		}
		else if (postModelOperate == ChangeColorOnePartOperate)
		{
			for (QString partName : partNames)
			{
				QVector3D color = _oneFrameData->getMeshPartColor(partName);
				Array *array = _partRenders[partName]->getFaceDrawable()->getVertexAttribArray(1);
				array->updata(0, array->size() * 12, QVector<QVector3D>(array->size(), color).data());
				array = _partRenders[partName]->getPointDrawable()->getVertexAttribArray(1);
				array->updata(0, array->size() * 12, QVector<QVector3D>(array->size(), color).data());
			}
		}
	}
	void mPostModelRender::updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate)
	{
		std::set<QString> partnames{ _partOrder.begin(),_partOrder.end()};
		if (postModelOperate == ImportOperate)
		{
			UpdateModelBuffer();
			setIsShowInitialShape(_rendStatus->_isShowInitialShape);
			setShowFuntion(_rendStatus->_showFunction);
			//UpdateRigidWallBuffer();
			//updateAllModelOperate(UpdateMinMax);
		}
		else if (postModelOperate == HideAllPartOperate)
		{
			updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(HideOnePartOperate, partnames));
		}
		else if (postModelOperate == ShowAllPartOperate)
		{
			updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(ShowOnePartOperate, partnames));
		}
		else if (postModelOperate == TextureAllPartOperate)
		{
			updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(TextureOnePartOperate, partnames));
		}
		else if (postModelOperate == TransparentAllPartOperate)
		{
			updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(TransparentOnePartOperate, partnames));
		}
		else if (postModelOperate == ColorAllPartOperate)
		{
			updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(ColorOnePartOperate, partnames));
		}
		else if (postModelOperate == UpdateVariableValue)
		{
			QHash<int, float> values = _oneFrameRendData->getRendValue();
			for (auto it : _partRenders)
			{
				it->UpdatePartVariableValue(values, _oneFrameRendData->getNodeOrElement());
			}
		}
		else if (postModelOperate == ChangeColorAllPartOperate)
		{
			updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>>(ColorOnePartOperate, partnames));
		}
		else if (postModelOperate == UpdateMinMax)
		{
			if (_oneFrameRendData == nullptr)
			{
				return;
			}
			float minValue;
			float maxValue;
			if (_rendStatus->_rangeType == 0)
			{
				minValue = _oneFrameRendData->getMinData();
				maxValue = _oneFrameRendData->getMaxData();
				_oneFrameRendData->updateColorTableMinMaxData();
			}
			else
			{
				minValue = FLT_MAX;
				maxValue = -FLT_MAX;
				QHash<QString, QVector2D> ha = _oneFrameRendData->getPartMinMaxData();
				QHashIterator<QString, QVector2D> iter(ha);
				while (iter.hasNext())
				{
					iter.next();
					if (!_oneFrameData->getMeshPartVisual(iter.key()))
					{
						continue;
					}
					minValue = min(minValue, iter.value().x());
					maxValue = max(maxValue, iter.value().y());
				}
				_oneFrameRendData->setCurrentMinMaxData(minValue, maxValue);
			}
		}
		else if (postModelOperate == UpdateNormal)
		{


		}
	}

	//void mPostModelRender::setDistancePlane(std::vector<QVector4D> cutplane)
	//{
	//	if (_faceStateSet)
	//	{
	//		std::vector<GLuint> fs;
	//		for (int i = 0; i < 7; i++)
	//		{
	//			if (i < cutplane.size())
	//			{
	//				_faceStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetData(cutplane[i]);
	//				_faceStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(true);
	//				_edgelineStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetData(cutplane[i]);
	//				_edgelineStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(true);
	//				fs.push_back(i);
	//			}
	//			else
	//			{
	//				_faceStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(false);
	//				_edgelineStateSet->getUniform(QString("plane%1").arg(i).toStdString())->SetEnable(false);
	//			}
	//		}
	//		_faceStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(fs), 1);
	//		_edgelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(fs), 1);
	//	}
	//}

	//void mPostModelRender::setModelView(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	//{
	//	if (_faceStateSet)
	//	{
	//		//updateAllModelOperate(ColorAllPartOperate);
	//		_faceStateSet->getUniform("projection")->SetData(projection);
	//		_faceStateSet->getUniform("view")->SetData(view);
	//		_faceStateSet->getUniform("model")->SetData(model);
	//		_edgelineStateSet->getUniform("projection")->SetData(projection);
	//		_edgelineStateSet->getUniform("view")->SetData(view);
	//		_edgelineStateSet->getUniform("model")->SetData(model);
	//	}
	//}

	void mPostModelRender::setDeformationScale(QVector3D deformationScale)
	{
		if (_faceStateSet)
		{
			_faceStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_faceTransparentStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_edgelineStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_facelineStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_lineStateSet->getUniform("deformationScale")->SetData(deformationScale);
			_pointStateSet->getUniform("deformationScale")->SetData(deformationScale);
		}
	}

	//void mPostModelRender::setLight(PostLight light, QVector3D viewpos, int lightIsOn)
	//{
	//	if (_faceStateSet)
	//	{
	//		_faceStateSet->getUniform("light.position")->SetData(light.lightPosition);
	//		_faceStateSet->getUniform("light.ambient")->SetData(light.ambient);
	//		_faceStateSet->getUniform("light.diffuse")->SetData(light.diffuse);
	//		_faceStateSet->getUniform("light.specular")->SetData(light.specular);
	//		_faceStateSet->getUniform("light.shiness")->SetData(light.shiness);
	//		_faceStateSet->getUniform("viewPos")->SetData(viewpos);
	//		_faceStateSet->getUniform("lightIsOn")->SetData(lightIsOn);

	//	}
	//}

	void mPostModelRender::setTextureArgument(int isEquivariance, float minValue, float maxValue, float textureCoordRatio)
	{
		if (_faceStateSet)
		{
			_faceStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_faceStateSet->getUniform("minValue")->SetData(minValue);
			_faceStateSet->getUniform("maxValue")->SetData(maxValue);
			_faceStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
			_lineStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_lineStateSet->getUniform("minValue")->SetData(minValue);
			_lineStateSet->getUniform("maxValue")->SetData(maxValue);
			_lineStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
			_pointStateSet->getUniform("isEquivariance")->SetData(isEquivariance);
			_pointStateSet->getUniform("minValue")->SetData(minValue);
			_pointStateSet->getUniform("maxValue")->SetData(maxValue);
			_pointStateSet->getUniform("textureCoordRatio")->SetData(textureCoordRatio);
		}
	}
	void mPostModelRender::setShowFuntion(ShowFuntion showFuntion)
	{
		if (_faceStateSet)
		{
			for (auto iter : _partRenders)
			{
				iter->setShowFuntion(showFuntion);
			}
		}
	}
	void mPostModelRender::setIsShowInitialShape(bool isShowInitialShape)
	{

		for (auto iter : _partRenders)
		{
			iter->setIsShowInitialShape(isShowInitialShape);
		}
		
	}

	Space::AABB mPostModelRender::getModelAABB()
	{
		Space::AABB aabb;
		for(auto partrend : _partRenders)
		{
			aabb.push(partrend->getPartSpaceTree()->space);
		}
		return aabb;
	}
}