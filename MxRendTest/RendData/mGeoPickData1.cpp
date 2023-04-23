#include "mGeoPickData1.h"

#include "mGeoModelData1.h"
#include "mGeoPartData1.h"
#include "mGeoPointData1.h"
#include "mGeoLineData1.h"
#include "mGeoFaceData1.h"
#include "mGeoSolidData1.h"
namespace MDataGeo
{
	mGeoPickData1::mGeoPickData1()
	{
		
	}

	mGeoPickData1::~mGeoPickData1()
	{

	}

	void mGeoPickData1::setGeoPickFunction(int pickfunction)
	{
		_pickFunction = pickfunction;
	}

	void mGeoPickData1::setSoloPickGeoPointData(int Pointid, float depth)
	{
		if (Pointid != 0)
		{
			if (depth < _pointBuffer.depth)
			{
				_pointBuffer.depth = depth;
				_pointBuffer.id = Pointid;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoLineData(int Lineid, float depth)
	{
		if (Lineid != 0)
		{
			if (depth < _lineBuffer.depth)
			{
				_lineBuffer.depth = depth;
				_lineBuffer.id = Lineid;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoFaceData(int Faceid, float depth)
	{
		if (Faceid != 0)
		{
			if (depth < _faceBuffer.depth)
			{
				_faceBuffer.depth = depth;
				_faceBuffer.id = Faceid;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoSolidData(int Solidid, float depth)
	{
		if (Solidid != 0)
		{
			if (depth < _solidBuffer.depth)
			{
				_solidBuffer.depth = depth;
				_solidBuffer.id = Solidid;
			}
		}
	}

	void mGeoPickData1::setSoloPickVertexOnGeoLineData(int geoLineID, QVector3D vertex, float depth)
	{
		if (geoLineID != 0)
		{
			if (depth < _vertexOnLineBuffer.depth)
			{
				_vertexOnLineBuffer.depth = depth;
				_vertexOnLineBuffer.geoLineID = geoLineID;
				_vertexOnLineBuffer.vertex = vertex;
			}
		}
	}

	void mGeoPickData1::setSoloPickVertexOnGeoFaceData(int geoFaceID, QVector3D vertex, float depth)
	{
		if (geoFaceID != 0)
		{
			if (depth < _vertexOnFaceBuffer.depth)
			{
				_vertexOnFaceBuffer.depth = depth;
				_vertexOnFaceBuffer.geoFaceID = geoFaceID;
				_vertexOnFaceBuffer.vertex = vertex;
			}
		}
	}

	void mGeoPickData1::setSoloPickVertexOnScreenData(QVector3D vertex)
	{
		_pickVertexsOnScreen.append(vertex);
	}

	void mGeoPickData1::setSoloPickGeoPointByPartData(set<int> Pointids, float depth)
	{
		if (Pointids.size() != 0)
		{
			if (depth < _pointBuffers.depth)
			{
				_pointBuffers.depth = depth;
				_pointBuffers.ids = Pointids;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoLineByPartData(set<int> Lineids, float depth)
	{
		if (Lineids.size() != 0)
		{
			if (depth < _lineBuffers.depth)
			{
				_lineBuffers.depth = depth;
				_lineBuffers.ids = Lineids;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoFaceByPartData(set<int> Faceids, float depth)
	{
		if (Faceids.size() != 0)
		{
			if (depth < _faceBuffers.depth)
			{
				_faceBuffers.depth = depth;
				_faceBuffers.ids = Faceids;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoSolidByPartData(set<int> Solidids, float depth)
	{
		if (Solidids.size()!=0)
		{
			if (depth < _solidBuffers.depth)
			{
				_solidBuffers.depth = depth;
				_solidBuffers.ids = Solidids;
			}
		}
	}

	void mGeoPickData1::setSoloPickGeoPartData(QString partName, float depth)
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

	void mGeoPickData1::setSoloPickGeoSolidByPartData(QString PartName, float depth)
	{
		if (PartName != 0)
		{
			if (depth < _partSolidBuffer.depth)
			{
				_partSolidBuffer.depth = depth;
				_partSolidBuffer.name = PartName;
			}
		}
	}

	void mGeoPickData1::setMutiplyPickGeoPointData(set<int> Pointids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = Pointids.begin(); iter != Pointids.end(); ++iter)
			{
				_pickPoints.insert(*iter);
			}
		}
		else
		{
			for (auto iter = Pointids.begin(); iter != Pointids.end(); ++iter)
			{
				_pickPoints.erase(*iter);
			}
		}
	}

	void mGeoPickData1::setMutiplyPickGeoLineData(set<int> Lineids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = Lineids.begin(); iter != Lineids.end(); ++iter)
			{
				_pickLines.insert(*iter);
			}
		}
		else
		{
			for (auto iter = Lineids.begin(); iter != Lineids.end(); ++iter)
			{
				_pickLines.erase(*iter);
			}
		}
	}

	void mGeoPickData1::setMutiplyPickGeoFaceData(set<int> Faceids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = Faceids.begin(); iter != Faceids.end(); ++iter)
			{
				_pickFaces.insert(*iter);
			}
		}
		else
		{
			for (auto iter = Faceids.begin(); iter != Faceids.end(); ++iter)
			{
				_pickFaces.erase(*iter);
			}
		}
	}

	void mGeoPickData1::setMutiplyPickGeoSolidData(set<int> Solidids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = Solidids.begin(); iter != Solidids.end(); ++iter)
			{
				_pickSolids.insert(*iter);
			}
		}
		else
		{
			for (auto iter = Solidids.begin(); iter != Solidids.end(); ++iter)
			{
				_pickSolids.erase(*iter);
			}
		}
	}

	void mGeoPickData1::setMutiplyPickGeoPartData(QString partName)
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

	void mGeoPickData1::setAllPickGeoPointData(set<int> Pointids)
	{
		_pickPoints = Pointids;
	}

	void mGeoPickData1::setAllPickGeoLineData(set<int> Lineids)
	{
		_pickLines = Lineids;
	}

	void mGeoPickData1::setAllPickGeoFaceData(set<int> Faceids)
	{
		_pickFaces = Faceids;
	}

	void mGeoPickData1::setAllPickGeoSolidData(set<int> Solidids)
	{
		_pickSolids = Solidids;
	}

	void mGeoPickData1::setAllPickGeoPartData(set<QString> partNames)
	{
		_pickParts = partNames;
	}

	void mGeoPickData1::setAddPickGeoPointData(set<int> Pointids)
	{
		_pickPoints.insert(Pointids.begin(), Pointids.end());
	}

	void mGeoPickData1::setAddPickGeoLineData(set<int> Lineids)
	{
		_pickLines.insert(Lineids.begin(), Lineids.end());
	}

	void mGeoPickData1::setAddPickGeoFaceData(set<int> Faceids)
	{
		_pickFaces.insert(Faceids.begin(), Faceids.end());
	}

	void mGeoPickData1::setAddPickGeoSolidData(set<int> Solidids)
	{
		_pickSolids.insert(Solidids.begin(), Solidids.end());
	}

	void mGeoPickData1::setAddPickGeoPartData(set<QString> partNames)
	{
		_pickParts.insert(partNames.begin(), partNames.end());
	}

	void mGeoPickData1::setReducePickGeoPointData(set<int> Pointids)
	{
		for (auto iter = Pointids.begin(); iter != Pointids.end(); ++iter)
		{
			_pickPoints.erase(*iter);
		}
	}

	void mGeoPickData1::setReducePickGeoLineData(set<int> Lineids)
	{
		for (auto iter = Lineids.begin(); iter != Lineids.end(); ++iter)
		{
			_pickLines.erase(*iter);
		}
	}

	void mGeoPickData1::setReducePickGeoFaceData(set<int> Faceids)
	{
		for (auto iter = Faceids.begin(); iter != Faceids.end(); ++iter)
		{
			_pickFaces.erase(*iter);
		}
	}

	void mGeoPickData1::setReducePickGeoSolidData(set<int> Solidids)
	{
		for (auto iter = Solidids.begin(); iter != Solidids.end(); ++iter)
		{
			_pickSolids.erase(*iter);
		}
	}

	void mGeoPickData1::setReducePickGeoPartData(set<QString> partNames)
	{
		for (auto iter = partNames.begin(); iter != partNames.end(); ++iter)
		{
			_pickParts.erase(*iter);
		}
	}

	void mGeoPickData1::setSoloPickData()
	{
		if (_pointBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickPoints.insert(_pointBuffer.id); break;
			case 1:
				_pickPoints.erase(_pointBuffer.id); break;
			}

			_pointBuffer.initial();
		}
		if (_lineBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickLines.insert(_lineBuffer.id); break;
			case 1:
				_pickLines.erase(_lineBuffer.id); break;
			}

			_lineBuffer.initial();
		}
		if (_faceBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickFaces.insert(_faceBuffer.id); break;
			case 1:
				_pickFaces.erase(_faceBuffer.id); break;
			}
			_faceBuffer.initial();
		}
		if (_solidBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickSolids.insert(_solidBuffer.id); break;
			case 1:
				_pickSolids.erase(_solidBuffer.id); break;
			}
			_solidBuffer.initial();
		}
		if (_partBuffer.name != 0)
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
		if (_vertexOnLineBuffer.geoLineID != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickVertexsOnLines[_vertexOnLineBuffer.geoLineID].append(_vertexOnLineBuffer.vertex); break;
			case 1:
				_pickVertexsOnLines[_vertexOnLineBuffer.geoLineID].removeOne(_vertexOnLineBuffer.vertex); break;
			}
			_vertexOnLineBuffer.initial();
		}
		if (_vertexOnFaceBuffer.geoFaceID != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickVertexsOnFaces[_vertexOnFaceBuffer.geoFaceID].append(_vertexOnFaceBuffer.vertex); break;
			case 1:
				_pickVertexsOnFaces[_vertexOnFaceBuffer.geoFaceID].removeOne(_vertexOnFaceBuffer.vertex); break;
			}
			_vertexOnFaceBuffer.initial();
		}
		if (_pointBuffers.ids.size() != 0)
		{
			setMutiplyPickGeoPointData(_pointBuffers.ids);
			_pointBuffers.initial();
		}
		if (_lineBuffers.ids.size() != 0)
		{
			setMutiplyPickGeoLineData(_lineBuffers.ids);
			_lineBuffers.initial();
		}
		if (_faceBuffers.ids.size()!= 0)
		{
			setMutiplyPickGeoFaceData(_faceBuffers.ids);
			_faceBuffers.initial();
		}
		if (_solidBuffers.ids.size() != 0)
		{
			setMutiplyPickGeoSolidData(_solidBuffers.ids);
			_solidBuffers.initial();
		}
	}

	set<int> mGeoPickData1::getPickPointIDs()
	{
		return _pickPoints;
	}

	set<int> mGeoPickData1::getPickLineIDs()
	{
		return _pickLines;
	}

	set<int> mGeoPickData1::getPickFaceIDs()
	{
		return _pickFaces;
	}

	set<int> mGeoPickData1::getPickSolidIDs()
	{
		return _pickSolids;
	}
	set<QString> mGeoPickData1::getPickGeoPartNames()
	{
		return _pickParts;
	}
	QHash<int, QVector<QVector3D>> mGeoPickData1::getPickVertexOnGeoLineDatas()
	{
		return _pickVertexsOnLines;
	}
	QHash<int, QVector<QVector3D>> mGeoPickData1::getPickVertexOnGeoFaceDatas()
	{
		return _pickVertexsOnFaces;
	}
	QVector<QVector3D> mGeoPickData1::getPickVertexOnScreenDatas()
	{
		return _pickVertexsOnScreen;
	}
	void mGeoPickData1::clearAllPickData()
	{
		_pickPoints.clear();
		_pickLines.clear();
		_pickFaces.clear();
		_pickSolids.clear();
		_pickParts.clear();
		_pickVertexsOnLines.clear();
		_pickVertexsOnFaces.clear();
		_pickVertexsOnScreen.clear();
	}
}
