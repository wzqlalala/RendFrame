#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QVector2D>
#include <QVector>
#include <QVector3D>
#include <QHash>
#include <qopengl.h>

#include "mBaseRender.h"

#include "Drawable.h"

#include <memory>

namespace mxr
{
	class Drawable;
	class StateSet;
	class Texture;
	class Group;
	class Uniform;
	class Shader;
}
namespace MBaseRend 
{
	//字形数据存储结构体
	struct Character
	{
		//大纹理上字形的像素原点和大小
		QVector2D  Positon;		//该字体原点（左下角）在文字大纹理上的像素坐标
		QVector2D  Size;       // 字形大小(像素大小 Width Height)

		//用来设置绘制字形的顶点数据（保持间隔）
		QVector2D  Bearing;    // 从基准线到字形左部/顶部的偏移值
		GLuint     Advance;    // 原点距下一个字形原点的距离

	};

	//大纹理
	extern std::shared_ptr<mxr::Texture> textTexture;

	//字形数据库
	extern QHash<GLchar, Character> Characters;

	class mBaseFont
	{
	public:
		mBaseFont(std::shared_ptr<mxr::Group> parent);

		~mBaseFont();

		void setStateSet(std::shared_ptr<mxr::StateSet> stateSet) { _drawable->setStateSet(stateSet); };

		void setIsShow(bool isShow);

		//文字基本数据，位置和纹理坐标
		void AppendFontVertexAndTexcoord(QVector<QString> txt, int vlocation, int tlocation);
		void AppendFontFloat(QVector<QString> txt, float size, int location);
		void AppendFontV_Vector3(QVector<QString> txt, QVector<QVector3D> pos, int location);
		void AppendFontV_Vector2(QVector<QString> txt, QVector<QVector2D> pos, int location);
		void AppendFontV_Int(QVector<QString> txt, QVector<int> type, int location);
		void AppendFontVector3(QVector<QString> txt, QVector3D color, int location);
		void AppendFontInt(QVector<QString> txt, int type, int location);

	protected:

		std::shared_ptr<mxr::Group> _parent;

		std::shared_ptr<mxr::Drawable> _drawable;
	};

	class mBaseRend;
	class RENDVIEW_EXPORT mFontRender : public mBaseRender
	{
	public:

		mFontRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend *baseRend = nullptr);

		~mFontRender();

		void clearAllRender();

		void updateUniform(shared_ptr<mModelView> modelView, shared_ptr<mCommonView> commonView) override;

		void appendFixedFont(QString key, QVector<QVector2D> pos, QVector<QString> txt, QVector3D color = QVector3D(1,1,1), float size = 1.0);//添加固定文字显示
		void setFixedFontIsShow(QString key, bool isShow);

		void appendCommonFont(QString key, QVector<QVector3D> pos, QVector<QString> txt, QVector3D color = QVector3D(1, 1, 1), float size = 1.0);//添加固定文字显示
		void setCommonFontIsShow(QString key, bool isShow);

		void appendGloabalAxisFont();//添加全局坐标系文字显示


	protected:



	private:                                                                                                                                                                
		//加载文字大纹理
		void LoadFontTexture();
	protected:

		//根据分辨率调整字体缩放比率
		static float _ratio;

		std::shared_ptr<mxr::StateSet> _fixedFontState;//不支持旋转、平移、缩放等的事件
		std::shared_ptr<mxr::StateSet> _fixedAxisFontState;//不支持平移、缩放等的事件、支持旋转的事件
		std::shared_ptr<mxr::StateSet> _commonFontState;//支持平移、旋转的事件

		QHash<QString, std::shared_ptr<mBaseFont>> _fixedFonts;

		QHash<QString, std::shared_ptr<mBaseFont>> _commonFonts;

		std::shared_ptr<mBaseFont> _globalAxisFont;

	};
}

