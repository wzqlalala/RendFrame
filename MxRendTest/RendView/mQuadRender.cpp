#include "mQuadRender.h"
#include "mBaseRend.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <renderpch.h>
#include "mShaderManage.h"
using namespace mxr;
using namespace std;
namespace MBaseRend
{
	mQuadRender::mQuadRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> root, mBaseRend *baseRend):mBaseRender(app, root, baseRend)
	{
		this->makeCurrent();
		_stateSet = MakeAsset<StateSet>();
		shared_ptr<Shader> shader = mShaderManage::GetInstance()->GetShader("ScreenQuad");
		_stateSet->setShader(shader);
		_stateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_stateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::LINE), 1);
		_stateSet->setUniform(MakeAsset<Uniform>("width", 0));
		_stateSet->setUniform(MakeAsset<Uniform>("height", 0));
		_stateSet->setUniform(MakeAsset<Uniform>("showColor", QVector4D(0,0,0,1)));
		_drawable = MakeAsset<Drawable>();
		_drawable->setStateSet(_stateSet);
		//_parent->addChild(_drawable);

		
		_cameraMode = _baseRend->getCameraOperateMode();
		_pickMode = _baseRend->getCurrentPickMode();
		_multiplyPickMode = _baseRend->getMultiplyPickMode();

	}
	mQuadRender::~mQuadRender()
	{
		if (_drawable)
		{
			_parent->removeChild(_drawable);
		}
	}

	void mQuadRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		_parent->removeChild(_drawable);
		const QVector<QVector2D> poses = _baseRend->getPickPolygonVertexs();
		if (poses.size() < 2)
		{
			_drawable.reset();
			return;
		}
		this->makeCurrent();
		_stateSet->getUniform("width")->SetData(_baseRend->width());
		_stateSet->getUniform("height")->SetData(_baseRend->height());
		QVector<QVector2D> vertexs;
		if (*_cameraMode == CameraOperateMode::Zoom)//画多段开线
		{
			vertexs = poses;
			_stateSet->setDrawMode(GL_LINE_STRIP);
		}
		else if (*_pickMode == PickMode::MultiplyPick)
		{
			if (*_multiplyPickMode == MultiplyPickMode::QuadPick)
			{
				vertexs = QVector<QVector2D>{ poses.first(), QVector2D(poses.first().x(), poses.last().y()), poses.last(), QVector2D(poses.last().x(), poses.first().y()) };
				_stateSet->setDrawMode(GL_LINE_LOOP);
			}
			else if (*_multiplyPickMode == MultiplyPickMode::RoundPick)
			{
				_stateSet->setDrawMode(GL_LINE_LOOP);
				QVector2D roundPoint = poses.last(), roundCenter = (poses.last() + poses.first()) / 2.0;
				float distance = roundCenter.distanceToPoint(roundPoint);
				for (int i = 0; i < 20; i++)
				{
					vertexs.append(QVector2D(roundCenter.x() + distance * cos(2 * i * M_PI / 20.0), roundCenter.y() + distance * sin(2 * i * M_PI / 20.0)));
				}
			}
			else if (*_multiplyPickMode == MultiplyPickMode::PolygonPick)//画多段开线
			{
				vertexs = poses;
				_stateSet->setDrawMode(GL_LINE_STRIP);
			}
		}
		else
		{
			_drawable.reset();
		}
		_drawable = MakeAsset<Drawable>();
		_drawable->setStateSet(_stateSet);
		_drawable->setVertexAttribArray(0, MakeAsset<Vec2Array>(vertexs));
		_parent->addChild(_drawable);
	}

	//void mQuadRender::draw(QVector<QVector2D> poses, int w, int h)
	//{
	//	

	//}




}
