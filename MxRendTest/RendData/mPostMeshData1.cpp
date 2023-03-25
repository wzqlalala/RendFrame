#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
namespace MDataPost
{
	mPostMeshData1::mPostMeshData1(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType eleType, QString partName)
	{
		_meshID = ID;
		_meshType = meshType;
		_eleType = eleType;
		_isShow = true;
		_partName = partName;
	}

	mPostMeshData1::~mPostMeshData1()
	{
	}

	void mPostMeshData1::setNodeIndex(QVector<int> index)
	{
		
		_index = index;
	}

	QVector<int> mPostMeshData1::getNodeIndex()
	{
		if (_index.size() != 0)
		{
			return _index;
		}
		else
		{
			std::set<int> set_index;
			QVector<int> indexs;
			QVector<int> index;
			switch (_meshType)
			{
			case MBasicFunction::MeshTet:
				indexs.append(faces[0]->getNodeIndex(_meshID)[0]);
				indexs.append(faces[2]->getNodeIndex(_meshID));
				break;
			case MBasicFunction::MeshWedge:
				index = faces[0]->getNodeIndex(_meshID);
				indexs.append(index[0]);
				indexs.append(index[2]);
				indexs.append(index[1]);
				indexs.append(faces[1]->getNodeIndex(_meshID));
				break;
			case MBasicFunction::MeshHex:
				index = faces[0]->getNodeIndex(_meshID);
				indexs.append(index[0]);
				indexs.append(index[3]);
				indexs.append(index[2]);
				indexs.append(index[1]);	
				indexs.append(faces[1]->getNodeIndex(_meshID));
				break;
			case MBasicFunction::MeshPyramid:
				indexs.append(faces[0]->getNodeIndex(_meshID));
				indexs.append(faces[1]->getNodeIndex(_meshID)[2]);
				break;
			default:
				break;
			}
			return indexs;
		}
	}


	MBasicFunction::MeshType mPostMeshData1::getMeshType()
	{
		return _meshType;
	}

	void mPostMeshData1::setElementType(MBasicFunction::ElementType eleType)
	{
		_eleType = eleType;
	}

	MBasicFunction::ElementType mPostMeshData1::getElementType()
	{
		return _eleType;
	}

	void mPostMeshData1::appendMeshFace(mPostMeshFaceData1 * meshFace)
	{
		faces[_location] = meshFace;
		_location++;
	}

	void mPostMeshData1::setMeshFace(uint8_t location, mPostMeshFaceData1 * meshFace)
	{
		faces[location - 1] = meshFace;
		_location = std::max(uint8_t(_location - 1), location);
	}



	QVector<int> mPostMeshData1::getMeshFaces()
	{
		QVector<int> ids;
		for (int i = 0; i < _location; i++)
		{
			ids.append(faces[i]->getMeshFaceID());
		}
		return ids;
	}

	QVector<mPostMeshFaceData1*> mPostMeshData1::getFace()
	{
		QVector<mPostMeshFaceData1*> ids;
		ids.resize(_location);
		memcpy(&ids[0], faces, _location * sizeof(mPostMeshFaceData1*));
		return ids;
	}

	void mPostMeshData1::appendMeshLine(int meshLineID)
	{
		//_meshLineIDs.append(meshLineID);
	}

	void mPostMeshData1::setMeshLine(const QVector<int>& ids)
	{
		//_meshLineIDs = ids;
	}

	QVector<int> mPostMeshData1::getMeshLines()
	{
		QVector<int> ids;
		return ids;
		//return _meshLineIDs;
	}

	void mPostMeshData1::setMeshVisual(bool isshow)
	{
		_isShow = isshow;
	}

	void mPostMeshData1::reverseMeshVisual()
	{
		_isShow = !_isShow;
	}

	bool mPostMeshData1::getMeshVisual()
	{
		return _isShow;
	}

	int mPostMeshData1::getMeshDimension()
	{
		switch (_meshType)
		{
		case MBasicFunction::MeshPoint:return 0;
		case MBasicFunction::MeshBeam: return 1;
		case MBasicFunction::MeshTri:
		case MBasicFunction::MeshQuad:return 2;
		default:return 3;
		}
	}
	int mPostMeshData1::getMeshID()
	{
		return _meshID;
	}
	QString mPostMeshData1::getPartName()
	{
		return _partName;
	}

}
