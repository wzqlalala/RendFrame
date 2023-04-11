#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QHash>
#include <QPair>
#include <QVector3D>
#include <QVector2D>
#include <set>

#include "mMeshViewEnum.h"
#include "mBasicEnum.h"

namespace MDataPost
{
	class mPostColorTableData;
	class RENDDATA_EXPORT mPostOneFrameRendData
	{
	public:
		/*
		 * 默认构造函数
		 */
		mPostOneFrameRendData();

		/*
		 * 复制构造函数
		 */
		mPostOneFrameRendData(int id);

		/*
		 * 析构函数
		 */
		~mPostOneFrameRendData();

		/*
		* 拷贝构造函数，用于线性动画构建
		*/
		mPostOneFrameRendData(const mPostOneFrameRendData& oneframe);

		/*
		 * 设置节点还是网格
		 */
		void setNodeOrElement(MViewBasic::NodeOrElement nodeOrElement);

		/*
		 * 获取节点还是网格
		 */
		MViewBasic::NodeOrElement getNodeOrElement();

		/*
		 * 设置位移数据，节点位移矢量
		 */
		void setNodeDisplacementData(const QHash<int, QVector3D>& dis);

		/*
		 * 设置渲染数据，节点标量
		 */
		void setRendData(int id,const QHash<int, float>& data, const QString & text);

		/*
		 * 设置渲染数据，矢量
		 */
		void setRendData(const QHash<int, QVector3D>& data, MViewBasic::NodeOrElement type);

		/*
		 * 获取变量的数据
		 */
		QHash<int,float> getRendValue();

		/*
		 * 获取节点对应的位移数据
		 */
		QHash<int, QVector3D> getNodeDisplacementData();

		/*
		 * 获取对应的矢量数据
		 */
		QHash<int, QVector3D> getVectorRendData();

		/*
		* 获取矢量数据类型
		*/
		MViewBasic::NodeOrElement getVectorValueType();

		/*
		 * 获取当前渲染的标题
		 */
		QString getRendTitle();

		/*
		 * 还原渲染最小最大值
		 */
		void resetMinMaxData();

		/*
		 * 设置渲染数据的最小值
		 */
		void setMinData(float value);

		/*
		 * 设置渲染数据的最大值
		 */
		void setMaxData(float value);

		/*
		 * 设置渲染数据的最值
		 */
		void setMinMaxData(float maxValue, float minValue);

		/*
		* 设置当前渲染数据的最值
		*/
		void setCurrentMinMaxData(float maxValue, float minValue);

		/*
		 * 设置原始数据的最小最大值
		 */
		void setOrignalMaxMinData(double max, double min);

		/*
		* 设置部件的最值
		*/
		void setPartMinMaxData(QString partName, float min, float max);

		/*
		* 通过最值更新颜色表
		*/
		void updateColorTableMinMaxData();

		/*
		* 根据部件最值获取最值
		*/
		QHash<QString, QVector2D> getPartMinMaxData();

		/*
		* 获取设置数据的最小值
		*/
		float getMinData();

		/*
		* 获取设置数据的最大值
		*/
		float getMaxData();

		/*
		* 获取当前渲染数据的最小值
		*/
		float getCurrentMinData();

		/*
		* 获取当前渲染数据的最大值
		*/
		float getCurrentMaxData();

		/*
		* 获取原始数据的最小值
		*/
		float getOriginalMinData();

		/*
		* 获取原始数据的最大值
		*/
		float getOriginalMaxData();

		/*
		 * 获取当前帧渲染
		 */
		int getRendID();

		/*
		 * 获取当前帧的颜色表
		 */
		mPostColorTableData *getRendColorTable();

		///*
		//*获取最小值的文字
		//*/
		//QString getMinText();

		///*
		//*获取最大值的文字
		//*/
		//QString getMaxText();

		/*
		 * 设置变形放大倍数
		 */
		void setDeformationScale(QVector3D scale);

		/*
		 * 获取变形放大倍数
		 */
		QVector3D getDeformationScale();

		/*
		 * 设置纹理坐标系数
		 */
		void setTextureCoordScale(float scale);

		/*
		 * 获取纹理坐标系数
		 */
		float getTextureCoordScale();

		/*
		* 删除变量变量数据和和矢量数据和位移数据
		*/
		void deleteValueAndDisplacementData();
	private:
		//颜色表对象
		mPostColorTableData *_postColorTable;

		//分量是根据节点还是单元
		MViewBasic::NodeOrElement _nodeOrElement;

		//当前帧渲染的标题
		QString _title;

		//当前帧的id
		int _id;

		//纹理坐标系数
		float _textureCoordScale;

		//变形放大倍数
		QVector3D _deformationScale;

		//变量标量
		QHash<int, float> _value;

		//原始数据最小值
		float _constMinData;

		//原始数据最大值
		float _constMaxData;

		//渲染的最小值
		float _minData;

		//渲染的最大值
		float _maxData;

		//当前渲染的最小值
		float _currentMinData;

		//当前渲染的最大值
		float _currentMaxData;

		//变量矢量
		QHash<int,QVector3D> _valueVector;

		//矢量变量类型
		MViewBasic::NodeOrElement _vectorValueType;

		//节点位移矢量数据
		QHash<int, QVector3D> _nodeDisplacement;

		//部件名的最值
		QHash<QString, QVector2D> _partMinMaxValue;
	};

}
