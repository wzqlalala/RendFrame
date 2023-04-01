#include "mPostMeshNodeData1.h"
#include <fstream>
#include "mAppConfigurationSingleton.h"
#include "mFileConfiguration.h"

#include <qguiapplication.h>

using namespace MAppConfiguration;
namespace MDataPost
{
	mPostMeshNodeData1::mPostMeshNodeData1(int ID, QVector3D vertex)
	{
		_nodeID = ID;
		_nodeVertex = vertex;
	}

	mPostMeshNodeData1::~mPostMeshNodeData1()
	{
	}

	//void mPostMeshNodeData1::addMesh(int meshID)
	//{
	//	//_meshIds.insert(meshID);
	//}

	bool mPostMeshNodeData1::deleteMesh(int meshID)
	{
		//_meshIds.erase(meshID);
		//if (_meshIds.size()==0)
		//{
		//	return true;
		//}
		return false;
	}
	int mPostMeshNodeData1::getNodeID()
	{
		return _nodeID;
	}

	QVector3D mPostMeshNodeData1::getNodeVertex()
	{
		return _nodeVertex;
	}

	void mPostMeshNodeData1::setNodeVertex(int position, float value)
	{
		_nodeVertex[position] = value;
	}

	//void mPostMeshNodeData1::setNodeInMeshIDs(const std::set<int>& ids)
	//{
	//	//_meshIds = ids;
	//}

	std::set<int> mPostMeshNodeData1::getNodeInMeshIDs()
	{
		std::set <int> ids;
		if (pointcell.size == 0)
		{
			return ids;
		}
		
		std::ifstream fin(mAppConfigurationSingleton::getInstance()->getFileConfiguration()->_postPointCellFileName + ".dat", std::ios::binary);
		fin.seekg(pointcell.addr, std::ios::beg);
		void* data = malloc(pointcell.size);
		fin.read((char*)data, pointcell.size);
		fin.close();
		for (int i = 0; i < pointcell.size / sizeof(int); i++)
		{
			int nNume = 0;
			memcpy(&nNume, (char*)data + i * sizeof(int), sizeof(int));
			ids.insert(nNume);
		}
		free(data);
		return ids;
	}

	std::set<int> mPostMeshNodeData1::getNodeInMeshLineIDs()
	{
		std::set <int> ids;
		if (pointline.size == 0)
		{
			return ids;
		}
		std::ifstream fin(mAppConfigurationSingleton::getInstance()->getFileConfiguration()->_postPointLineFileName + ".dat", std::ios::binary);
		fin.seekg(pointline.addr, std::ios::beg);
		for (int i = 0; i < pointline.size / sizeof(int); i++)
		{
			int nNum;
			fin.read((char*)&nNum, sizeof(int));
			ids.insert(nNum);
		}
		fin.close();
		return ids;
	}


	bool mPostMeshNodeData1::getNodeIsFreeNode()
	{
		std::set<int> ids = getNodeInMeshIDs();
		return ids.empty();
	}
}