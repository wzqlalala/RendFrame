#include "mPostOneFrameRendData.h"
#include "mPostColorTableData.h"

#include<math.h>
#include <QSet>


namespace MDataPost
{
	mPostOneFrameRendData::mPostOneFrameRendData()
	{
		_postColorTable = new mPostColorTableData(0,0);
		_deformationScale = QVector3D(1.0,1.0,1.0);
		_textureCoordScale = 1.0;

		_nodeOrElement = MViewBasic::PostNode;
		_vectorValueType = MViewBasic::PostNode;
		_id = 0;
		_constMinData = 0;
		_constMaxData = 0;
		_minData = 0;
		_maxData = 0;
	}

	mPostOneFrameRendData::mPostOneFrameRendData(int id)
	{
		_postColorTable = new mPostColorTableData(0, 0);
		_deformationScale = QVector3D(1.0, 1.0, 1.0);
		_textureCoordScale = 1.0;

		_nodeOrElement = MViewBasic::PostNode;
		_vectorValueType = MViewBasic::PostNode;

		_id = id;
		_constMinData = 0;
		_constMaxData = 0;
		_minData = 0;
		_maxData = 0;
	}

	mPostOneFrameRendData::~mPostOneFrameRendData()
	{
		if (_postColorTable!=nullptr)
		{
			delete _postColorTable;
			_postColorTable = nullptr;
		}
		_value.clear();
		_valueVector.clear();
		_nodeDisplacement.clear();
	}

	mPostOneFrameRendData::mPostOneFrameRendData(const mPostOneFrameRendData& oneframe)
	{
		_postColorTable = new mPostColorTableData(*oneframe._postColorTable);
		_nodeOrElement = oneframe._nodeOrElement;
		_title = oneframe._title;
		_id = oneframe._id;
		_deformationScale = oneframe._deformationScale;
		_value = oneframe._value;
		_constMinData = oneframe._constMinData;
		_constMaxData = oneframe._constMaxData;
		_minData = oneframe._minData;
		_maxData = oneframe._maxData;
		_valueVector = oneframe._valueVector;
		_nodeDisplacement = oneframe._nodeDisplacement;
	}

	void mPostOneFrameRendData::setNodeOrElement(MViewBasic::NodeOrElement nodeOrElement)
	{
		_nodeOrElement = nodeOrElement;
	}

	MViewBasic::NodeOrElement mPostOneFrameRendData::getNodeOrElement()
	{
		return _nodeOrElement;
	}

	void mPostOneFrameRendData::setNodeDisplacementData(const QHash<int, QVector3D>& dis)
	{
		_nodeDisplacement = dis;
	}

	void mPostOneFrameRendData::setRendData(int id, const QHash<int,float> &nodevalues, const QString &text)
	{
		_id = id;
		_title = text;
		//设置颜色表
		this->updateColorTableMinMaxData();
		_value = nodevalues;

		////标题文字
		//QMap<QVector<float>, QString> res;
		//res.insert(QVector<float>{_postColorTable->getColorTableTitleVertex().x(), _postColorTable->getColorTableTitleVertex().y()}, text);
		////res.insert(QVector<float>{_mxVertex.x(), _mxVertex.y()}, "MxSim");

		//MViewBasic::mBasicRendData_post::GetInstance()->appendFixedFontData("postTitle", res, QVector3D(0, 0, 0), 1);
	}

	void mPostOneFrameRendData::setRendData(const QHash<int, QVector3D>& data, MViewBasic::NodeOrElement type)
	{
		_valueVector = data;
		_vectorValueType = type;
	}

	QHash<int, float> mPostOneFrameRendData::getRendValue()
	{
		return _value;
	}

	QHash<int, QVector3D> mPostOneFrameRendData::getNodeDisplacementData()
	{
		return _nodeDisplacement;
	}

	QHash<int, QVector3D> mPostOneFrameRendData::getVectorRendData()
	{
		return _valueVector;
	}

	MViewBasic::NodeOrElement mPostOneFrameRendData::getVectorValueType()
	{
		return _vectorValueType;
	}

	QString mPostOneFrameRendData::getRendTitle()
	{
		return _title;
	}

	void mPostOneFrameRendData::resetMinMaxData()
	{
		_minData = _constMinData;
		_maxData = _constMaxData;
		setMinMaxData(_constMaxData, _constMinData);
	}

	void mPostOneFrameRendData::setMinData(float value)
	{
		_minData = value;
		this->updateColorTableMinMaxData();
	}

	void mPostOneFrameRendData::setMaxData(float value)
	{
		_maxData = value;
		this->updateColorTableMinMaxData();
	}

	void mPostOneFrameRendData::setMinMaxData(float maxValue, float minValue)
	{
		_minData = minValue;
		_maxData = maxValue;
		setCurrentMinMaxData(maxValue, minValue);
	}

	void mPostOneFrameRendData::setCurrentMinMaxData(float maxValue, float minValue)
	{
		_currentMinData = minValue;
		_currentMaxData = maxValue;
		this->updateColorTableMinMaxData();
	}

	void mPostOneFrameRendData::setOrignalMaxMinData(double max, double min)
	{
		_constMinData = min;
		_constMaxData = max;
		_currentMinData = min;
		_currentMaxData = max;
		_minData = min;
		_maxData = max;
	}

	void mPostOneFrameRendData::setPartMinMaxData(QString partName, float min, float max)
	{
		_partMinMaxValue[partName].setX(min);
		_partMinMaxValue[partName].setY(max);
	}

	void mPostOneFrameRendData::updateColorTableMinMaxData()
	{
		_postColorTable->setPostColorTableValue(_currentMaxData, _currentMinData);
	}

	QHash<QString, QVector2D> mPostOneFrameRendData::getPartMinMaxData()
	{
		return _partMinMaxValue;
	}

	float mPostOneFrameRendData::getMinData()
	{
		return _minData;
	}

	float mPostOneFrameRendData::getMaxData()
	{
		return _maxData;
	}

	float mPostOneFrameRendData::getCurrentMinData()
	{
		return _currentMinData;
	}

	float mPostOneFrameRendData::getCurrentMaxData()
	{
		return _currentMaxData;
	}

	float mPostOneFrameRendData::getOriginalMinData()
	{
		return _constMinData;
	}

	float mPostOneFrameRendData::getOriginalMaxData()
	{
		return _constMaxData;
	}

	int mPostOneFrameRendData::getRendID()
	{
		return _id;
	}

	mPostColorTableData* mPostOneFrameRendData::getRendColorTable()
	{
		return _postColorTable;
	}
	//QString mPostOneFrameRendData::getMinText()
	//{
	//	return QString("min:%1").arg(_minData);
	//}
	//QString mPostOneFrameRendData::getMaxText()
	//{
	//	return QString("max:%1").arg(_maxData);
	//}

	void mPostOneFrameRendData::setDeformationScale(QVector3D scale)
	{
		_deformationScale = scale;
	}

	QVector3D mPostOneFrameRendData::getDeformationScale()
	{
		return _deformationScale;
	}
	void mPostOneFrameRendData::setTextureCoordScale(float scale)
	{
		_textureCoordScale = scale;
	}
	float mPostOneFrameRendData::getTextureCoordScale()
	{
		return _textureCoordScale;
	}
	void mPostOneFrameRendData::deleteValueAndDisplacementData()
	{
		_value.clear();
		_valueVector.clear();
		_nodeDisplacement.clear();
	}
}
