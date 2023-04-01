#include "mPostMeshFaceData1.h"
namespace MDataPost
{
	mPostMeshFaceData1::mPostMeshFaceData1(int ID, QVector<int> index, int meshID, int order, QString partName)
	{
		_ID = ID;
		_index[3] = -1;
		for (int i = 0; i < index.size(); i++)
		{
			_index[i] = index[i];
		}

		_meshID1 = meshID;
		_order1 = order;
		
		_meshID2 = 0;
		_isShow = true;
	}

	mPostMeshFaceData1::~mPostMeshFaceData1()
	{
	}

	void mPostMeshFaceData1::appendMeshID(int meshID, int order, int startIndex)
	{
		_meshID2 = meshID;
		_order2 = order;

		for (int i = 0;i< 4;++i)
		{
			if (_index[i] != -1 && _index[i] == startIndex)
			{
				_startIndex = i;
				break;
			}
		}
	}

	QVector<int> mPostMeshFaceData1::getNodeIndex()
	{
		//QVector<int> sindex;
		//for (int i = 0; i < 4 ; i++)
		//{
		//	if (_index[i] != -1)
		//	{
		//		sindex.append(_index[i]);
		//	}
		//}
		if (_index[3] != -1)
		{
			return QVector<int>{_index[0], _index[1], _index[2], _index[3]};
		}

		return QVector<int>{_index[0], _index[1], _index[2]};
		
	}

	QVector<int> mPostMeshFaceData1::getNodeIndex(int meshID)
	{
		if (meshID == _meshID1)
		{
			if (_index[3] == -1)
			{
				return QVector<int>{_index[0], _index[1], _index[2]};
			}
			return QVector<int>{_index[0], _index[1], _index[2], _index[3]};
		}
		else
		{
			if (_index[3] != -1)//四边形
			{
				switch (_startIndex)
				{
				case 0:return QVector<int>{_index[0], _index[3], _index[2], _index[1]};
					break;				
				case 1:return QVector<int>{_index[1], _index[0], _index[3], _index[2]};
					break;
				case 2:return QVector<int>{_index[2], _index[1], _index[0], _index[3]};
					   break;
				case 3:return QVector<int>{_index[3], _index[2], _index[1], _index[0]};
					   break;
				default:
					break;
				}
			}
			else//三角形
			{
				switch (_startIndex)
				{
				case 0:return QVector<int>{_index[0], _index[2], _index[1]};
					   break;
				case 1:return QVector<int>{_index[1], _index[0], _index[2]};
					   break;
				case 2:return QVector<int>{_index[2], _index[1], _index[0]};
					   break;
				case 3:return QVector<int>{_index[2], _index[1], _index[0]};
					   break;
				default:
					break;
				}
			}
		}
		return QVector<int>();
	}

	QPair<int, int> mPostMeshFaceData1::getFirstMesh()
	{
		return QPair<int, int>(_meshID1,_order1);
	}
	QString mPostMeshFaceData1::getPartName()
	{
		QString _partName;
		return _partName;
	}
	int mPostMeshFaceData1::getMeshID1()
	{
		return _meshID1;
	}

	short int mPostMeshFaceData1::getMeshOrder1()
	{
		return _order1;
	}

	int mPostMeshFaceData1::getMeshID2()
	{
		return _meshID2;
	}

	short int mPostMeshFaceData1::getMeshOrder2()
	{
		return _order2;
	}

	int mPostMeshFaceData1::getMeshFaceID()
	{
		return _ID;
	}
	bool mPostMeshFaceData1::deleteMesh(int meshID)
	{
		if (_meshID2 == meshID)
		{
			_meshID2 = 0;
			return false;
		}
		else if(_meshID1 == meshID&& _meshID2 == 0)
		{
			return true;
		}
		else if (_meshID1 == meshID && _meshID2 !=0)
		{
			_meshID1 = _meshID2;
			_meshID2 = 0;
			return false;
		}
		return false;
	}

	bool mPostMeshFaceData1::getVisual()
	{
		return _isShow;
	}

	void mPostMeshFaceData1::setVisual(bool isShow)
	{
		_isShow = isShow;
	}
}