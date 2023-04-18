#include "mArrowRender.h"

#include <QApplication>
#include <QDesktopWidget>

#include <renderpch.h>
#include "mShaderManage.h"

#include "mModelView.h"
#include "mCommonView.h"

//freetype
#include "ft2build.h"
#include FT_FREETYPE_H

using namespace mxr;
namespace MBaseRend
{
	//坐标轴顶点
	QVector<QVector3D> _Arrow_Vertices;

	mArrowRender::mArrowRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend * baseRend) :mBaseRender(app, parent, baseRend)
	{
		makeCurrent();

		SetVertices();

		_fixedArrowFontState = MakeAsset<StateSet>();
		asset_ref<Shader> shader = mShaderManage::GetInstance()->GetShader("Arrow_GlobalAxis");
		_fixedArrowFontState->setShader(shader);
		_fixedArrowFontState->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_fixedArrowFontState->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_fixedArrowFontState->setDrawMode(GL_LINES);
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uModel_c", QMatrix4x4()));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uView_c", QMatrix4x4()));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uProjection_c", QMatrix4x4()));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uScr_width", float(0)));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uScr_height", float(0)));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uPers_width", float(0)));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uPers_height", float(0)));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uAuxX_new", QVector3D()));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uAuxY_new", QVector3D()));
		_fixedArrowFontState->setUniform(MakeAsset<Uniform>("uAuxZ_new", QVector3D()));

		_commonArrowState = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("Arrow_Common");
		_commonArrowState->setShader(shader);
		_commonArrowState->setDrawMode(GL_LINES);
		_commonArrowState->setUniform(MakeAsset<Uniform>("uModel_c", QMatrix4x4()));
		_commonArrowState->setUniform(MakeAsset<Uniform>("uView_c", QMatrix4x4()));
		_commonArrowState->setUniform(MakeAsset<Uniform>("uProjection_c", QMatrix4x4()));
		_commonArrowState->setUniform(MakeAsset<Uniform>("uModel_m", QMatrix4x4()));
		_commonArrowState->setUniform(MakeAsset<Uniform>("uView_m", QMatrix4x4()));
		_commonArrowState->setUniform(MakeAsset<Uniform>("uProjection_m", QMatrix4x4()));
		_commonArrowState->setAttributeAndModes(MakeAsset<Depth>(), 1);
	}
	mArrowRender::~mArrowRender()
	{

	}
	void mArrowRender::clearAllRender()
	{
		QHash<QString, std::shared_ptr<mBaseArrow>>().swap(_commonArrows);
	}
	void mArrowRender::updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView)
	{
		QMatrix4x4 projection, view, model;
		projection.ortho(0, modelView->SCR_WIDTH, 0, modelView->SCR_HEIGHT, 0.1, 99);
		view.lookAt(QVector3D(0, 0, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		model.setToIdentity();

		_fixedArrowFontState->getUniform("uModel_c")->SetData(commonView->_model);
		_fixedArrowFontState->getUniform("uView_c")->SetData(commonView->_view);
		_fixedArrowFontState->getUniform("uProjection_c")->SetData(commonView->_projection);
		_fixedArrowFontState->getUniform("uScr_width")->SetData((float)modelView->SCR_WIDTH);
		_fixedArrowFontState->getUniform("uScr_height")->SetData((float)modelView->SCR_HEIGHT);
		_fixedArrowFontState->getUniform("uPers_width")->SetData((float)commonView->_Right - commonView->_Left);
		_fixedArrowFontState->getUniform("uPers_height")->SetData((float)commonView->_Top - commonView->_Bottom);
		_fixedArrowFontState->getUniform("uAuxX_new")->SetData(QVector3D::crossProduct(commonView->_Up, commonView->_Eye).normalized());
		_fixedArrowFontState->getUniform("uAuxY_new")->SetData(commonView->_Up.normalized());
		_fixedArrowFontState->getUniform("uAuxZ_new")->SetData(commonView->_Eye.normalized());

		_commonArrowState->getUniform("uModel_c")->SetData(commonView->_model);
		_commonArrowState->getUniform("uView_c")->SetData(commonView->_view);
		_commonArrowState->getUniform("uProjection_c")->SetData(commonView->_projection);
		_commonArrowState->getUniform("uModel_m")->SetData(modelView->_model);
		_commonArrowState->getUniform("uView_m")->SetData(modelView->_view);
		_commonArrowState->getUniform("uProjection_m")->SetData(modelView->_projection);
	}
	void mArrowRender::appendCommonArrow(QString key, QVector<QVector3D> pos, QVector<QVector3D> dir, QVector3D color, float size)
	{
		makeCurrent();
		std::shared_ptr<mBaseArrow> arrows = MakeAsset<mBaseArrow>(_parent);
		arrows->setStateSet(_commonArrowState);

		arrows->AppendArrowVertex(pos.size(), 0);
		arrows->AppendArrowV_Vector3(pos.size(), _Arrow_Vertices.size(), pos, 1);
		arrows->AppendArrowV_Vector3(pos.size(), _Arrow_Vertices.size(), dir, 2);
		arrows->AppendArrowFloat(pos.size(), _Arrow_Vertices.size(), size, 3);
		arrows->AppendArrowVector3(pos.size(), _Arrow_Vertices.size(), color, 4);
		arrows->AppendArrowIntIndex(pos.size());
		_commonArrows[key] = arrows;
	}
	void mArrowRender::setCommonArrowIsShow(QString key, bool isShow)
	{
		auto value = _commonArrows.value(key);
		if (value)
		{
			value->setIsShow(isShow);
		}
	}
	void mArrowRender::appendGloabalAxisArrow()
	{
		makeCurrent();
		_globalAxisArrow = MakeAsset<mBaseArrow>(_parent);
		_globalAxisArrow->setStateSet(_fixedArrowFontState);
		_globalAxisArrow->AppendArrowV_Int(3, _Arrow_Vertices.size(), QVector<int>{0,1,2}, 0);
		_globalAxisArrow->AppendArrowVertex(3, 1);
		_globalAxisArrow->AppendArrowIntIndex();
	}

	mBaseArrow::mBaseArrow(std::shared_ptr<mxr::Group> parent)
	{
		_parent = parent;

		_drawable = MakeAsset<Drawable>();

		_parent->addChild(_drawable);
	}

	mBaseArrow::~mBaseArrow()
	{
		if (_parent)
		{
			_parent->removeChild(_drawable);
		}
	}

	void mBaseArrow::setIsShow(bool isShow)
	{
		if (_drawable)
		{
			_drawable->setNodeMask(isShow?0:1);
		}
	}

	void mBaseArrow::AppendArrowFloat(int size, int num, float f, int location)
	{
		asset_ref<FloatArray> _Size = MakeAsset<FloatArray>();
		_Size->append(QVector<float>(num * size, f));
		_drawable->setVertexAttribArray(location, _Size);
	}

	void mBaseArrow::AppendArrowV_Vector3(int size, int num, QVector<QVector3D> pos, int location)
	{
		asset_ref<Vec3Array> _Pos = MakeAsset<Vec3Array>();
		for (int i = 0; i < size; i++)
		{
			_Pos->append(QVector<QVector3D>(num, pos[i]));
		}
		_drawable->setVertexAttribArray(location, _Pos);
	}

	void mBaseArrow::AppendArrowV_Vector2(int size, int num, QVector<QVector2D> pos, int location)
	{
		asset_ref<Vec2Array> _Pos = MakeAsset<Vec2Array>();
		for (int i = 0; i < size; i++)
		{
			_Pos->append(QVector<QVector2D>(num, pos[i]));
		}
		_drawable->setVertexAttribArray(location, _Pos);
	}

	void mBaseArrow::AppendArrowV_Float(int size, int num, QVector<float> vec2, int location)
	{
		asset_ref<FloatArray> _Pos = MakeAsset<FloatArray>();
		for (int i = 0; i < size; i++)
		{
			_Pos->append(QVector<float>(num, vec2[i]));
		}
		_drawable->setVertexAttribArray(location, _Pos);
	}

	void mBaseArrow::AppendArrowV_Int(int size, int num, QVector<int> type, int location)
	{
		asset_ref<FloatArray> _Type = MakeAsset<FloatArray>();
		for (int i = 0; i < size; i++)
		{
			_Type->append(QVector<float>(num, type[i]));
		}
		_drawable->setVertexAttribArray(location, _Type);
	}

	void mBaseArrow::AppendArrowVector3(int size, int num, QVector3D color, int location)
	{
		asset_ref<Vec3Array> _Color = MakeAsset<Vec3Array>();
		_Color->append(QVector<QVector3D>(num * size, color));
		_drawable->setVertexAttribArray(location, _Color);
	}

	void mBaseArrow::AppendArrowInt(int size, int num, int type, int location)
	{
		asset_ref<FloatArray> _Type = MakeAsset<FloatArray>();
		_Type->append(QVector<float>(num * size, type));
		_drawable->setVertexAttribArray(location, _Type);
	}

	void mBaseArrow::AppendArrowIntIndex(int size)
	{
		QVector<unsigned int> index;
		int BeginIndex = 0;
		for (int i = 0; i < size; i++)
		{
			//直线
			index.push_back(BeginIndex);//0
			index.push_back(BeginIndex + 1);//1		

			index.push_back(BeginIndex + 1);//1
			index.push_back(BeginIndex + 2);//2

			index.push_back(BeginIndex + 1);//1
			index.push_back(BeginIndex + 3);//3

			index.push_back(BeginIndex + 1);//1
			index.push_back(BeginIndex + 4);//4

			index.push_back(BeginIndex + 1);//1
			index.push_back(BeginIndex + 5);//5

			index.push_back(BeginIndex + 2);//2
			index.push_back(BeginIndex + 3);//3

			index.push_back(BeginIndex + 3);//3
			index.push_back(BeginIndex + 4);//4

			index.push_back(BeginIndex + 4);//4
			index.push_back(BeginIndex + 5);//5

			index.push_back(BeginIndex + 5);//5
			index.push_back(BeginIndex + 2);//2	

			BeginIndex += 6;


		}
		asset_ref<UIntArray> uintarray = MakeAsset<UIntArray>(index);
		_drawable->setIndexAttribute(uintarray);
	}

	void mBaseArrow::AppendArrowVertex(int size, int location)
	{
		asset_ref<Vec3Array> _Pos = MakeAsset<Vec3Array>();
		for (int i = 0; i < size; i++)
		{
			_Pos->append(_Arrow_Vertices);
		}
		_drawable->setVertexAttribArray(location, _Pos);
	}

	void SetVertices()
	{
		if (!_Arrow_Vertices.empty())
		{
			return;
		}

		float h = 0.75; //箭头顶点---原点距离
		float h1 = 0.6;//四棱锥底面----原点距离
		float a = 0.06;//四棱锥底面边长的一半
		_Arrow_Vertices =
		{
			QVector3D(0.0f,0.0f,0.0f),//0
			QVector3D(h,0.0f,0.0f),	  //1
			QVector3D(h1,a,-a),		  //2	
			QVector3D(h1,a,a),		  //3
			QVector3D(h1,-a,a),		  //4
			QVector3D(h1,-a,-a)		  //5
		};
	}

}
