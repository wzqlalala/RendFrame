#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"
#include <QOpenGLVersionFunctions>
#include <QVector2D>
#include "mBasicStruct.h"


namespace MDataPost
{
	class RENDDATA_EXPORT mPostColorTableData
	{
	public:

		/*
		 * 构造函数
		 */
		mPostColorTableData(float max, float min, bool isEquivariance = true, int num = 12);

		/*
		 * 析构函数
		 */
		~mPostColorTableData();

		/*
		 * 设置颜色的一行的比例高度
		 */
		void setPostColorRowRatio(float ratio);

		/*
		 * 设置颜色的层次
		 */
		void setPostColorTableNum(int num);

		/*
		 * 设置值的最大最小值
		 */
		void setPostColorTableValue(float max, float min);

		/*
		 * 设置颜色的层次是否等差
		 */
		void setPostColorTableIsEquivariance(bool isEquivariance);

		/*
		* 获取颜色表是否等差
		*/
		bool getPostColorTableIsEquivariance();

		/*
		 * 获取颜色的层次
		 */
		int getPostColorTableNum();

		/*
		 * 获取颜色表
		 */
		GLubyte* getColorTable();

		/*
		 * 获取颜色表的位置坐标值
		 */
		QVector<QVector2D> getColorTableVertex();

		/*
		 * 获取颜色表的纹理坐标值
		 */
		QVector<float> getColorTableTexture();

		/*
		 * 获取颜色表的刻度线的位置坐标值
		 */
		QVector<QVector2D> getColorTableLineMarkVertex();

		/*
		 * 获取颜色表的数字
		 */
		QVector<MViewBasic::FontText> getColorTableNumVertex();

		/*
		 * 获取颜色表的mx文字的坐标
		 */
		QVector2D getColorTableMXVertex();

		/*
		 * 获取颜色表的标题文字的坐标
		 */
		QVector2D getColorTableTitleVertex();

		/*
		*获取颜色表的最小值文字坐标
		*/
		QVector2D getColorTableMinVertex();

		/*
		*获取颜色表的最大值文字坐标
		*/
		QVector2D getColorTableMaxVertex();

	private:
		/*
		 * 更新顶点坐标
		 */
		void UpdateVertex();

		/*
		* 当纹理坐标系数改变时更新顶点的数据
		*/
		void UpdateFontData();

	private:
		/*
		 * 初始化颜色表
		 */
		void initialColorTable();

	private:
		//颜色的层次
		int _colortablenum{ 12 };

		//颜色表
		GLubyte _colortable[90];

		//窗口大小
		int Win_WIDTH, Win_HEIGHT;

		//一行的高度比例
		float _ratio;

		//颜色表的矩形位置坐标
		QVector<QVector2D> _colorTableVertex;

		//颜色表的矩形纹理坐标
		QVector<float> _colorTableTexture;

		//颜色表的顶层位置
		QVector2D _topleft,_topright;

		//颜色表的底层位置
		QVector2D _bottomleft, _bottomright;

		//颜色表的刻度线
		QVector<QVector2D> _lineMarkVertex;

		//颜色表的刻度数字值及其坐标
		QVector<MViewBasic::FontText> _fontVertex;

		//颜色表渲染的值的最大最小值
		float _max, _min;

		//是否等差
		bool _isEquivariance;

		//mx的标志
		QVector2D _mxVertex;

		//分量的标题文字坐标
		QVector2D _titleVertex;

		//最大值的文字坐标
		QVector2D _maxVertex;

		//最小值的文字坐标
		QVector2D _minVertex;
	};
}
