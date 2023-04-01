#include "mPostColorTableData.h"

#include"qdebug.h"
#include"math.h"
namespace MDataPost
{
	mPostColorTableData::mPostColorTableData(float max, float min, bool isEquivariance, int num):_colortablenum(num), _max(max),_min(min),_isEquivariance(isEquivariance)
	{
		_ratio = 0.05f;
		_mxVertex = QVector2D(-0.98, 1.0 - _ratio);
		_titleVertex = QVector2D(-0.98, 1.0 - 2 * _ratio);
		_topleft = QVector2D(-0.98, 1.0 - 3 * _ratio);
		_topright = QVector2D(-0.94, 1.0 - 3 * _ratio);
		_bottomleft = QVector2D(-0.98, _topleft.y() - _colortablenum * _ratio);
		_bottomright = QVector2D(-0.94, _topright.y() - _colortablenum * _ratio);
		_colorTableTexture = QVector<float>{ 1.00,1.00,0.00,0.00,0.00,1.00 };
		UpdateVertex();
		initialColorTable();
	}

	mPostColorTableData::~mPostColorTableData()
	{
		_colorTableVertex.clear();
		_colorTableTexture.clear();
		_lineMarkVertex.clear();
		_fontVertex.clear();
	}

	void mPostColorTableData::setPostColorRowRatio(float ratio)
	{
		_ratio = ratio;
		_mxVertex = QVector2D(-0.98, 1.0 - _ratio);
		_titleVertex = QVector2D(-0.98, 1.0 - 2 * _ratio);
		_topleft = QVector2D(-0.98, 1.0 - 3 * _ratio);
		_topright = QVector2D(-0.94, 1.0 - 3 * _ratio);
		UpdateVertex();
	}

	void mPostColorTableData::setPostColorTableNum(int num)
	{
		_colortablenum = num;
		UpdateVertex();
		initialColorTable();
	}

	void mPostColorTableData::setPostColorTableValue(float max, float min)
	{
		_max = max;
		_min = min;
		UpdateVertex();
	}

	void mPostColorTableData::setPostColorTableIsEquivariance(bool isEquivariance)
	{
		if (isEquivariance == _isEquivariance)
		{
			return;
		}
		_isEquivariance = isEquivariance;
		UpdateVertex();
	}

	bool mPostColorTableData::getPostColorTableIsEquivariance()
	{
		return _isEquivariance;
	}

	int mPostColorTableData::getPostColorTableNum()
	{
		return _colortablenum;
	}

	GLubyte* mPostColorTableData::getColorTable()
	{
		return _colortable;
	}

	QVector<QVector2D> mPostColorTableData::getColorTableVertex()
	{
		return _colorTableVertex;
	}

	QVector<float> mPostColorTableData::getColorTableTexture()
	{
		return _colorTableTexture;
	}

	QVector<QVector2D> mPostColorTableData::getColorTableLineMarkVertex()
	{
		return _lineMarkVertex;
	}

	QVector<MViewBasic::FontText> mPostColorTableData::getColorTableNumVertex()
	{
		return _fontVertex;
	}

	QVector2D mPostColorTableData::getColorTableMXVertex()
	{
		return _mxVertex;
	}

	QVector2D mPostColorTableData::getColorTableTitleVertex()
	{
		return _titleVertex;
	}

	QVector2D mPostColorTableData::getColorTableMinVertex()
	{
		return _minVertex;
	}

	QVector2D mPostColorTableData::getColorTableMaxVertex()
	{
		return _maxVertex;
	}

	void mPostColorTableData::UpdateVertex()
	{

		//矩形的坐标
		_bottomleft.setY(_topleft.y() - _colortablenum * _ratio);
		_bottomright.setY(_topright.y() - _colortablenum * _ratio);
		_colorTableVertex = QVector<QVector2D>{ _topleft,_topright,_bottomright,_bottomright,_bottomleft,_topleft };

		//线条的坐标
		_lineMarkVertex.clear();
		for (int i = 0; i <= _colortablenum; ++i)
		{
			float y = _topleft.y() - i * _ratio;
			_lineMarkVertex.push_back(QVector2D(-0.98, y));
			_lineMarkVertex.push_back(QVector2D(-0.92, y));
		}
		//字的坐标
		_fontVertex.clear();
		if (_isEquivariance)//等差
		{
			float change = (_max - _min) / float(_colortablenum);
			for (int i = 0; i <= _colortablenum; ++i)
			{
				MViewBasic::FontText font;
				font.pos = QVector2D(-0.91, _topleft.y() - i * _ratio);
				font.value = QString("%1").arg((_max - i * change), 4, 'E', 3);
				font.visual = true;
				_fontVertex.push_back(font);
			}
		}
		else//等比
		{
			float change = (log10(_max / _min)) / float(_colortablenum);
			for (int i = 0; i <= _colortablenum; ++i)
			{
				MViewBasic::FontText font;
				font.pos = QVector2D(-0.91, _topleft.y() - i * _ratio);
				font.value = QString("%1").arg(_max / pow(10, i * change), 4, 'E', 3);
				font.visual = true;
				_fontVertex.push_back(font);
			}
		}
		_fontVertex[_colortablenum].value = QString("%1").arg(_min, 4, 'E', 3);

		//最大值字的坐标
		if (!_fontVertex.isEmpty())
		{
			_maxVertex = QVector2D(-0.98, _fontVertex.last().pos.y() - _ratio);
		}
		//最小值字的坐标
		if (!_fontVertex.isEmpty())
		{
			_minVertex = QVector2D(-0.98, _fontVertex.last().pos.y() - _ratio * 2);
		}
		
	}

	void mPostColorTableData::UpdateFontData()
	{
		if (_isEquivariance)//等差
		{
			float change = (_max - _min) / float(_colortablenum);
			for (int i = 0; i <= _colortablenum; ++i)
			{
				MViewBasic::FontText font;
				font.pos = QVector2D(-0.91, _topleft.y() - i * _ratio);
				font.value = QString("%1").arg((_max - i * change), 4, 'E', 3);
				font.visual = true;
				_fontVertex.push_back(font);
			}
		}
		else//等比
		{
			float change = (log10(_max / _min)) / float(_colortablenum);
			for (int i = 0; i <= _colortablenum; ++i)
			{
				MViewBasic::FontText font;
				font.pos = QVector2D(-0.91, _topleft.y() - i * _ratio);
				font.value = QString("%1").arg(_max / pow(10, i * change), 4, 'E', 3);
				font.visual = true;
				_fontVertex.push_back(font);
			}
		}
	}

	void mPostColorTableData::initialColorTable()
	{
		//新算法colormap
		int TempR = 0, TempG = 0, TempB = 255;
		float value = 255.0 * 4 / (_colortablenum - 1);
		float remainder = 0.0;
		_colortable[0] = TempR;
		_colortable[1] = TempG;
		_colortable[2] = TempB;
		//第一段，R为0,先是G从0到255，G到了255之后B开始减少
		for (int i = 1; i <= _colortablenum / 2; ++i)
		{
			_colortable[3 * i + 0] = 0.0;
			if (TempG < 255)
			{
				TempG += value;
				if (TempG >= 255)
				{
					remainder = TempG - 255.0;
					TempG = 255;
					TempB = TempB - remainder;
				}
				_colortable[3 * i + 1] = TempG;
				_colortable[3 * i + 2] = TempB;
			}
			else
			{
				TempB = TempB - value;
				if (TempB <= 0)
				{
					remainder = TempB;
					TempB = 0;
					break;
				}
				_colortable[3 * i + 1] = TempG;
				_colortable[3 * i + 2] = TempB;
			}
			//qDebug() << _colortable[3 * i] << _colortable[3 * i + 1] << _colortable[3 * i + 2];
		}
		TempR = 255, TempG = 0, TempB = 0;
		_colortable[3 * (_colortablenum - 1)] = TempR;
		_colortable[3 * (_colortablenum - 1) + 1] = TempG;
		_colortable[3 * (_colortablenum - 1) + 2] = TempB;
		//第二段,B为0,先是G从0到255，G到了255之后R开始减少
		for (int i = _colortablenum - 2; i >= _colortablenum / 2; --i)
		{
			_colortable[3 * i + 2] = 0.0;
			if (TempG < 255)
			{
				TempG += value;
				if (TempG >= 255)
				{
					remainder = TempG - 255.0;
					TempG = 255;
					TempR = TempR - remainder;
				}
				_colortable[3 * i + 0] = TempR;
				_colortable[3 * i + 1] = TempG;
			}
			//else if (TempG == 255)
			//{
			//	_colortable[3 * i + 0] = TempR;
			//	_colortable[3 * i + 1] = TempG;
			//}
			else
			{
				TempR = TempR - value;
				if (TempR <= 0)
				{
					remainder = TempR;
					TempR = 0;
					_colortable[3 * i + 0] = TempR;
					_colortable[3 * i + 1] = TempG;
					break;
				}
				_colortable[3 * i + 0] = TempR;
				_colortable[3 * i + 1] = TempG;
			}
			//qDebug() << _colortable[3 * i] << _colortable[3 * i + 1] << _colortable[3 * i + 2];
		}
	}
}
