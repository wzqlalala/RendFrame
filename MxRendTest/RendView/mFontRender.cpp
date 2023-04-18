#include "mFontRender.h"

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
	float mFontRender::_ratio = 0.0;

	std::shared_ptr<mxr::Texture> textTexture = 0;
	QHash<GLchar, Character> Characters;
	mFontRender::mFontRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend * baseRend) :mBaseRender(app, parent, baseRend)
	{
		makeCurrent();
		//获取屏幕分辨率
		int ScreenWidth = QApplication::desktop()->width();
		int ScreenHeight = QApplication::desktop()->height();
		//以小的为基准
		int standard = 1080; //开发基准为1080
		int smallsize = ScreenWidth > ScreenHeight ? ScreenHeight : ScreenWidth;
		_ratio = (float)standard / smallsize;

		this->LoadFontTexture();

		_fixedAxisFontState = MakeAsset<StateSet>();
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uView_font", QMatrix4x4()));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uProjection_font", QMatrix4x4()));

		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uModel_c", QMatrix4x4()));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uView_c", QMatrix4x4()));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uProjection_c", QMatrix4x4()));

		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uScr_width", float(0)));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uScr_height", float(0)));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uPers_width", float(0)));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uPers_height", float(0)));

		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uAuxX_new", QVector3D()));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uAuxY_new", QVector3D()));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uAuxZ_new", QVector3D()));
		_fixedAxisFontState->setUniform(MakeAsset<Uniform>("uRatio", float(0)));

		asset_ref<Shader> GlobalAxisShader = mShaderManage::GetInstance()->GetShader("Font_GlobalAxis");
		_fixedAxisFontState->setShader(GlobalAxisShader);
		_fixedAxisFontState->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_fixedAxisFontState->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_fixedAxisFontState->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);
		_fixedAxisFontState->setTexture("Texture", textTexture);

		_fixedFontState = MakeAsset<StateSet>();
		_fixedFontState->setUniform(MakeAsset<Uniform>("uView_font", QMatrix4x4()));
		_fixedFontState->setUniform(MakeAsset<Uniform>("uProjection_font", QMatrix4x4()));
		_fixedFontState->setUniform(MakeAsset<Uniform>("uModel_font", QMatrix4x4()));
		_fixedFontState->setUniform(MakeAsset<Uniform>("uScr_width", float(0)));
		_fixedFontState->setUniform(MakeAsset<Uniform>("uScr_height", float(0)));
		_fixedFontState->setUniform(MakeAsset<Uniform>("uRatio", float(0)));

		asset_ref<Shader> FixedFontShader = mShaderManage::GetInstance()->GetShader("Font_Fixed");
		_fixedFontState->setShader(FixedFontShader);
		_fixedFontState->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_fixedFontState->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_fixedFontState->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);
		_fixedFontState->setTexture("Texture", textTexture);


		_commonFontState = MakeAsset<StateSet>();
		_commonFontState->setUniform(MakeAsset<Uniform>("uView_font", QMatrix4x4()));
		_commonFontState->setUniform(MakeAsset<Uniform>("uProjection_font", QMatrix4x4()));
		_commonFontState->setUniform(MakeAsset<Uniform>("uModel_font", QMatrix4x4()));
		_commonFontState->setUniform(MakeAsset<Uniform>("uModel_m", QMatrix4x4()));
		_commonFontState->setUniform(MakeAsset<Uniform>("uView_m", QMatrix4x4()));
		_commonFontState->setUniform(MakeAsset<Uniform>("uProjection_m", QMatrix4x4()));
		_commonFontState->setUniform(MakeAsset<Uniform>("uScr_width", float(0)));
		_commonFontState->setUniform(MakeAsset<Uniform>("uScr_height", float(0)));
		_commonFontState->setUniform(MakeAsset<Uniform>("uRatio", float(0)));

		asset_ref<Shader> CommonFontShader = mShaderManage::GetInstance()->GetShader("Font_Common");
		_commonFontState->setShader(CommonFontShader);
		_commonFontState->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_commonFontState->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_commonFontState->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);
		_commonFontState->setTexture("Texture", textTexture);

	}
	mFontRender::~mFontRender()
	{

	}
	void mFontRender::clearAllRender()
	{
		QHash<QString, std::shared_ptr<mBaseFont>>().swap(_commonFonts);
		QHash<QString, std::shared_ptr<mBaseFont>>().swap(_fixedFonts);
	}
	void mFontRender::updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView)
	{
		QMatrix4x4 projection, view, model;
		projection.ortho(0, modelView->SCR_WIDTH, 0, modelView->SCR_HEIGHT, 0.1, 99);
		view.lookAt(QVector3D(0, 0, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		model.setToIdentity();

		_fixedAxisFontState->getUniform("uView_font")->SetData(view);
		_fixedAxisFontState->getUniform("uProjection_font")->SetData(projection);

		_fixedAxisFontState->getUniform("uModel_c")->SetData(commonView->_model);
		_fixedAxisFontState->getUniform("uView_c")->SetData(commonView->_view);
		_fixedAxisFontState->getUniform("uProjection_c")->SetData(commonView->_projection);

		_fixedAxisFontState->getUniform("uScr_width")->SetData((float)modelView->SCR_WIDTH);
		_fixedAxisFontState->getUniform("uScr_height")->SetData((float)modelView->SCR_HEIGHT);
		_fixedAxisFontState->getUniform("uPers_width")->SetData((float)commonView->_Right - commonView->_Left);
		_fixedAxisFontState->getUniform("uPers_height")->SetData((float)commonView->_Top - commonView->_Bottom);

		_fixedAxisFontState->getUniform("uAuxX_new")->SetData(QVector3D::crossProduct(commonView->_Up, commonView->_Eye).normalized());
		_fixedAxisFontState->getUniform("uAuxY_new")->SetData(commonView->_Up.normalized());
		_fixedAxisFontState->getUniform("uAuxZ_new")->SetData(commonView->_Eye.normalized());
		_fixedAxisFontState->getUniform("uRatio")->SetData(_ratio);

		_fixedFontState->getUniform("uView_font")->SetData(view);
		_fixedFontState->getUniform("uProjection_font")->SetData(projection);
		_fixedFontState->getUniform("uModel_font")->SetData(model);
		_fixedFontState->getUniform("uScr_width")->SetData((float)modelView->SCR_WIDTH);
		_fixedFontState->getUniform("uScr_height")->SetData((float)modelView->SCR_HEIGHT);
		_fixedFontState->getUniform("uRatio")->SetData(_ratio);


		_commonFontState->getUniform("uView_font")->SetData(view);
		_commonFontState->getUniform("uProjection_font")->SetData(projection);
		_commonFontState->getUniform("uModel_font")->SetData(model);
		_commonFontState->getUniform("uModel_m")->SetData(modelView->_model);
		_commonFontState->getUniform("uView_m")->SetData(modelView->_view);
		_commonFontState->getUniform("uProjection_m")->SetData(modelView->_projection);
		_commonFontState->getUniform("uScr_width")->SetData((float)modelView->SCR_WIDTH);
		_commonFontState->getUniform("uScr_height")->SetData((float)modelView->SCR_HEIGHT);
		_commonFontState->getUniform("uRatio")->SetData(_ratio);
	}
	void mFontRender::appendFixedFont(QString key, QVector<QVector2D> pos, QVector<QString> txt, QVector3D color, float size)
	{
		makeCurrent();
		std::shared_ptr<mBaseFont> fonts = MakeAsset<mBaseFont>(_parent);
		fonts->setStateSet(_fixedFontState);

		fonts->AppendFontVector3(txt, color, 0);
		fonts->AppendFontFloat(txt, size, 1);
		fonts->AppendFontVertexAndTexcoord(txt, 2, 3);
		fonts->AppendFontV_Vector2(txt, pos, 4);
		_fixedFonts[key] = fonts;
	}
	void mFontRender::setFixedFontIsShow(QString key, bool isShow)
	{
		auto value = _fixedFonts.value(key);
		if (value)
		{
			value->setIsShow(isShow);
		}
	}
	void mFontRender::appendCommonFont(QString key, QVector<QVector3D> pos, QVector<QString> txt, QVector3D color, float size)
	{
		makeCurrent();
		std::shared_ptr<mBaseFont> fonts = MakeAsset<mBaseFont>(_parent);
		fonts->setStateSet(_commonFontState);

		fonts->AppendFontVector3(txt, color, 0);
		fonts->AppendFontFloat(txt, size, 1);
		fonts->AppendFontVertexAndTexcoord(txt, 2, 3);
		fonts->AppendFontV_Vector3(txt, pos, 4);
		_commonFonts[key] = fonts;
	}
	void mFontRender::setCommonFontIsShow(QString key, bool isShow)
	{
		auto value = _commonFonts.value(key);
		if (value)
		{
			value->setIsShow(isShow);
		}
	}
	void mFontRender::appendGloabalAxisFont()
	{
		makeCurrent();
		QVector<QString> txt = { "X","Y","Z" };
		QVector<int> types = { 0,1,2 };
		_globalAxisFont = MakeAsset<mBaseFont>(_parent);
		_globalAxisFont->setStateSet(_fixedAxisFontState);
		_globalAxisFont->AppendFontV_Int(txt, types, 0);
		_globalAxisFont->AppendFontVertexAndTexcoord(txt, 1, 2);


	}
	void mFontRender::LoadFontTexture()
	{
		if (!textTexture)
		{
			textTexture = MakeAsset<Texture>(GL_TEXTURE_2D, 1024, 1024, 0, GL_R8, 1);
			//将字形纹理贴在大纹理上
			FT_Library ft;
			if (FT_Init_FreeType(&ft))
			{
				qDebug() << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
				return;
			}

			//创建字形
			FT_Face face;
			if (FT_New_Face(ft, (qApp->applicationDirPath() + "/../GLSL/arial.ttf").toLocal8Bit(), 0, &face))
			{
				qDebug() << "ERROR::FREETYPE: Failed to load font" << endl;
				return;
			}
			//设置字体大小
			FT_Set_Pixel_Sizes(face, 0, 48);	//宽度设为0即为根据高度动态计算出宽度
			//禁用字节对齐限制	
			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			textTexture->PixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for (GLubyte c = 0; c < 128; c++)
			{
				// 加载字符的字形 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					qDebug() << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
					continue;
				}
				textTexture->SetData(0, 50 * (c % 20), 50 * (c / 20), face->glyph->bitmap.width,
					face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
				Character character =
				{
					QVector2D(50 * (c % 20),50 * (c / 20)),					//文字纹理左下角的像素坐标
					QVector2D(face->glyph->bitmap.width,face->glyph->bitmap.rows),
					QVector2D(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x,
				};
				Characters.insert(static_cast<GLchar>(c), character);
			}
			textTexture->PixelStorei(GL_UNPACK_ALIGNMENT, 4);
			FT_Done_FreeType(ft);
		}
	}

	mBaseFont::mBaseFont(std::shared_ptr<mxr::Group> parent)
	{
		_parent = parent;

		_drawable = MakeAsset<Drawable>();

		_parent->addChild(_drawable);
	}

	mBaseFont::~mBaseFont()
	{
		if (_parent)
		{
			_parent->removeChild(_drawable);
		}
	}

	void mBaseFont::setIsShow(bool isShow)
	{
		if (_drawable)
		{
			_drawable->setNodeMask(isShow?0:1);
		}
	}

	void mBaseFont::AppendFontVertexAndTexcoord(QVector<QString> txt, int vlocation, int tlocation)
	{
		asset_ref<Vec2Array> _Vertex = MakeAsset<Vec2Array>();
		asset_ref<Vec2Array> _TexCoord = MakeAsset<Vec2Array>();
		asset_ref<UIntArray> _Index = MakeAsset<UIntArray>();
		int beginIndex = 0;

		for (int i = 0; i < txt.size(); i++)
		{
			int posx = 0;
			int posy = 0;
			string text = txt[i].toStdString();
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = Characters[*c];
				//顶点坐标
				GLfloat xpos = posx + ch.Bearing.x();
				GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
				//该字体的纹理坐标原点
				GLfloat xpos_texture = ch.Positon.x();
				GLfloat ypos_texture = ch.Positon.y();
				//纹理大小
				GLfloat w = ch.Size.x();
				GLfloat h = ch.Size.y();

				//第一组顶点
				_Vertex->push_back(QVector2D(xpos, ypos + h));
				_TexCoord->push_back(QVector2D(xpos_texture, ypos_texture));
				//第二组顶点		 
				_Vertex->push_back(QVector2D(xpos, ypos));
				_TexCoord->push_back(QVector2D(xpos_texture, ypos_texture + ch.Size.y()));
				//第三组顶点		 push_back		
				_Vertex->push_back(QVector2D(xpos + w, ypos));
				_TexCoord->push_back(QVector2D(xpos_texture + ch.Size.x(), ypos_texture + ch.Size.y()));
				//第四组顶点		 push_back		
				_Vertex->push_back(QVector2D(xpos + w, ypos + h));
				_TexCoord->push_back(QVector2D(xpos_texture + ch.Size.x(), ypos_texture));


				_Index->push_back(beginIndex);
				_Index->push_back(beginIndex + 1);
				_Index->push_back(beginIndex + 2);
				_Index->push_back(beginIndex + 2);
				_Index->push_back(beginIndex + 3);
				_Index->push_back(beginIndex);
				beginIndex += 4;
				posx += (ch.Advance >> 6);
			}
		}
		_drawable->setVertexAttribArray(vlocation, _Vertex);
		_drawable->setVertexAttribArray(tlocation, _TexCoord);
		_drawable->setIndexAttribute(_Index);
	}

	void mBaseFont::AppendFontFloat(QVector<QString> txt, float size, int location)
	{
		asset_ref<FloatArray> _Size = MakeAsset<FloatArray>();
		for (int i = 0; i < txt.size(); i++)
		{
			_Size->append(QVector<float>(4 * txt.at(i).size(), size));
		}
		_drawable->setVertexAttribArray(location, _Size);
	}

	void mBaseFont::AppendFontV_Vector3(QVector<QString> txt, QVector<QVector3D> pos, int location)
	{
		asset_ref<Vec3Array> _Pos = MakeAsset<Vec3Array>();
		for (int i = 0; i < txt.size(); i++)
		{
			_Pos->append(QVector<QVector3D>(4 * txt.at(i).size(), pos[i]));
		}
		_drawable->setVertexAttribArray(location, _Pos);
	}

	void mBaseFont::AppendFontV_Vector2(QVector<QString> txt, QVector<QVector2D> pos, int location)
	{
		asset_ref<Vec2Array> _Pos = MakeAsset<Vec2Array>();
		for (int i = 0; i < txt.size(); i++)
		{
			_Pos->append(QVector<QVector2D>(4 * txt.at(i).size(), pos[i]));
		}
		_drawable->setVertexAttribArray(location, _Pos);
	}

	void mBaseFont::AppendFontV_Int(QVector<QString> txt, QVector<int> type, int location)
	{
		asset_ref<FloatArray> _Type = MakeAsset<FloatArray>();
		for (int i = 0; i < txt.size(); i++)
		{
			_Type->append(QVector<float>(4 * txt.at(i).size(), type[i]));
		}
		_drawable->setVertexAttribArray(location, _Type);
	}

	void mBaseFont::AppendFontVector3(QVector<QString> txt, QVector3D color, int location)
	{
		asset_ref<Vec3Array> _Color = MakeAsset<Vec3Array>();
		for (int i = 0; i < txt.size(); i++)
		{
			_Color->append(QVector<QVector3D>(4 * txt.at(i).size(), color));
		}
		_drawable->setVertexAttribArray(location, _Color);
	}

	void mBaseFont::AppendFontInt(QVector<QString> txt, int type, int location)
	{
		asset_ref<FloatArray> _Type = MakeAsset<FloatArray>();
		for (int i = 0; i < txt.size(); i++)
		{
			_Type->append(QVector<float>(4 * txt.at(i).size(), type));
		}
		_drawable->setVertexAttribArray(location, _Type);
	}

}
