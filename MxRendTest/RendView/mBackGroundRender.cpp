#include "mBackGroundRender.h"

#include <qguiapplication.h>
#include <renderpch.h>
#include "mShaderManage.h"

using namespace mxr;
using namespace MViewBasic;
namespace MBaseRend
{
	float id = 1;
	mBackGroundRender::mBackGroundRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> root)
	{
		QOpenGLContext *context = app->GLContext();
		QSurface *surface = context->surface();
		context->makeCurrent(surface);
		//context = QOpenGLContext::currentContext();
		qDebug() << "mBaseRend::paintGL()" << QString::number(long long int(context), 16);

		_backGroundMode = GradientL_R;
		_backGroundVertex = QVector<QVector2D>{ QVector2D(-1,-1),QVector2D(-1,1), QVector2D(1,1),QVector2D(-1,-1), QVector2D(1,1), QVector2D(1,-1) };//左下，左上，右上，右下
		//for (QVector2D &v: _backGroundVertex)
		//{
		//	v /= id;
		//}
		_backGroundColor = QVector<QVector3D>{ QVector3D(0.59,0.78,0.93),QVector3D(0.59,0.78,0.93),QVector3D(0.88,0.93,0.98), QVector3D(0.59,0.78,0.93),QVector3D(0.88,0.93,0.98),QVector3D(0.88,0.93,0.98) };//左下，左上，右上，右下
		for (QVector3D &v : _backGroundColor)
		{
			v /= id;
		}
		id++;
		_backgroundShader = mShaderManage::GetInstance()->GetShader("BackGround");
		asset_ref<Vec2Array> _vertex = MakeAsset<Vec2Array>(_backGroundVertex);
		asset_ref<Vec3Array> _color = MakeAsset<Vec3Array>(_backGroundColor);
		_drawable = MakeAsset<Drawable>();
		_drawable->setVertexAttribArray(0, _vertex);
		_drawable->setVertexAttribArray(1, _color);
		asset_ref<StateSet> set = _drawable->getOrCreateStateSet();
		set->setAttributeAndModes(MakeAsset<Depth>(), 0);
		set->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		set->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);
		set->setShader(_backgroundShader);

		root->addChild(_drawable);

		app->GLContext()->doneCurrent();
	}

	mBackGroundRender::~mBackGroundRender()
	{
		

	}

	void mBackGroundRender::setBackGroundMode(BackGroundMode backgroundmode)
	{
		_backGroundMode = backgroundmode;
	}

	BackGroundMode mBackGroundRender::getBackGroundMode()
	{
		return _backGroundMode;
	}

	void mBackGroundRender::setGradientColor(QVector3D color1, QVector3D color2)
	{
		color1 = color1 / 255.0;
		color2 = color2 / 255.0;
		if (_backGroundMode == GradientL_R)
		{
			_backGroundColor = QVector<QVector3D>{ color1,color1,color2, color1, color2, color2 };//左下，左上，右上，右下
		}
		else if (_backGroundMode == GradientT_B)
		{
			_backGroundColor = QVector<QVector3D>{ color2,color1,color1, color2,color1,color2 };//左下，左上，右上，右下
		}
		
		//更新
		Array* color = _drawable->getVertexAttribArray(1);
		color->updata(0, _backGroundColor.size() * sizeof(QVector3D), _backGroundColor.data());
	}

	void mBackGroundRender::getGradientColor(QVector3D& color1, QVector3D& color2)
	{
		if (_backGroundMode == GradientL_R)
		{
			color1 = _backGroundColor.first() * 255;
			color2 = _backGroundColor.last() * 255;
		}
		else if (_backGroundMode == GradientT_B)
		{
			color1 = _backGroundColor.at(1) * 255;
			color2 = _backGroundColor.first() * 255;
		}
	}

	void mBackGroundRender::setPureColor(QVector3D color)
	{
		color = color / 255.0;
		_backGroundColor = QVector<QVector3D>{ color,color,color, color,color,color };//左下，左上，右上，右下
		
	}

	void mBackGroundRender::getPureColor(QVector3D& color)
	{
		color = _backGroundColor.first() * 255;
	}
}