#include "mPreRend.h"
#include "mPreRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"

#include "mViewBase.h"

////MxRender
#include <renderpch.h>

#include <math.h>

namespace MPreRend
{

	mPreRend::mPreRend(const QString& name):mBaseRend( name)
	{
		qDebug() << "Pre Struct";
	}

	void mPreRend::initializeGL()
	{
		mBaseRend::initializeGL();

		makeCurrent();

		shared_ptr<mPreRender> preRender = make_shared<mPreRender>(_app, _root, this);
		this->addRender(preRender);

		_fontRender->appendGloabalAxisFont();
		_arrowRender->appendGloabalAxisArrow();

		qDebug() << "Pre Initial";
	}

	void mPreRend::paintGL()
	{
		mBaseRend::paintGL();
	}

	void mPreRend::resizeGL(int w, int h)
	{
		mBaseRend::resizeGL(w, h);
	}
	void mPreRend::mousePressEvent(QMouseEvent *event)
	{
		mBaseRend::mousePressEvent(event);
	}
	void mPreRend::mouseReleaseEvent(QMouseEvent *event)
	{
		mBaseRend::mouseReleaseEvent(event);
	}
	void mPreRend::mouseMoveEvent(QMouseEvent *event)
	{
		mBaseRend::mouseMoveEvent(event);
	}
	void mPreRend::wheelEvent(QWheelEvent *event)
	{
		mBaseRend::wheelEvent(event);
	}

	mPreRend::~mPreRend()
	{
		qDebug() << "Pre Distruct";
	}
	shared_ptr<mPreRender> mPreRend::getPreRender()
	{
		for (auto render : _renderArray)
		{
			auto pre = dynamic_pointer_cast<mPreRender>(render);
			if (pre)
			{
				return pre;
			}
		}
		return nullptr;
	}
	void mPreRend::GetModelSizePara(bool isModelCenter)
	{
		Space::AABB aabb;
		if (!this->getPreRender()->getModelAABB(aabb))
		{
			aabb = Space::AABB();
		}

		//额外顶点数据
		//QVector<QVector3D> ExtraVertex = mTempHighLightData1::getInstance()->getHighLightPointVertex() + MDataMesh::mMeshModelData1::getInstance()->getTempNodeVertex()
		//	+ _lableRend_pre->getLableDataController()->getExtraVertex() + _lableRend_common->getLableDataController()->getExtraVertex();
		//if (!ExtraVertex.isEmpty())
		//{
		//	float left_extra = 0, right_extra = 0, bottom_extra = 0, top_extra = 0, back_extra = 0, front_extra = 0;
		//	//获取额外顶点数据的最大参数
		//	left_extra = right_extra = ExtraVertex.at(0).x();
		//	bottom_extra = top_extra = ExtraVertex.at(0).y();
		//	back_extra = front_extra = ExtraVertex.at(0).z();
		//	for (int i = 0; i < ExtraVertex.size(); i++)
		//	{
		//		left_extra = min(left_extra, ExtraVertex.at(i).x());
		//		right_extra = max(right_extra, ExtraVertex.at(i).x());
		//		bottom_extra = min(bottom_extra, ExtraVertex.at(i).y());
		//		top_extra = max(top_extra, ExtraVertex.at(i).y());
		//		back_extra = min(back_extra, ExtraVertex.at(i).z());
		//		front_extra = max(front_extra, ExtraVertex.at(i).z());
		//	}

		//	//获取最终的最大参数
		//	_left = min(left_mesh, left_extra);
		//	_right = max(right_mesh, right_extra);
		//	_bottom = min(bottom_mesh, bottom_extra);
		//	_top = max(top_mesh, top_extra);
		//	_back = min(back_mesh, back_extra);
		//	_front = max(front_mesh, front_extra);
		//}
		//else
		//{
		//	//获取最终的最大参数
		//	_left = left_mesh;
		//	_right = right_mesh;
		//	_bottom = bottom_mesh;
		//	_top = top_mesh;
		//	_back = back_mesh;
		//	_front = front_mesh;
		//}
		_aabb = aabb;
		//模型中心
		_center_model = (_aabb.maxEdge + _aabb.minEdge)/2.0;
		_maxRadius_model = _aabb.maxEdge.distanceToPoint(_aabb.minEdge);
		if (isinf(_maxRadius_model))
		{
			_maxRadius_model = _maxRadius_now;
		}
		if (isModelCenter)
		{
			_center_now = _center_model;
			_maxRadius_now = _maxRadius_model;
		}
		else
		{
			float t = _maxRadius_now;
			_maxRadius_now = _aabb.maxEdge.distanceToPoint(_center_now);
			if (isinf(_maxRadius_now))
			{
				_maxRadius_now = t;
			}
		}
	}
	void mPreRend::slotResetOrthoAndCamera()
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_model, _maxRadius_now);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		update();
	}

	void mPreRend::slotUpdateOrthoAndCamera()
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_model, _maxRadius_now);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		//HideOrShowAllFont();
		update();

	}

	void mPreRend::slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex)
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_now, addVertex);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		//HideOrShowAllFont();
		update();
	}
}