#pragma once
#include "rendview_global.h"

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

//MViewBasic
#include "mMeshViewEnum.h"

#include "app.h"

namespace mxr
{
	class Drawable;
	class Shader;
	class Group;
}

namespace MBaseRend
{
	class RENDVIEW_EXPORT mBackGroundRender : public QObject
	{
		Q_OBJECT

	public:
		/*
		* 默认构造函数
		*/
		mBackGroundRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> root);
		/*
		* 析构函数
		*/
		~mBackGroundRender();
		/*
		 * 设置背景的显示模式
		 */
		void setBackGroundMode(MViewBasic::BackGroundMode backgroundmode);
		/*
		 * 获取背景的显示模式
		 */
		MViewBasic::BackGroundMode getBackGroundMode();
		/*
		 * 设置上下或者左右的颜色
		 */
		void setGradientColor(QVector3D color1,QVector3D color2);
		/*
		 * 获取上下或者左右的颜色
		 */
		void getGradientColor(QVector3D &color1, QVector3D &color2);
		/*
		 * 设置纯色的颜色
		 */
		void setPureColor(QVector3D color);
		/*
		 * 获取纯色的颜色
		 */
		void getPureColor(QVector3D &color);
	private:	
		std::shared_ptr<mxr::Shader> _backgroundShader;
		std::shared_ptr<mxr::Drawable> _drawable;
		//背景矩形的顶点坐标
		QVector<QVector2D> _backGroundVertex;
		//背景矩形的颜色值
		QVector<QVector3D> _backGroundColor;
		//渲染背景的模式
		MViewBasic::BackGroundMode _backGroundMode;
	};
}
