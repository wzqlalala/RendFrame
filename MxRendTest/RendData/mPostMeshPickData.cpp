#include "mPostMeshPickData.h"

namespace MDataPost
{
	mPostMeshPickData::mPostMeshPickData()
	{
		_pickFunction = 0;
	}
	mPostMeshPickData::~mPostMeshPickData()
	{

	}

	void mPostMeshPickData::setMeshPickFunction(int pickfunction)
	{
		_pickFunction = pickfunction;
	}

	void mPostMeshPickData::setSoloPickNodeData(int nodeid, float depth)
	{
		if (nodeid != 0)
		{
			if (depth < _nodeBuffer.depth)
			{
				_nodeBuffer.depth = depth;
				_nodeBuffer.id = nodeid;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshData(int meshid, float depth)
	{
		if (meshid != 0)
		{
			if (depth < _meshBuffer.depth)
			{
				_meshBuffer.depth = depth;
				_meshBuffer.id = meshid;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshFaceData(int meshfaceid, float depth)
	{
		if (meshfaceid != 0)
		{
			if (depth < _meshFaceBuffer.depth)
			{
				_meshFaceBuffer.depth = depth;
				_meshFaceBuffer.id = meshfaceid;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshPartData(QString partName, float depth)
	{
		if (partName.size() != 0)
		{
			if (depth < _partBuffer.depth)
			{
				_partBuffer.depth = depth;
				_partBuffer.name = partName;
			}
		}
	}

	void mPostMeshPickData::setSoloPickNodeByPartData(set<int> nodeids, float depth)
	{
		if (nodeids.size() != 0)
		{
			if (depth < _nodeBuffers.depth)
			{
				_nodeBuffers.depth = depth;
				_nodeBuffers.ids = nodeids;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshByPartData(set<int> meshids, float depth)
	{
		if (meshids.size() != 0)
		{
			if (depth < _meshBuffers.depth)
			{
				_meshBuffers.depth = depth;
				_meshBuffers.ids = meshids;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshDataByAngle(int meshid, QString partName, float depth)
	{
		if (meshid != 0)
		{
			if (depth < _meshPartNameBuffers.depth)
			{
				_meshPartNameBuffers.depth = depth;
				_meshPartNameBuffers.partName = partName;
				_meshPartNameBuffers.id = meshid;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshLineDataByAngle(int meshlineid, QString partName, float depth)
	{
		if (meshlineid != 0)
		{
			if (depth < _meshLinePartNameBuffers.depth)
			{
				_meshLinePartNameBuffers.depth = depth;
				_meshLinePartNameBuffers.partName = partName;
				_meshLinePartNameBuffers.id = meshlineid;
			}
		}
	}

	void mPostMeshPickData::setSoloPickMeshFaceDataByAngle(int meshfaceid, QString partName, float depth)
	{
		if (meshfaceid != 0)
		{
			if (depth < _meshFacePartNameBuffers.depth)
			{
				_meshFacePartNameBuffers.depth = depth;
				_meshFacePartNameBuffers.partName = partName;
				_meshFacePartNameBuffers.id = meshfaceid;
			}
		}
	}

	void mPostMeshPickData::setSoloPickData()
	{
		if (_nodeBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickNodes.insert(_nodeBuffer.id); break;
			case 1:
				_pickNodes.erase(_nodeBuffer.id); break;
			}

			_nodeBuffer.initial();
		}
		if (_meshBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickMeshs.insert(_meshBuffer.id); break;
			case 1:
				_pickMeshs.erase(_meshBuffer.id); break;
			}

			_meshBuffer.initial();
		}
		if (_meshFaceBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickMeshFaces.insert(_meshFaceBuffer.id); break;
			case 1:
				_pickMeshFaces.erase(_meshFaceBuffer.id); break;
			}
			_meshFaceBuffer.initial();
		}
		if (_partBuffer.name.size() != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickParts.insert(_partBuffer.name); break;
			case 1:
				_pickParts.erase(_partBuffer.name); break;
			}
			_partBuffer.initial();
		}
		if (_nodeBuffers.ids.size() != 0)
		{
			setMultiplyPickNodeData(_nodeBuffers.ids);
			_nodeBuffers.initial();
		}
		if (_meshBuffers.ids.size() != 0)
		{
			setMultiplyPickMeshData(_meshBuffers.ids);
			_meshBuffers.initial();
		}
		if (_meshFaceBuffers.ids.size() != 0)
		{
			setMultiplyPickMeshFaceData(_meshFaceBuffers.ids);
			_meshFaceBuffers.initial();
		}
	}

	void mPostMeshPickData::setSoloOrderPickData()
	{
		if (_nodeBuffer.id != 0)
		{
			_pickNodesOrder.removeOne(_nodeBuffer.id);
			switch (_pickFunction)
			{
			case 0:
				_pickNodesOrder.append(_nodeBuffer.id); break;
			case 1:
				break;
			}
			_nodeBuffer.initial();
		}
	}

	void mPostMeshPickData::setMultiplyPickNodeData(std::set<int> nodeids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
			{
				_pickNodes.insert(*iter);
			}
			// set_union(_picknodes.begin(), _picknodes.end(), nodeData.begin(), nodeData.end(), inserter(_picknodes, _picknodes.begin()));
		}
		else
		{
			for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
			{
				_pickNodes.erase(*iter);
			}
		}
	}

	void mPostMeshPickData::setMultiplyPickMeshData(std::set<int> meshids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = meshids.begin(); iter != meshids.end(); ++iter)
			{
				_pickMeshs.insert(*iter);
			}
		}
		else
		{
			for (auto iter = meshids.begin(); iter != meshids.end(); ++iter)
			{
				_pickMeshs.erase(*iter);
			}
		}
	}

	void mPostMeshPickData::setMultiplyPickMeshFaceData(set<int> meshfaceids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
			{
				_pickMeshFaces.insert(*iter);
			}
		}
		else
		{
			for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
			{
				_pickMeshFaces.erase(*iter);
			}
		}
	}

	void mPostMeshPickData::setMultiplyPickMeshPartData(QString partName)
	{
		if (_pickFunction == 0)
		{
			_pickParts.insert(partName);
		}
		else
		{
			_pickParts.erase(partName);
		}
	}

	void mPostMeshPickData::setAllPickNodeData(set<int> nodeids)
	{
		_pickNodes = nodeids;
	}

	void mPostMeshPickData::setAllPickMeshData(set<int> meshids)
	{
		_pickMeshs = meshids;
	}

	void mPostMeshPickData::setAllPickMeshFaceData(set<int> meshfaceids)
	{
		_pickMeshFaces = meshfaceids;
	}

	void mPostMeshPickData::setAllPickMeshPartData(set<QString> partNames)
	{
		_pickParts = partNames;
	}

	void mPostMeshPickData::setAllPickAnyPosition(QVector3D vertex, float value)
	{
		_pickAnyPosition.clear();
		_pickAnyPosition.append(QVector4D(vertex,value));
	}

	void mPostMeshPickData::setAddPickNodeData(set<int> nodeids)
	{
		_pickNodes.insert(nodeids.begin(), nodeids.end());
	}

	void mPostMeshPickData::setAddPickMeshData(set<int> meshids)
	{
		_pickMeshs.insert(meshids.begin(), meshids.end());
	}

	void mPostMeshPickData::setAddPickMeshFaceData(set<int> meshfaceids)
	{
		_pickMeshFaces.insert(meshfaceids.begin(), meshfaceids.end());
	}

	void mPostMeshPickData::setAddPickMeshPartData(set<QString> partNames)
	{
		_pickParts.insert(partNames.begin(), partNames.end());
	}

	void mPostMeshPickData::setAddPickAnyPosition(QVector3D vertex, float value)
	{
		_pickAnyPosition.append(QVector4D(vertex, value));
	}

	void mPostMeshPickData::setReducePickNodeData(set<int> nodeids)
	{
		for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
		{
			_pickNodes.erase(*iter);
		}
	}

	void mPostMeshPickData::setReducePickMeshData(set<int> meshids)
	{
		for (auto iter = meshids.begin(); iter != meshids.end(); ++iter)
		{
			_pickMeshs.erase(*iter);
		}
	}

	void mPostMeshPickData::setReducePickMeshFaceData(set<int> meshfaceids)
	{
		for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
		{
			_pickMeshFaces.erase(*iter);
		}
	}

	void mPostMeshPickData::setReducePickMeshPartData(set<QString> partNames)
	{
		for (auto iter = partNames.begin(); iter != partNames.end(); ++iter)
		{
			_pickParts.erase(*iter);
		}
	}

	set<int> mPostMeshPickData::getPickNodeIDs()
	{
		return _pickNodes;
	}

	QVector<int> mPostMeshPickData::getPickNodeIDsOrder()
	{
		return _pickNodesOrder;
	}

	set<int> mPostMeshPickData::getPickMeshIDs()
	{
		return _pickMeshs;
	}

	set<int> mPostMeshPickData::getPickMeshFaceIDs()
	{
		return _pickMeshFaces;
	}

	set<QString> mPostMeshPickData::getPickMeshPartNames()
	{
		return _pickParts;
	}

	QVector<QVector4D> mPostMeshPickData::getAnyPositionValue()
	{
		return _pickAnyPosition;
	}

	void mPostMeshPickData::clearAllPickData()
	{
		_pickNodes.clear();
		_pickNodesOrder.clear();
		_pickMeshs.clear();
		_pickMeshFaces.clear();
		_pickParts.clear();
		_pickAnyPosition.clear();
	}

	void mPostMeshPickData::clearPickNodeData()
	{
		_pickNodes.clear();
		_pickNodesOrder.clear();
	}

	void mPostMeshPickData::clearPickMeshData()
	{
		_pickMeshs.clear();
	}

	void mPostMeshPickData::clearPickMeshFaceData()
	{
		_pickMeshFaces.clear();
	}

	void mPostMeshPickData::clearPickMeshPartData()
	{
		_pickParts.clear();
	}

	QPair<PickObjectType, QPair<QString, int>> mPostMeshPickData::getSoloPickNodeDataByLineAngle()
	{
		//PickObjectType objectType = PickObjectType::Mesh1D;
		//QString partName = _meshPartNameBuffers.partName;
		//int id = _meshPartNameBuffers.id;
		//if (_meshLinePartNameBuffers.id != 0 && _meshPartNameBuffers.id != 0)
		//{
		//	if (_meshLinePartNameBuffers.depth < _meshPartNameBuffers.depth)
		//	{
		//		objectType = MeshEdge;
		//		partName = _meshLinePartNameBuffers.partName;
		//		id = _meshLinePartNameBuffers.id;
		//	}
		//}
		//else if (_meshLinePartNameBuffers.id != 0)
		//{
		//	objectType = MeshEdge;
		//	partName = _meshLinePartNameBuffers.partName;
		//	id = _meshLinePartNameBuffers.id;
		//}
		//_meshPartNameBuffers.initial();
		//_meshLinePartNameBuffers.initial();
		//return QPair<PickObjectType, QPair<QString, int>>(objectType, { partName, id });
		return QPair<PickObjectType, QPair<QString, int>>();
	}
	QPair<PickObjectType, QPair<QString, int>> mPostMeshPickData::getSoloPickNodeDataByFaceAngle()
	{
		PickObjectType objectType = PickObjectType::Mesh2D;
		QString partName = _meshPartNameBuffers.partName;
		int id = _meshPartNameBuffers.id;
		if (_meshFacePartNameBuffers.id != 0 && _meshPartNameBuffers.id != 0)
		{
			if (_meshFacePartNameBuffers.depth < _meshPartNameBuffers.depth)
			{
				objectType = PickObjectType::MeshFace;
				partName = _meshFacePartNameBuffers.partName;
				id = _meshFacePartNameBuffers.id;
			}
		}
		else if (_meshFacePartNameBuffers.id != 0)
		{
			objectType = PickObjectType::MeshFace;
			partName = _meshFacePartNameBuffers.partName;
			id = _meshFacePartNameBuffers.id;
		}
		_meshPartNameBuffers.initial();
		_meshFacePartNameBuffers.initial();
		return QPair<PickObjectType, QPair<QString, int>>(objectType, { partName, id });
	}
	QPair<QString, int> mPostMeshPickData::getSoloPickMeshDataByAngle()
	{
		QPair<QString, int> pair(_meshPartNameBuffers.partName, _meshPartNameBuffers.id);
		_meshPartNameBuffers.initial();
		return pair;
	}
	QPair<QString, int> mPostMeshPickData::getSoloPickMeshLineDataByAngle()
	{
		//QPair<QString, int> pair(_meshLinePartNameBuffers.partName, _meshLinePartNameBuffers.id);
		//_meshLinePartNameBuffers.initial();
		//return pair;
		return QPair<QString, int>();
	}
	QPair<QString, int> mPostMeshPickData::getSoloPickMeshFaceDataByAngle()
	{
		QPair<QString, int> pair(_meshFacePartNameBuffers.partName, _meshFacePartNameBuffers.id);
		_meshFacePartNameBuffers.initial();
		return pair;
	}
}