#include "mPostMeshPartData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshData1.h"
namespace MDataPost
{
	//int mPostMeshPartData1::ID = 0;

	QList<QVector3D> mPostMeshPartData1::Color = QList<QVector3D>{ QVector3D(1.00f, 0.00f, 0.00f),
										   QVector3D(0.00f, 0.00f, 1.00f),
										  QVector3D(0.65f, 0.59f, 0.0f),
										  QVector3D(0.84f, 0.19f, 0.46f),
										  QVector3D(0.57f, 0.43f, 0.85f),
										  QVector3D(0.91f, 0.67f, 0.65f),
										  QVector3D(0.50f, 0.24f, 0.12f),
										  QVector3D(0.05f, 0.94f, 0.04f) };

	mPostMeshPartData1::mPostMeshPartData1()
	{
		//ID++;
		_partName = "";
		_isShow = true;
		_partID = 1;
		int num = (_partID - 1) % Color.size();
		_partColor = Color.at(num);
		//spaceTree = nullptr;
	}

	mPostMeshPartData1::mPostMeshPartData1(QString name, int ID)
	{
		//ID++;
		_partName = name;
		_isShow = true;
		_partID = ID;
		int num = (_partID - 1) % Color.size();
		_partColor = Color.at(num);
		//spaceTree = nullptr;
	}

	mPostMeshPartData1::~mPostMeshPartData1()
	{
		//Space::destoryTree(spaceTree);
	}

	void mPostMeshPartData1::setPartColor(QVector3D color)
	{
		_partColor = color / 255.0;
	}

	void mPostMeshPartData1::setPartVisual(bool isshow)
	{
		_isShow = isshow;
	}

	void mPostMeshPartData1::setMeshID0(const std::set<int>& ids)
	{
		//_meshIDs0 = ids;
	}

	void mPostMeshPartData1::setMeshID1(const std::set<int>& ids)
	{
		//_meshIDs1 = ids;
	}

	void mPostMeshPartData1::setMeshID2(const std::set<int>& ids)
	{
		//_meshIDs2 = ids;
	}

	void mPostMeshPartData1::setMeshID3(const std::set<int>& ids)
	{
		//_meshIDs3= ids;
	}

	void mPostMeshPartData1::setMeshLineID(const std::set<int>& ids)
	{
		_meshLineID = ids;
	}

	void mPostMeshPartData1::setMeshFaceID(const std::set<int>& ids)
	{
		//_meshFaceID = ids;
	}

	
	void mPostMeshPartData1::appendMesh0(mPostMeshData1 * meshID)
	{
		_meshData0.push_back(meshID);
	}

	void mPostMeshPartData1::appendMesh1(mPostMeshData1 * meshID)
	{
		_meshData1.push_back(meshID);

	}

	void mPostMeshPartData1::appendMesh2(mPostMeshData1 *meshID)
	{
		_meshData2.push_back(meshID);

	}

	void mPostMeshPartData1::appendMesh3(mPostMeshData1 * meshID)
	{
		_meshData3.push_back(meshID);

	}

	void mPostMeshPartData1::appendMeshLineID(int geoLineID)
	{
		_meshLineID.insert(geoLineID);
	}

	void mPostMeshPartData1::appendMeshFaceID(int geoFaceID)
	{
		//_meshFaceID.insert(geoFaceID);
	}

	void mPostMeshPartData1::deleteMeshID0(int meshID)
	{
		//_meshIDs0.erase(meshID);
	}

	void mPostMeshPartData1::deleteMeshID1(int meshID)
	{
		//_meshIDs1.erase(meshID);
	}

	void mPostMeshPartData1::deleteMeshID2(int meshID)
	{
		//_meshIDs2.erase(meshID);
	}

	void mPostMeshPartData1::deleteMeshID3(int meshID)
	{
		//_meshIDs3.erase(meshID);
	}

	void mPostMeshPartData1::deleteMeshLineID(int geoLineID)
	{
		_meshLineID.erase(geoLineID);
	}

	void mPostMeshPartData1::deleteMeshFaceID(int geoFaceID)
	{
		//_meshFaceID.erase(geoFaceID);
	}

	std::set<int> mPostMeshPartData1::getMeshIDs0()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData0.size(); i++)
			ids.insert(_meshData0[i]->getMeshID());
		return ids;
	}

	std::set<int> mPostMeshPartData1::getMeshIDs1()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData1.size(); i++)
			ids.insert(_meshData1[i]->getMeshID());
		return ids;
	}

	std::set<int> mPostMeshPartData1::getMeshIDs2()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData2.size(); i++)
			ids.insert(_meshData2[i]->getMeshID());
		return ids;
	}

	std::set<int> mPostMeshPartData1::getMeshIDs3()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData3.size(); i++)
			ids.insert(_meshData3[i]->getMeshID());
		return ids;
	}

	QVector<mPostMeshData1*> mPostMeshPartData1::getMeshDatas0()
	{
		return _meshData0;
	}

	QVector<mPostMeshData1*> mPostMeshPartData1::getMeshDatas1()
	{
		return _meshData1;
	}

	QVector<mPostMeshData1*> mPostMeshPartData1::getMeshDatas2()
	{
		return _meshData2;
	}

	QVector<mPostMeshData1*> mPostMeshPartData1::getMeshDatas3()
	{
		return _meshData3;
	}

	int mPostMeshPartData1::getMeshCount()
	{
		return _meshData0.size() + _meshData1.size() + _meshData2.size() + _meshData3.size();
	}

	std::set<int> mPostMeshPartData1::getMeshLineIDs()
	{
		return _meshLineID;
	}

	std::set<int> mPostMeshPartData1::getMeshFaceIDs()
	{	
		std::set<int> ids;
		
		for (int i = 0; i < _meshData3.size(); i++)
		{
			QVector<int> _ids = _meshData3[i]->getMeshFaces();
			for (int j = 0; j < _ids.size(); j++)
			{
				ids.insert(_ids[j]);
			}
		}
		return ids;

	}

	std::set<mPostMeshFaceData1*> mPostMeshPartData1::getMeshFaces()
	{
		std::set<mPostMeshFaceData1*> ids;
		for (int i = 0; i < _meshData3.size(); i++)
		{
			QVector<mPostMeshFaceData1*> _ids = _meshData3[i]->getFace();
			for (int j = 0; j < _ids.size(); j++)
			{
				ids.insert(_ids[j]);
			}
		}
		return ids;
	}

	QVector3D mPostMeshPartData1::getPartColor()
	{
		return _partColor;
	}

	bool mPostMeshPartData1::getPartVisual()
	{
		return _isShow;
	}

	int mPostMeshPartData1::getPartID()
	{
		return _partID;
	}

	QString mPostMeshPartData1::getPartName()
	{
		return _partName;
	}
	void mPostMeshPartData1::appendMeshFaceData(const QVector<mPostMeshFaceData1*>& face)
	{
		_faceData.append(face);
	}
	void mPostMeshPartData1::setMeshFaceData(const QVector<mPostMeshFaceData1*> &face)
	{		
		_faceData = face;
	}

	void mPostMeshPartData1::clearMeshFaceData()
	{
		QVector<mPostMeshFaceData1*>().swap(_faceData);
		//_faceData.clear();
	}

	QVector<mPostMeshFaceData1*> mPostMeshPartData1::getMeshFaceData()
	{
		return _faceData;
	}
}
