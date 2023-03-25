#include "mOneFrameData1.h"


#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"

//Qt
#include <QTime>
#include <qguiapplication.h>

//MAppconfinguration
#include "mAppConfigurationSingleton.h"
#include "mFileConfiguration.h"

#include "mContainers.h"
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;
using namespace MBasicFunction;
using namespace MAppConfiguration;
namespace MDataPost
{
	mOneFrameData1::mOneFrameData1()
	{
		_hasModalValue = false;
	}

	mOneFrameData1::~mOneFrameData1()
	{
		deleteOneFrameData();
	}

	void mOneFrameData1::calculatePointCell()
	{
		std::unordered_map<int, std::vector<int>> pcells;
		for (auto p : _meshData1)
		{
			if (p == nullptr)
			{
				continue;
			}
			QVector<int> s = p->getNodeIndex();
			int mId = p->getMeshID();
			for (int i = 0; i < s.size(); i++)
			{
				pcells[s[i]].emplace_back(mId);
			}
		}
		for (auto p : _meshData2)
		{
			if (p == nullptr)
			{
				continue;
			}
			QVector<int> s = p->getNodeIndex();
			int mId = p->getMeshID();
			for (int i = 0; i < s.size(); i++)
			{
				pcells[s[i]].emplace_back(mId);
			}
		}


		std::ofstream fout(mAppConfigurationSingleton::getInstance()->getFileConfiguration()->_postPointCellFileName + ".dat", std::ios::binary | std::ios::trunc);
		int begin_size = 0;
		for (auto &item : pcells)
		{
			for (auto j : item.second)
			{
				fout.write((char*)&j, sizeof(int));
			}
			int size = item.second.size() * sizeof(int);
			getNodeDataByID(item.first)->pointcell.addr = begin_size;
			getNodeDataByID(item.first)->pointcell.size = size;
			begin_size += size;

		}
		fout.close();
		pcells.clear();

	}

	void mOneFrameData1::resizeNodeSize(int size)
	{
		_nodeData2.resize(size+1);
	}

	void mOneFrameData1::resizeCopyCellSize(int size)
	{
		
		int bef = _meshData2.size();
		_meshData2.resize(size + bef+1);
		//_meshData2;
	}

	void mOneFrameData1::calculateCellData()
	{
		QVector<mPostMeshData1*> _meshtemp;
		_meshtemp.swap(_meshData2);
		_meshData2.resize(_meshtemp.size());
		for (auto item : _meshtemp)
		{
			int id = item->getMeshID();
			if (id < _meshData2.size())
			{
				_meshData2[id] = item;
			}
			else
			{
				_meshData1[id] = item;
			}
		}

	}

	void mOneFrameData1::caculateMeshModelRadiusAndCenter()
	{
		//太慢排序
		//set<float> x, y, z;
		//QHashIterator<int, mMeshNodeData1*> iter(_nodeData1);
		//while (iter.hasNext())
		//{
		//	iter.next();
		//	x.insert(iter.value()->getNodeVertex().x());
		//	y.insert(iter.value()->getNodeVertex().y());
		//	z.insert(iter.value()->getNodeVertex().z());
		//}
		//if (x.size() != 0)
		//{
		//	float xmin = *x.begin();
		//	float ymin = *y.begin();
		//	float zmin = *z.begin();
		//	float xmax = *(--x.end());
		//	float ymax = *(--y.end());
		//	float zmax = *(--z.end());
		//	_modelCenter = QVector3D{ (xmin + xmax) / 2,(ymin + ymax) / 2, (zmin + zmax) / 2 };
		//	_modelRadius = std::sqrt(pow((xmax - xmin) / 2, 2) + pow((ymax - ymin) / 2, 2) + pow((zmax - zmin) / 2, 2));
		//}
		//else
		//{
		//	_modelCenter = QVector3D(0, 0, 0);
		//	_modelRadius = 0;
		//}

		/*
		QTime time;
		time.start();

		float xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
		if (!_nodeData1.isEmpty())
		{
			QHashIterator<int, mPostMeshNodeData1*> iter(_nodeData1);
			while (iter.hasNext())
			{
				iter.next();
				xmin = xmax = iter.value()->getNodeVertex().x();
				ymin = ymax = iter.value()->getNodeVertex().y();
				zmin = zmax = iter.value()->getNodeVertex().z();
				break;
			}
		}
		QHashIterator<int, mPostMeshNodeData1*> iternode(_nodeData1);
		while (iternode.hasNext())
		{
			iternode.next();
			xmin = min(xmin, iternode.value()->getNodeVertex().x());
			xmax = max(xmax, iternode.value()->getNodeVertex().x());
			ymin = min(ymin, iternode.value()->getNodeVertex().y());
			ymax = max(ymax, iternode.value()->getNodeVertex().y());
			zmin = min(zmin, iternode.value()->getNodeVertex().z());
			zmax = max(zmax, iternode.value()->getNodeVertex().z());
		}
		_modelSize = ModelSize(xmin, ymin, zmin, xmax, ymax, zmax);
		*/





		//_modelCenter = QVector3D{ (xmin + xmax) / 2,(ymin + ymax) / 2, (zmin + zmax) / 2 };
		//_modelRadius = std::sqrt(pow((xmax - xmin) / 2, 2) + pow((ymax - ymin) / 2, 2) + pow((zmax - zmin) / 2, 2));

		//qDebug() << "runtime " << time.elapsed();
	}

	void mOneFrameData1::caculateMeshSize()
	{
		bool isIn = false;
		QHashIterator<int, mPostMeshData1*> itermesh(_meshData1);
		while (itermesh.hasNext())
		{
			itermesh.next();

			if (itermesh.value()->getMeshType() == MBasicFunction::MeshPoint)
			{
				continue;
			}
			QVector3D node1 = getNodeDataByID(itermesh.value()->getNodeIndex().at(0))->getNodeVertex();
			QVector3D node2 = getNodeDataByID(itermesh.value()->getNodeIndex().at(1))->getNodeVertex();
			_meshSize = node1.distanceToPoint(node2);
			isIn = true;
			break;
		}
		if (isIn)
		{
			return;
		}
		for (mPostMeshData1* meshData: _meshData2)
		{
			if (meshData == nullptr|| meshData->getMeshType() == MBasicFunction::MeshPoint)
			{
				continue;
			}
			QVector3D node1 = getNodeDataByID(meshData->getNodeIndex().at(0))->getNodeVertex();
			QVector3D node2 = getNodeDataByID(meshData->getNodeIndex().at(1))->getNodeVertex();
			_meshSize = node1.distanceToPoint(node2);
			break;
		}
	}

	void mOneFrameData1::caculateMeshLine()
	{
		QStringList partNames = _partData1.keys();
		for (int a = 0; a < partNames.size(); ++a)
		{
			QString partName = partNames.at(a);
			mPostMeshPartData1 *partData = this->getMeshPartDataByPartName(partName);

			//判断三维网格的表面的单元线
			QMultiHash<QSet<int>, mPostMeshFaceData1*> lines1;//存单元线和它所在的单元面
			QVector<mPostMeshFaceData1*> meshFaceIDDatas = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceIDDatas)
			{
				//由于读取文件后不存在新生成的表面的单元面，所以这儿只考虑最原始的表面的单元面
				if (meshFaceData != nullptr && meshFaceData->getVisual())
				{
					const QVector<int> &indexs = meshFaceData->getNodeIndex();
					//int meshFaceID = meshFaceData->getMeshFaceID();
					if (indexs.size() == 3)
					{
						lines1.insert(QSet<int>{indexs.at(0), indexs.at(1)}, meshFaceData);
						lines1.insert(QSet<int>{indexs.at(0), indexs.at(2)}, meshFaceData);
						lines1.insert(QSet<int>{indexs.at(1), indexs.at(2)}, meshFaceData);
					}
					else if (indexs.size() == 4)
					{
						lines1.insert(QSet<int>{indexs.at(0), indexs.at(1)}, meshFaceData);
						lines1.insert(QSet<int>{indexs.at(1), indexs.at(2)}, meshFaceData);
						lines1.insert(QSet<int>{indexs.at(2), indexs.at(3)}, meshFaceData);
						lines1.insert(QSet<int>{indexs.at(3), indexs.at(0)}, meshFaceData);
					}
				}
			}

			QList<QSet<int>> linelist1 = lines1.keys();
			linelist1 = linelist1.toSet().toList();
			for (int i = 0; i < linelist1.size(); ++i)
			{
				QSet<int> tempset = linelist1.at(i);
				//int count = lines1.count(tempset);
				if (lines1.count(tempset) == 1)//边界线,保留信息
				{
					mPostMeshFaceData1 *meshFaceData = lines1.value(tempset);
					mPostMeshData1 *meshData = this->getMeshDataByID(meshFaceData->getMeshID1());
					_globalMeshLineId++;
					int order = this->judgeMeshLineOrder(meshData, std::set<int>{tempset.begin(), tempset.end()});
					mPostMeshLineData1 *meshLineData = new mPostMeshLineData1(_globalMeshLineId, std::set<int>{tempset.begin(), tempset.end()}, meshData->getMeshID(), order, partName);
					this->appendMeshLineData(_globalMeshLineId, meshLineData);
					partData->appendMeshLineID(_globalMeshLineId);
					//meshData->appendMeshLine(_globalMeshLineId);

					//for (int nodeID : tempset)
					//{
					//	this->getNodeDataByID(nodeID)->addMeshLine(_globalMeshLineId);
					//}
				}
				else if (lines1.count(tempset) == 2)//判断两个单元面的方向是否大于一定度数，如果超过了则设为单元线
				{
					QList<mPostMeshFaceData1*> meshFaceDatas = lines1.values(tempset);
					if (meshFaceDatas.size() == 2)
					{
						mPostMeshFaceData1 *meshFaceData1 = meshFaceDatas.at(0);
						mPostMeshFaceData1 *meshFaceData2 = meshFaceDatas.at(1);
						QVector<int> nodeIDs = meshFaceData1->getNodeIndex();
						QVector<int> nodeIDs1 = meshFaceData2->getNodeIndex();
						if (nodeIDs.size() >= 3 && nodeIDs1.size() >= 3)
						{
							mPostMeshNodeData1* nodeData1 = this->getNodeDataByID(nodeIDs.at(0));
							mPostMeshNodeData1* nodeData2 = this->getNodeDataByID(nodeIDs.at(1));
							mPostMeshNodeData1* nodeData3 = this->getNodeDataByID(nodeIDs.at(2));

							QVector3D direction1 = QVector3D::crossProduct((nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized(), (nodeData3->getNodeVertex() - nodeData2->getNodeVertex()).normalized()).normalized();

							nodeData1 = this->getNodeDataByID(nodeIDs1.at(0));
							nodeData2 = this->getNodeDataByID(nodeIDs1.at(1));
							nodeData3 = this->getNodeDataByID(nodeIDs1.at(2));

							QVector3D direction2 = QVector3D::crossProduct((nodeData2->getNodeVertex() - nodeData1->getNodeVertex()).normalized(), (nodeData3->getNodeVertex() - nodeData2->getNodeVertex()).normalized()).normalized();

							float x = QVector3D::dotProduct(direction1, direction2);

							float angleValue;
							if (x > 1.0)
							{
								angleValue = 0;
							}
							else
							{
								angleValue = fabs(180 * acos(x) / 3.1415926);
							}
							//qDebug() << angleValue << endl;
							if (angleValue > 60)
							{
								_globalMeshLineId++;
								int meshID = meshFaceData1->getMeshID1();
								mPostMeshData1 *meshData = getMeshDataByID(meshID);
								int order = this->judgeMeshLineOrder(meshData, std::set<int>{tempset.begin(), tempset.end()});
								mPostMeshLineData1 *meshLineData = new mPostMeshLineData1(_globalMeshLineId, std::set<int>{tempset.begin(), tempset.end()}, meshID, order, partName);
								this->appendMeshLineData(_globalMeshLineId, meshLineData);
								partData->appendMeshLineID(_globalMeshLineId);
								//meshData->appendMeshLine(_globalMeshLineId);
								//for (int nodeID : tempset)
								//{
								//	mMeshModelData1::getInstance()->getNodeDataByID(nodeID)->addMeshLine(_globalMeshLineId);
								//}
							}
						}

					}
				}
			}
			lines1.clear();

			//判断二维网格的单元线
			QMultiHash<QSet<int>, int> lines;//存单元线和它所在的单元

			QVector<mPostMeshData1*> meshDatas = partData->getMeshDatas2();
			for (mPostMeshData1 *meshData : meshDatas)
			{
				int meshID = meshData->getMeshID();
				if (meshData->getMeshType() == MeshTri)
				{
					lines.insert(QSet<int>{meshData->getNodeIndex().at(0), meshData->getNodeIndex().at(1)}, meshID);
					lines.insert(QSet<int>{meshData->getNodeIndex().at(0), meshData->getNodeIndex().at(2)}, meshID);
					lines.insert(QSet<int>{meshData->getNodeIndex().at(1), meshData->getNodeIndex().at(2)}, meshID);
				}
				else if (meshData->getMeshType() == MeshQuad)
				{
					lines.insert(QSet<int>{meshData->getNodeIndex().at(0), meshData->getNodeIndex().at(1)}, meshID);
					lines.insert(QSet<int>{meshData->getNodeIndex().at(1), meshData->getNodeIndex().at(2)}, meshID);
					lines.insert(QSet<int>{meshData->getNodeIndex().at(2), meshData->getNodeIndex().at(3)}, meshID);
					lines.insert(QSet<int>{meshData->getNodeIndex().at(3), meshData->getNodeIndex().at(0)}, meshID);
				}
			}


			QList<QSet<int>> linelist = lines.keys();
			linelist = linelist.toSet().toList();
			for (int i = 0; i < linelist.size(); ++i)
			{
				QSet<int> tempset = linelist.at(i);
				if (lines.count(tempset) == 1)//边界线,保留信息
				{
					mPostMeshData1 *meshData = this->getMeshDataByID(lines.value(tempset));
					_globalMeshLineId++;
					int order = this->judgeMeshLineOrder(meshData, std::set<int>{tempset.begin(), tempset.end()});
					mPostMeshLineData1 *meshLineData = new mPostMeshLineData1(_globalMeshLineId, std::set<int>{tempset.begin(), tempset.end()}, meshData->getMeshID(), order, partName);
					this->appendMeshLineData(_globalMeshLineId, meshLineData);
					partData->appendMeshLineID(_globalMeshLineId);
					//meshData->appendMeshLine(_globalMeshLineId);

					/*for (int nodeID : tempset)
					{
						mMeshModelData1::getInstance()->getNodeDataByID(nodeID)->addMeshLine(_globalMeshLineId);
					}*/
				}
				else if (lines.count(tempset) == 2)//判断两个单元的方向是否大于一定度数，如果超过了则设为单元线
				{
					QList<int> meshIDs = lines.values(tempset);
					if (meshIDs.size() == 2)
					{
						mPostMeshData1 *meshData1 = this->getMeshDataByID(meshIDs.at(0));
						mPostMeshData1 *meshData2 = this->getMeshDataByID(meshIDs.at(1));

						QVector<int> nodeIDs = meshData1->getNodeIndex();
						QVector<int> nodeIDs1 = meshData2->getNodeIndex();
						if (nodeIDs.size() >= 3 && nodeIDs1.size() >= 3)
						{
							mPostMeshNodeData1* nodeData1 = this->getNodeDataByID(nodeIDs.at(0));
							mPostMeshNodeData1* nodeData2 = this->getNodeDataByID(nodeIDs.at(1));
							mPostMeshNodeData1* nodeData3 = this->getNodeDataByID(nodeIDs.at(2));

							QVector3D direction1 = QVector3D::crossProduct(nodeData2->getNodeVertex() - nodeData1->getNodeVertex(), nodeData3->getNodeVertex() - nodeData2->getNodeVertex()).normalized();

							nodeData1 = this->getNodeDataByID(nodeIDs1.at(0));
							nodeData2 = this->getNodeDataByID(nodeIDs1.at(1));
							nodeData3 = this->getNodeDataByID(nodeIDs1.at(2));

							QVector3D direction2 = QVector3D::crossProduct(nodeData2->getNodeVertex() - nodeData1->getNodeVertex(), nodeData3->getNodeVertex() - nodeData2->getNodeVertex()).normalized();

							float x = QVector3D::dotProduct(direction1, direction2);

							float angleValue;
							if (x > 1.0)
							{
								angleValue = 0;
							}
							else
							{
								angleValue = fabs(180 * acos(QVector3D::dotProduct(direction1, direction2)) / 3.1415926);
							}
							//qDebug() << angleValue << endl;
							if (angleValue > 60)
							{
								_globalMeshLineId++;
								int order = this->judgeMeshLineOrder(meshData1, std::set<int>{tempset.begin(), tempset.end()});
								mPostMeshLineData1 *meshLineData = new mPostMeshLineData1(_globalMeshLineId, std::set<int>{tempset.begin(), tempset.end()}, meshData1->getMeshID(), order, partName);
								this->appendMeshLineData(_globalMeshLineId, meshLineData);
								partData->appendMeshLineID(_globalMeshLineId);
								//meshData1->appendMeshLine(_globalMeshLineId);

								//for (int nodeID : tempset)
								//{
								//	this->getNodeDataByID(nodeID)->addMeshLine(_globalMeshLineId);
								//}
							}
						}

					}
				}
			}
			lines.clear();
		}

	}
	

	void mOneFrameData1::generateMeshLine(mPostMeshFaceData1* surfaceID1, mPostMeshFaceData1* surfaceID2, mPostMeshData1* meshData, QString partName, mPostMeshPartData1* partData)
	{
		
		mPostMeshFaceData1 *meshFaceData1 = surfaceID1;
		QVector<int> i1 = meshFaceData1->getNodeIndex();
		std::set<int> index1(i1.begin(), i1.end());
		mPostMeshFaceData1 *meshFaceData2 = surfaceID2;
		QVector<int> i2 = meshFaceData2->getNodeIndex();
		std::set<int> index2(i2.begin(), i2.end());
		std::set<int> result;
		std::set_intersection(index1.begin(), index1.end(), index2.begin(), index2.end(), std::inserter(result, result.begin()));
		if (result.size() == 2)
		{
			_globalMeshLineId++;
			int order = this->judgeMeshLineOrder(meshData, result);
			mPostMeshLineData1 *meshLineData = new mPostMeshLineData1(_globalMeshLineId, result, meshData->getMeshID(), order, partName);
			this->appendMeshLineData(_globalMeshLineId, meshLineData);
			partData->appendMeshLineID(_globalMeshLineId);
			meshData->appendMeshLine(_globalMeshLineId);
		}
	}

	set<int> mOneFrameData1::getAllNodeIDs()
	{
		set<int> ids;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			//三维网格的节点
			set<int> meshIDs = partData->getMeshIDs3();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				/*if (!meshData->getMeshVisual())
				{
					continue;
				}*/
				QVector<int> nodeindex = meshData->getNodeIndex();
				ids.insert(nodeindex.begin(), nodeindex.end());
			}

			//二维网格的节点
			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				/*if (!meshData->getMeshVisual())
				{
					continue;
				}*/
				QVector<int> nodeindex = meshData->getNodeIndex();
				ids.insert(nodeindex.begin(), nodeindex.end());
			}

			//一维网格的节点
			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				/*if (!meshData->getMeshVisual())
				{
					continue;
				}*/
				QVector<int> nodeindex = meshData->getNodeIndex();
				ids.insert(nodeindex.begin(), nodeindex.end());
			}

			//0维网格的节点
			meshIDs = partData->getMeshIDs0();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				/*if (!meshData->getMeshVisual())
				{
					continue;
				}*/
				QVector<int> nodeindex = meshData->getNodeIndex();
				ids.insert(nodeindex.begin(), nodeindex.end());
			}
		}
		return ids;
	}

	set<int> mOneFrameData1::getAllNodeIDsInPart(QString partName)
	{
		set<int> ids;
		mPostMeshPartData1 *partData = _partData1.value(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		//三维网格的节点
		set<int> meshIDs = partData->getMeshIDs3();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			/*if (!meshData->getMeshVisual())
			{
				continue;
			}*/
			QVector<int> nodeindex = meshData->getNodeIndex();
			ids.insert(nodeindex.begin(), nodeindex.end());
		}

		//二维网格的节点
		meshIDs = partData->getMeshIDs2();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			/*if (!meshData->getMeshVisual())
			{
				continue;
			}*/
			QVector<int> nodeindex = meshData->getNodeIndex();
			ids.insert(nodeindex.begin(), nodeindex.end());
		}

		//一维网格的节点
		meshIDs = partData->getMeshIDs1();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			/*if (!meshData->getMeshVisual())
			{
				continue;
			}*/
			QVector<int> nodeindex = meshData->getNodeIndex();
			ids.insert(nodeindex.begin(), nodeindex.end());
		}

		//0维网格的节点
		meshIDs = partData->getMeshIDs0();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			/*if (!meshData->getMeshVisual())
			{
				continue;
			}*/
			QVector<int> nodeindex = meshData->getNodeIndex();
			ids.insert(nodeindex.begin(), nodeindex.end());
		}

		return ids;
	}

	set<int> mOneFrameData1::getAllMeshIDs()
	{
		set<int> ids;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			set<int> meshIDs = partData->getMeshIDs3();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				//if (!meshData->getMeshVisual())
				//{
				//	continue;
				//}
				ids.insert(meshID);
			}

			meshIDs = partData->getMeshIDs2();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				ids.insert(meshID);
			}

			meshIDs = partData->getMeshIDs1();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				ids.insert(meshID);
			}

			meshIDs = partData->getMeshIDs0();
			for (int meshID : meshIDs)
			{
				mPostMeshData1 *meshData = getMeshDataByID(meshID);
				if (meshData == nullptr)
				{
					continue;
				}
				ids.insert(meshID);
			}
		}
		return ids;
	}

	set<int> mOneFrameData1::getAllMeshIDsInPart(QString partName)
	{
		set<int> ids;
		mPostMeshPartData1 *partData = _partData1.value(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		set<int> meshIDs = partData->getMeshIDs3();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			ids.insert(meshID);
		}

		meshIDs = partData->getMeshIDs2();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			ids.insert(meshID);
		}

		meshIDs = partData->getMeshIDs1();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			ids.insert(meshID);
		}

		meshIDs = partData->getMeshIDs0();
		for (int meshID : meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData == nullptr)
			{
				continue;
			}
			if (!meshData->getMeshVisual())
			{
				continue;
			}
			ids.insert(meshID);
		}

		return ids;
	}

	QVector<mPostMeshData1*> mOneFrameData1::getAll3DMeshIDs()
	{
		QVector<mPostMeshData1*> meshDatas;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			//判断单元面
			QVector<mPostMeshData1*> meshs = partData->getMeshDatas3();
			meshDatas.append(meshs);
		}
		return meshDatas;
	}

	QVector<mPostMeshData1*> mOneFrameData1::getAll2DMeshIDs()
	{
		QVector<mPostMeshData1*> meshDatas;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			//判断单元面
			QVector<mPostMeshData1*> meshs = partData->getMeshDatas2();
			meshDatas.append(meshs);
		}
		return meshDatas;
	}

	set<int> mOneFrameData1::getAllMeshFaceIDs()
	{
		set<int> ids;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			//判断单元面
			QVector<mPostMeshFaceData1*> meshFaceDatas = partData->getMeshFaceData();
			for (mPostMeshFaceData1* meshFaceData : meshFaceDatas)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				ids.insert(meshFaceData->getMeshFaceID());
			}
		}
		return ids;
	}

	set<int> mOneFrameData1::getAllMeshFaceIDsInPart(QString partName)
	{
		set<int> ids;
		mPostMeshPartData1 *partData = this->getMeshPartDataByPartName(partName);
		if (partData == nullptr)
		{
			return ids;
		}
		//判断单元面
		QVector<mPostMeshFaceData1*> meshFaceDatas = partData->getMeshFaceData();
		for (mPostMeshFaceData1* meshFaceData : meshFaceDatas)
		{
			if (meshFaceData == nullptr)
			{
				continue;
			}
			ids.insert(meshFaceData->getMeshFaceID());
		}

		return ids;
	}

	QVector<mPostMeshFaceData1*> mOneFrameData1::getAllMeshFaceDatas()
	{
		QVector<mPostMeshFaceData1*> meshFaceDatas;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			//判断单元面
			QVector<mPostMeshFaceData1*> meshFaces = partData->getMeshFaceData();
			meshFaceDatas.append(meshFaces);
		}
		return meshFaceDatas;
	}

	bool mOneFrameData1::isIdExistInModel(int id, const QString & type)
	{
		if (type == QString("节点"))
		{
			if (this->getNodeDataByID(id) != nullptr)
			{
				return true;
			}
		}
		else if (type == QString("单元"))
		{
			if (this->getMeshDataByID(id) != nullptr)
			{
				return true;
			}
		}
		return false;
	}

	mPostMeshPartData1* mOneFrameData1::getMeshPartDataByPartName(QString partName)
	{
		return _partData1.value(partName);
	}

	mPostMeshLineData1* mOneFrameData1::getMeshLineDataByID(int ID)
	{
		return _meshLineData1.value(ID);
	}

	mPostMeshFaceData1* mOneFrameData1::getMeshFaceDataByID(int ID)
	{
		if (ID < _meshFaceData1.size())
		{
			return _meshFaceData1.at(ID);
		}
		return nullptr;
	}

	mPostMeshData1* mOneFrameData1::getMeshDataByID(int ID)
	{
		if (ID < _meshData2.size())
		{
			return _meshData2[ID];
		}
		return _meshData1.value(ID);
	}

	mPostMeshNodeData1* mOneFrameData1::getNodeDataByID(int ID)
	{
		if (ID < _nodeData2.size())
		{
			return _nodeData2[ID];
		}
		return _nodeData1.value(ID);
	}

	QHashIterator<QString, mPostMeshPartData1*> mOneFrameData1::getMeshPartIterator()
	{
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		return iter;
	}

	const QHash<QString, mPostMeshPartData1*> mOneFrameData1::getMeshParts()
	{
		return _partData1;
	}

	void mOneFrameData1::appendMeshPartData(QString partName, mPostMeshPartData1* data)
	{
		
		_partData1.insert(partName, data);
		_oneMeshOperatePartName.insert(partName);
		_isAddOrDeletePart_Mesh = true;
		_meshOperateEnum = AppendOnePart;

		caculateMeshSize();
		//_meshFace.clear();
		//std::map<std::set<int>, mPostMeshFaceData1*>().swap(_meshFace);
	
		
				
	}

	void mOneFrameData1::appendMeshFaceData(int ID, mPostMeshFaceData1* data)
	{
		_meshFaceData1.push_back(data);
	}

	void mOneFrameData1::appendMeshLineData(int ID, mPostMeshLineData1* data)
	{
		_meshLineData1.insert(ID, data);
	}

	void mOneFrameData1::appendMeshData(int ID, mPostMeshData1* data)
	{
		//if (ID < _meshData2.size())
		//{
		//	_meshData2[ID] = data;
		//}
		//else
		//{
		//	_meshData1.insert(ID, data);
		//}
		_meshData2.push_back(data);
	}

	void mOneFrameData1::appendNodeData(int ID, mPostMeshNodeData1* data)
	{
		if (ID < _nodeData2.size())
		{
			_nodeData2[ID] = data;
		}
		else
		{
			_nodeData1.insert(ID, data);
		}
	}

	void mOneFrameData1::createMesh(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType elementType, QVector<int> index, 
		mPostMeshPartData1* partData, QHash<QVector<int>, mPostMeshFaceData1*> &_meshFace)
	{
		QString partName = partData->getPartName();
		mPostMeshData1 *meshData = new mPostMeshData1(ID, meshType, elementType, partName);
		appendMeshData(ID, meshData);

		if (meshType == MeshPoint)
		{
			meshData->setNodeIndex(index);
			partData->appendMesh0(meshData);
		}
		else if (meshType == MeshBeam)
		{
			meshData->setNodeIndex(index);
			partData->appendMesh1(meshData);
		}
		else if (meshType == MeshTri|| meshType == MeshQuad)
		{
			meshData->setNodeIndex(index);
			partData->appendMesh2(meshData);
		}
		else if (meshType == MeshTet)
		{
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(4);
			createMeshFace(QVector<int>{index.at(0), index.at(2), index.at(1)}, QVector<int>{index.at(0), index.at(2), index.at(1)}, meshData, 1, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(3)}, QVector<int>{index.at(0), index.at(1), index.at(3)}, meshData, 2, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(3)}, QVector<int>{index.at(1), index.at(2), index.at(3)}, meshData, 3, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(3), index.at(2)}, QVector<int>{index.at(0), index.at(3), index.at(2)}, meshData, 4, partName, partData, _meshFace);
		
		}
		else if (meshType == MeshWedge)
		{
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(5);
			createMeshFace(QVector<int>{index.at(0), index.at(2), index.at(1)}, QVector<int>{index.at(0), index.at(2), index.at(1)}, meshData, 1, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(3), index.at(4), index.at(5)}, QVector<int>{index.at(3), index.at(4), index.at(5)}, meshData, 2, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(4), index.at(3)}, QVector<int>{index.at(0), index.at(1), index.at(4), index.at(3)}, meshData, 3, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(5), index.at(4)}, QVector<int>{index.at(1), index.at(2), index.at(5), index.at(4)}, meshData, 4, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(2), index.at(0), index.at(3), index.at(5)}, QVector<int>{index.at(2), index.at(0), index.at(3), index.at(5)}, meshData, 5, partName, partData, _meshFace);
		}
		else if (meshType == MeshPyramid)
		{
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(5);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(2), index.at(3)}, QVector<int>{index.at(0), index.at(1), index.at(2), index.at(3)}, meshData, 1, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(4)}, QVector<int>{index.at(1), index.at(2), index.at(4)}, meshData, 2, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(3), index.at(2)}, QVector<int>{index.at(4), index.at(3), index.at(2)}, meshData, 3, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(0), index.at(3)}, QVector<int>{index.at(4), index.at(0), index.at(3)}, meshData, 4, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(4)}, QVector<int>{index.at(0), index.at(1), index.at(4)}, meshData, 5, partName, partData, _meshFace);
		}
		else if (meshType == MeshHex)
		{
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(6);
			createMeshFace(QVector<int>{index.at(0), index.at(3), index.at(2), index.at(1)}, QVector<int>{index.at(0), index.at(3), index.at(2), index.at(1)}, meshData, 1, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(5), index.at(6), index.at(7)}, QVector<int>{index.at(4), index.at(5), index.at(6), index.at(7)}, meshData, 2, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(5), index.at(4)}, QVector<int>{index.at(0), index.at(1), index.at(5), index.at(4)}, meshData, 3, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(6), index.at(5)}, QVector<int>{index.at(1), index.at(2), index.at(6), index.at(5)}, meshData, 4, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(2), index.at(3), index.at(7), index.at(6)}, QVector<int>{index.at(2), index.at(3), index.at(7), index.at(6)}, meshData, 5, partName, partData, _meshFace);
			createMeshFace(QVector<int>{index.at(3), index.at(0), index.at(4), index.at(7)}, QVector<int>{index.at(3), index.at(0), index.at(4), index.at(7)}, meshData, 6, partName, partData, _meshFace);
		}
		_eleNum[elementType]++;
	}

	void mOneFrameData1::createMesh(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType elementType, QVector<int> index, mPostMeshPartData1 * partData, QVector<QVector<QPair<QPair<int,int>, mPostMeshFaceData1*>>>& _meshFace)
	{
		QString partName = partData->getPartName();
		mPostMeshData1 *meshData = new mPostMeshData1(ID, meshType, elementType, partName);
		appendMeshData(ID, meshData);
		switch (meshType)
		{
		case MBasicFunction::MeshPoint:	
			meshData->setNodeIndex(index);
			partData->appendMesh0(meshData);
			break;
		case MBasicFunction::MeshBeam:
			meshData->setNodeIndex(index);
			partData->appendMesh1(meshData);
			break;
		case MBasicFunction::MeshTri:
		case MBasicFunction::MeshQuad:
			meshData->setNodeIndex(index);
			partData->appendMesh2(meshData);
			break;
		case MBasicFunction::MeshTet:
			partData->appendMesh3(meshData);
			createMeshFace(QVector<int>{index.at(0), index.at(2), index.at(1)}, meshData, 1, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(3)}, meshData, 2, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(3)}, meshData, 3, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(3), index.at(2)}, meshData, 4, partName, _meshFace);
			break;
		case MBasicFunction::MeshWedge:
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(5);
			createMeshFace(QVector<int>{index.at(0), index.at(2), index.at(1)}, meshData, 1, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(3), index.at(4), index.at(5)}, meshData, 2, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(4), index.at(3)}, meshData, 3, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(5), index.at(4)}, meshData, 4, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(2), index.at(0), index.at(3), index.at(5)}, meshData, 5, partName, _meshFace);
			break;
		case MBasicFunction::MeshHex:
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(6);
			createMeshFace(QVector<int>{index.at(0), index.at(3), index.at(2), index.at(1)}, meshData, 1, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(5), index.at(6), index.at(7)}, meshData, 2, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(5), index.at(4)}, meshData, 3, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(6), index.at(5)}, meshData, 4, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(2), index.at(3), index.at(7), index.at(6)}, meshData, 5, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(3), index.at(0), index.at(4), index.at(7)}, meshData, 6, partName, _meshFace);
			break;
		case MBasicFunction::MeshPyramid:
			partData->appendMesh3(meshData);
			//meshData->faces.reserve(5);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(2), index.at(3)}, meshData, 1, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(4)}, meshData, 2, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(3), index.at(2)}, meshData, 3, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(0), index.at(3)}, meshData, 4, partName, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(4)}, meshData, 5, partName, _meshFace);
			break;
		default:
			break;
		}
		_eleNum[elementType]++;
	}

	void mOneFrameData1::maskMesh(set<int> meshIDs)
	{
		for (auto iter = meshIDs.begin(); iter != meshIDs.end(); ++iter)
		{
			mPostMeshData1* meshData = getMeshDataByID(*iter);
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->setMeshVisual(false);
		}

		//this->judgeMeshFaceIsShow(meshIDs);
	}

	void mOneFrameData1::showMesh(set<int> meshIDs)
	{
		for (auto iter = meshIDs.begin(); iter != meshIDs.end(); ++iter)
		{
			mPostMeshData1* meshData = getMeshDataByID(*iter);
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->setMeshVisual(true);
		}

		//this->judgeMeshFaceIsShow(meshIDs);
	}

	void mOneFrameData1::reverseMaskMesh()
	{
		for (mPostMeshData1 *meshData : _meshData2)
		{
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->reverseMeshVisual();
		}
		QHashIterator<int, mPostMeshData1*> iter(_meshData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshData1* meshData = iter.value();
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->reverseMeshVisual();
		}

		//this->judgeMeshFaceIsShow();
	}

	void mOneFrameData1::showAllMesh()
	{
		for (mPostMeshData1 *meshData : _meshData2)
		{
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->setMeshVisual(true);
		}
		QHashIterator<int, mPostMeshData1*> iter(_meshData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshData1* meshData = iter.value();
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->setMeshVisual(true);
		}

		//this->judgeMeshFaceIsShow();
	}

	int mOneFrameData1::getNumByElementType(ElementType elementType)
	{
		return _eleNum.value(elementType);
	}

	void mOneFrameData1::createMeshFace(QVector<int> set, QVector<int> ve, mPostMeshData1* meshData, int order,
		QString partName,  mPostMeshPartData1* partData, QHash<QVector<int>, mPostMeshFaceData1*> &_meshFace)
	{
		sort(set.begin(), set.end());
		if (_meshFace.find(set) == _meshFace.end())
		{
			//_globalMeshFaceId++;	

			mPostMeshFaceData1 *meshFaceData1 = new mPostMeshFaceData1(_meshFaceData1.size(), ve, meshData->getMeshID(), order, partName);
			this->appendMeshFaceData(_globalMeshFaceId, meshFaceData1);
			meshData->appendMeshFace(meshFaceData1);
			_meshFace[set] = meshFaceData1;
		}
		else
		{
			mPostMeshFaceData1 *meshFaceData1 = _meshFace[set];
			meshFaceData1->appendMeshID(meshData->getMeshID(), order, ve[0]);
			meshData->appendMeshFace(meshFaceData1);
			_meshFace.remove(set);
			meshFaceData1->setVisual(false);
		}
	}

	void mOneFrameData1::createMeshFace(QVector<int> set, mPostMeshData1 * meshData, int order, QString partName, QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>>& _meshFace)
	{
		auto itermin = min_element(set.begin(), set.end());
		int in = distance(set.begin(),itermin);
		QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>> &face = _meshFace[*itermin];
		bool isNeibu{ false };
		QPair<int, int> pair;
		if (set.size() == 3)
		{
			switch (in)
			{
			case 0:pair.first = set.at(2); pair.second = set.at(1);break;
			case 1:pair.first = set.at(0); pair.second = set.at(2); break;
			case 2:pair.first = set.at(1); pair.second = set.at(0); break;
			default:
				break;
			}
		}
		else
		{
			switch (in)
			{
			case 0:pair.first = set.at(3); pair.second = set.at(1); break;
			case 1:pair.first = set.at(0); pair.second = set.at(2); break;
			case 2:pair.first = set.at(1); pair.second = set.at(3); break;
			case 3:pair.first = set.at(2); pair.second = set.at(0); break;
			default:
				break;
			}
			//for (int i = 0; i < face.size(); ++i)
			//{
			//	QPair<QPair<int, int>, mPostMeshFaceData1*> f = face.at(i);
			//	if (f.first.first == pair.second && f.first.second == pair.first)//是内部面
			//	{
			//		f.second->appendMeshID(meshData->getMeshID(), order, set[0]);
			//		meshData->appendMeshFace(f.second);
			//		//meshData->setMeshFace(order, f.second);
			//		face.removeAt(i);
			//		f.second->setVisual(false);
			//		isNeibu = true;
			//		break;
			//	}
			//}
			//if (!isNeibu)//外部面，重新创建
			//{
			//	mPostMeshFaceData1 *meshFaceData1 = new mPostMeshFaceData1(_meshFaceData1.size(), set, meshData->getMeshID(), order, partName);
			//	this->appendMeshFaceData(0, meshFaceData1);
			//	meshData->appendMeshFace(meshFaceData1);
				//meshData->setMeshFace(order, meshFaceData1);
				//if (in == 0)
				//{
				//	pair.first = set.at(2); pair.second = set.at(3);
				//}
				//else if (in == 1)
				//{
				//	pair.first = set.at(3); pair.second = set.at(0);
				//}
				//else if (in == 2)
				//{
				//	pair.first = set.at(0); pair.second = set.at(1);
				//}
				//else if (in == 3)
				//{
				//	pair.first = set.at(1); pair.second = set.at(2);
				//}
				//_meshFace[*itermin].append({ pair,meshFaceData1 });
			//}
		}
		for (int i = 0, n = face.size(); i < n; ++i)
		{
			QPair<QPair<int, int>, mPostMeshFaceData1*> f = face.at(i);
			if (f.first.first == pair.second && f.first.second == pair.first)//是内部面
			{
				f.second->appendMeshID(meshData->getMeshID(), order, set[0]);
				meshData->appendMeshFace(f.second);
				//meshData->setMeshFace(order, f.second);
				face.removeAt(i);
				f.second->setVisual(false);
				isNeibu = true;
				break;
			}
		}
		if (!isNeibu)//外部面，重新创建
		{
			mPostMeshFaceData1 *meshFaceData1 = new mPostMeshFaceData1(_meshFaceData1.size(), set, meshData->getMeshID(), order, partName);
			this->appendMeshFaceData(0, meshFaceData1);
			meshData->appendMeshFace(meshFaceData1);
			//meshData->setMeshFace(order, meshFaceData1);
			_meshFace[*itermin].append({ pair,meshFaceData1 });
		}

		//sort(set.begin(), set.end());
		//if (_meshFace.find(set) == _meshFace.end())
		//{
		//	//_globalMeshFaceId++;	

		//	mPostMeshFaceData1 *meshFaceData1 = new mPostMeshFaceData1(_meshFaceData1.size(), index, meshData->getMeshID(), order, partName);
		//	this->appendMeshFaceData(_globalMeshFaceId, meshFaceData1);
		//	meshData->appendMeshFace(meshFaceData1);
		//	_meshFace[set] = meshFaceData1;
		//}
		//else
		//{
		//	mPostMeshFaceData1 *meshFaceData1 = _meshFace[set];
		//	meshFaceData1->appendMeshID(meshData->getMeshID(), order, ve[0]);
		//	meshData->appendMeshFace(meshFaceData1);
		//	_meshFace.remove(set);
		//	meshFaceData1->setVisual(false);
		//}
	}

	void mOneFrameData1::eraseNotMeshFace()
	{
		/*
		auto iter = this->getMeshPartIterator();
		while (iter.hasNext())
		{
			iter.next();
			QString partName = iter.key();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}

			if (!partData->getPartVisual())
			{
				continue;
			}

			//三维网格表面
			set<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaces();	
			for(mPostMeshFaceData1 *meshFaceData : meshFaceIDs)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (!this->MeshFaceIsSurfaceIDPointer(meshFaceData))//判断这个单元面是不是外表面,原始文件可能出现共节点的情况，所以需要判断两个部件是否相同
				{
					//不是则移除掉
					delete meshFaceData;
					meshFaceData = nullptr;
					_meshFaceData1.remove(meshFaceData);

					partData->deleteMeshFaceID(meshFaceID);
				}
			}
		}*/
	}

	void mOneFrameData1::judgeMeshFaceIsShow()
	{
		auto iter = this->getMeshPartIterator();
		while (iter.hasNext())
		{
			iter.next();
			QString partName = iter.key();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}

			if (!partData->getPartVisual())
			{
				continue;
			}

			//三维网格表面
			set<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaces();
			for (mPostMeshFaceData1 *meshFaceData : meshFaceIDs)
			{
				
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (!this->MeshFaceIsSurfaceIDPointer(meshFaceData))//判断这个单元面是不是外表面,原始文件可能出现共节点的情况，所以需要判断两个部件是否相同
				{
					meshFaceData->setVisual(false);
				}
				else
				{
					meshFaceData->setVisual(true);
				}
			}
		}
	}

	bool mOneFrameData1::getMeshFaceShow(mPostMeshFaceData1 * mMeshFaceData1, mPostMeshData1* meshData)
	{
		int meshID = meshData->getMeshID();
		if (mMeshFaceData1->getMeshID2() == 0)
		{			
			if (meshData->getMeshVisual())
			{
				//mMeshFaceData1->setVisual(true);
				return true;
			}
		}
		else if (mMeshFaceData1->getMeshID1() == meshID)//第一个面
		{			
			mPostMeshData1 *mMeshData12 = getMeshDataByID(mMeshFaceData1->getMeshID2());
			if (meshData->getMeshVisual() && !mMeshData12->getMeshVisual())
			{
				//mMeshFaceData1->setVisual(true);
				return true;
			}			
		}
		else if(mMeshFaceData1->getMeshID2() == meshID)//第二个面
		{
			mPostMeshData1 *mMeshData12 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			if (meshData->getMeshVisual() && !mMeshData12->getMeshVisual())
			{
				//mMeshFaceData1->setVisual(true);
				return true;
			}
		}
		//mMeshFaceData1->setVisual(false);
		return false;
	}

	void mOneFrameData1::setMeshFaceShow()
	{
		auto iter = this->getMeshPartIterator();
		while (iter.hasNext())
		{
			iter.next();
			QString partName = iter.key();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}

			if (!partData->getPartVisual())
			{
				continue;
			}

			QVector<mPostMeshFaceData1*> face;

			//三维网格表面
			set<mPostMeshFaceData1*> meshFaceIDs = partData->getMeshFaces();
			for (mPostMeshFaceData1 *meshFaceData : meshFaceIDs)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (meshFaceData->getVisual())
				{
					face.append(meshFaceData);
				}
			}

			partData->setMeshFaceData(face);
		}
		
	}

	void mOneFrameData1::judgeMeshFaceIsShow(set<int> meshIDs)
	{
		for (int meshID :meshIDs)
		{
			mPostMeshData1 *meshData = getMeshDataByID(meshID);
			if (meshData== nullptr)
			{
				continue;
			}
			//三维网格表面
			QVector<mPostMeshFaceData1 *> meshFaceIDs = meshData->getFace();
			for (mPostMeshFaceData1 *meshFaceData : meshFaceIDs)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (!this->MeshFaceIsSurfaceIDPointer(meshFaceData))//判断这个单元面是不是外表面,原始文件可能出现共节点的情况，所以需要判断两个部件是否相同
				{
					meshFaceData->setVisual(false);
				}
				else
				{
					meshFaceData->setVisual(true);
				}
			}
		}
	}

	int mOneFrameData1::judgeMeshLineOrder(mPostMeshData1* meshData, std::set<int> index)
	{
		if (meshData == nullptr)
		{
			return 0;
		}
		QVector<int> meshIndex = meshData->getNodeIndex();
		if (meshData->getMeshType() == MeshTri)
		{
			if (index == set<int>{meshIndex[0], meshIndex[1]})
			{
				return 1;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[2]})
			{
				return 2;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[0]})
			{
				return 3;
			}
		}
		else if (meshData->getMeshType() == MeshQuad)
		{
			if (index == set<int>{meshIndex[0], meshIndex[1]})
			{
				return 1;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[2]})
			{
				return 2;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[3]})
			{
				return 3;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[0]})
			{
				return 4;
			}
		}
		else if (meshData->getMeshType() == MeshTet)
		{
			if (index == set<int>{meshIndex[0], meshIndex[1]})
			{
				return 1;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[2]})
			{
				return 2;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[0]})
			{
				return 3;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[0]})
			{
				return 4;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[3]})
			{
				return 5;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[3]})
			{
				return 6;
			}
		}
		else if (meshData->getMeshType() == MeshWedge)
		{
			if (index == set<int>{meshIndex[0], meshIndex[1]})
			{
				return 1;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[2]})
			{
				return 2;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[0]})
			{
				return 3;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[0]})
			{
				return 4;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[4]})
			{
				return 5;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[5]})
			{
				return 6;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[4]})
			{
				return 7;
			}
			else if (index == set<int>{meshIndex[4], meshIndex[5]})
			{
				return 8;
			}
			else if (index == set<int>{meshIndex[5], meshIndex[3]})
			{
				return 9;
			}
		}
		else if (meshData->getMeshType() == MeshPyramid)
		{
			if (index == set<int>{meshIndex[0], meshIndex[1]})
			{
				return 1;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[2]})
			{
				return 2;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[3]})
			{
				return 3;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[0]})
			{
				return 4;
			}
			else if (index == set<int>{meshIndex[0], meshIndex[4]})
			{
				return 5;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[4]})
			{
				return 6;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[4]})
			{
				return 7;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[4]})
			{
				return 8;
			}
		}
		else if (meshData->getMeshType() == MeshHex)
		{
			if (index == set<int>{meshIndex[0], meshIndex[1]})
			{
				return 1;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[2]})
			{
				return 2;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[3]})
			{
				return 3;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[0]})
			{
				return 4;
			}
			else if (index == set<int>{meshIndex[0], meshIndex[4]})
			{
				return 5;
			}
			else if (index == set<int>{meshIndex[1], meshIndex[5]})
			{
				return 6;
			}
			else if (index == set<int>{meshIndex[2], meshIndex[6]})
			{
				return 7;
			}
			else if (index == set<int>{meshIndex[3], meshIndex[7]})
			{
				return 8;
			}
			else if (index == set<int>{meshIndex[4], meshIndex[5]})
			{
				return 9;
			}
			else if (index == set<int>{meshIndex[5], meshIndex[6]})
			{
				return 10;
			}
			else if (index == set<int>{meshIndex[6], meshIndex[7]})
			{
				return 11;
			}
			else if (index == set<int>{meshIndex[7], meshIndex[4]})
			{
				return 12;
			}
		}

		return 0;
	}

	bool mOneFrameData1::MeshFaceIsSurfaceIDPointer(mPostMeshFaceData1* mMeshFaceData1)
	{
		
		if (mMeshFaceData1 == nullptr)
		{
			return false;
		}
		if (mMeshFaceData1->getMeshID2() == 0)
		{
			mPostMeshData1 *mMeshData1 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			if (!mMeshData1->getMeshVisual())
			{
				return false;
			}
			return true;
		}
		else
		{
			mPostMeshData1 *mMeshData11 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			mPostMeshData1 *mMeshData12 = getMeshDataByID(mMeshFaceData1->getMeshID2());
			if (mMeshData11->getPartName() == mMeshData12->getPartName())
			{
				if (mMeshData11->getMeshVisual() && mMeshData12->getMeshVisual())
				{
					return false;
				}
				if ((!mMeshData11->getMeshVisual()) && (!mMeshData12->getMeshVisual()))
				{
					return false;
				}
				return true;
			}
			else if (mMeshData11->getPartName() != mMeshData12->getPartName())
			{
				if ((!mMeshData11->getMeshVisual()) && (!mMeshData12->getMeshVisual()))
				{
					return false;
				}
				return true;
			}

		}
		return false;
	}

	bool mOneFrameData1::MeshFaceIsSurface(mPostMeshFaceData1* mMeshFaceData1, mPostMeshData1 **meshData, int isInTwoMesh, mPostMeshData1 **meshData1)
	{
		
		if (mMeshFaceData1 == nullptr)
		{
			return false;
		}
		if (mMeshFaceData1->getMeshID2() == 0)
		{
			isInTwoMesh = 1;
			*meshData = getMeshDataByID(mMeshFaceData1->getMeshID1());
			return true;
		}
		else
		{
			mPostMeshData1 *mMeshData11 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			mPostMeshData1 *mMeshData12 = getMeshDataByID(mMeshFaceData1->getMeshID2());
			if (mMeshData11->getPartName() != mMeshData12->getPartName())
			{
				isInTwoMesh = 2;
				*meshData = mMeshData11;
				*meshData1 = mMeshData12;
				return true;
			}

		}
		return false;
	}

	bool mOneFrameData1::MeshFaceIsSurface(mPostMeshFaceData1* mMeshFaceData1)
	{
		if (mMeshFaceData1->getMeshID2() == 0)
		{
			mPostMeshData1 *mMeshData1 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			if (!mMeshData1->getMeshVisual())
			{
				return false;
			}
		}
		else
		{
			mPostMeshData1 *mMeshData11 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			mPostMeshData1 *mMeshData12 = getMeshDataByID(mMeshFaceData1->getMeshID2());
			if (mMeshData11->getMeshVisual() && mMeshData12->getMeshVisual())
			{
				return false;
			}
			if ((!mMeshData11->getMeshVisual()) && (!mMeshData12->getMeshVisual()))
			{
				return false;
			}
		}
		return true;
	}

	int mOneFrameData1::MeshFaceIsSurface1(mPostMeshFaceData1* mMeshFaceData1)
	{
		if (mMeshFaceData1->getMeshID2() == 0)
		{
			mPostMeshData1 *mMeshData1 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			if (!mMeshData1->getMeshVisual())
			{
				return 0;
			}
			return mMeshData1->getMeshID();
		}
		else
		{
			mPostMeshData1 *mMeshData11 = getMeshDataByID(mMeshFaceData1->getMeshID1());
			mPostMeshData1 *mMeshData12 = getMeshDataByID(mMeshFaceData1->getMeshID2());
			if (!mMeshData11->getMeshVisual() && mMeshData12->getMeshVisual())
			{
				return mMeshData12->getMeshID();
			}
			if (mMeshData11->getMeshVisual() && !mMeshData12->getMeshVisual())
			{
				return mMeshData11->getMeshID();
			}
		}
		return 0;
	}

	void mOneFrameData1::getModelSize(ModelSize &modelSize)
	{
		if (_isAddOrDeletePart_Mesh)
		{
			caculateMeshModelRadiusAndCenter();
			_isAddOrDeletePart_Mesh = false;
		}
		modelSize = _modelSize;
	}

	bool mOneFrameData1::isExistModel()
	{
		return !_partData1.isEmpty();
	}

	float mOneFrameData1::getModeMeshSize()
	{
		return _meshSize;
	}

	bool mOneFrameData1::isExistMeshPart(QString partName)
	{
		return _partData1.contains(partName);
	}

	std::set<QString> mOneFrameData1::getAllPartNames()
	{
		set<QString> partNames;
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr)
			{
				continue;
			}
			if (!partData->getPartVisual())
			{
				continue;
			}
			partNames.insert(iter.key());
		}
		return partNames;
	}

	QStringList mOneFrameData1::getAllPartNameList()
	{
		QStringList names = _partData1.keys();
		sort(names.begin(), names.end());
		return names;
	}

	void mOneFrameData1::importMeshPartData(QString partName, mPostMeshPartData1* data)
	{
		_partData1.insert(partName, data);
		_oneMeshOperatePartName.insert(partName);
		_isAddOrDeletePart_Mesh = true;
		_meshOperateEnum = ImportPart;
		caculateMeshSize();
	}

	void mOneFrameData1::replaceMeshPartData(QString partName, mPostMeshPartData1* data)
	{
		_partData1.insert(partName, data);
		_oneMeshOperatePartName.insert(partName);
		_isAddOrDeletePart_Mesh = true;
		_meshOperateEnum = AppendOnePart;
		caculateMeshSize();
	}

	void mOneFrameData1::setMeshPartColor(QString partName, QVector3D color)
	{
		mPostMeshPartData1* meshPartData = _partData1.value(partName);
		if (meshPartData != nullptr)
		{
			meshPartData->setPartColor(color);
			_oneMeshOperatePartName.insert(partName);
		}
	}

	QVector3D mOneFrameData1::getMeshPartColor(QString partName)
	{
		mPostMeshPartData1* meshPartData = _partData1.value(partName);
		if (meshPartData != nullptr)
		{
			return	meshPartData->getPartColor();
		}
		return QVector3D();

	}

	void mOneFrameData1::setMeshPartVisual(QString partName, bool isShow)
	{
		mPostMeshPartData1* meshPartData = _partData1.value(partName);
		if (meshPartData != nullptr)
		{
			meshPartData->setPartVisual(isShow);
			_oneMeshOperatePartName.insert(partName);
		}
	}

	bool mOneFrameData1::getMeshPartVisual(QString partName)
	{
		mPostMeshPartData1* meshPartData = _partData1.value(partName);
		if (meshPartData != nullptr)
		{
			return meshPartData->getPartVisual();
		}
		return false;
	}

	void mOneFrameData1::setAllMeshPartVisual(bool isShow)
	{
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			iter.value()->setPartVisual(isShow);
		}
	}

	void mOneFrameData1::setAllMeshPartColor(QVector3D color)
	{
		QHashIterator<QString, mPostMeshPartData1*> iter(_partData1);
		while (iter.hasNext())
		{
			iter.next();
			iter.value()->setPartColor(color);
		}
	}

	int mOneFrameData1::getOneFrameNodeAmount()
	{
		return _nodeData1.size() + _nodeData2.size();
	}

	void mOneFrameData1::appendPartData(QString partName, mPostMeshPartData1* onePartData)
	{
		_partData1[partName] = onePartData;
	}

	QHash<QString, mPostMeshPartData1*> mOneFrameData1::getAllPartData()
	{
		return _partData1;
	}

	QStringList mOneFrameData1::getOneFramePartNames()
	{
		return _partData1.keys();
	}

	mPostMeshPartData1* mOneFrameData1::getOnePartDataByName(QString partName)
	{
		if (_partData1.contains(partName))
		{
			return _partData1.value(partName);
		}
		return nullptr;
	}

	void mOneFrameData1::deleteOneFrameData()
	{
		_allMeshOperatePart = true;
		_meshOperateEnum = DeleteAllPart;

		QList<int> allNodeIDs = _nodeData1.keys();
		for_each(allNodeIDs.begin(), allNodeIDs.end(), [this](int id) {mPostMeshNodeData1* nodeData = _nodeData1.value(id); delete nodeData; nodeData = nullptr; });
		QList<int> allMeshIDs = _meshData1.keys();
		for_each(allMeshIDs.begin(), allMeshIDs.end(), [this](int id) {mPostMeshData1* meshData = _meshData1.value(id); delete meshData; meshData = nullptr; });
		QList<int> allMeshLineIDs = _meshLineData1.keys();
		for_each(allMeshLineIDs.begin(), allMeshLineIDs.end(), [this](int id) {mPostMeshLineData1* meshLineData = _meshLineData1.value(id); delete meshLineData; meshLineData = nullptr; });
		//QList<int> allMeshFaceIDs = _meshFaceData1.keys();
		//for_each(allMeshFaceIDs.begin(), allMeshFaceIDs.end(), [this](int id) {mPostMeshFaceData1* meshFaceData = _meshFaceData1.value(id); delete meshFaceData; meshFaceData = nullptr; });

		for (int i = 0; i < _meshFaceData1.size(); i++)
		{	
			mPostMeshFaceData1* s = _meshFaceData1[i];
			delete s;
		}

		for (int i = 0; i < _nodeData2.size(); i++)
		{
			mPostMeshNodeData1* s = _nodeData2[i];
			delete s;
		}

		for (int i = 0; i < _meshData2.size(); i++)
		{
			mPostMeshData1* s = _meshData2[i];
			delete s;
		}

		std::vector<mPostMeshFaceData1*>().swap(_meshFaceData1);

		QList<QString> allPartNames = _partData1.keys();
		for_each(allPartNames.begin(), allPartNames.end(), [this](QString name) {mPostMeshPartData1* partData = _partData1.value(name); delete partData; partData = nullptr; });

		_modelSize = ModelSize();

		_nodeData1.clear();
		_meshData1.clear();
		_nodeData2.clear();
		_meshData2.clear();

		_meshLineData1.clear();
		_meshFaceData1.clear();
		_partData1.clear();
		
	}

	int mOneFrameData1::getOneFrameElementAmount()
	{
		return _meshData1.size();
	}

	void mOneFrameData1::setOneFrameModalValue(double freq, double eig)
	{
		_hasModalValue = true;
		_modalValue[0] = freq;
		_modalValue[1] = eig;
	}

	bool mOneFrameData1::isOneFrameModalValue()
	{
		return _hasModalValue;
	}

	double mOneFrameData1::getOneFrameModalFrequency()
	{
		return _modalValue[0];
	}

	double mOneFrameData1::getOneFrameModalEigen()
	{
		return _modalValue[1];
	}

	QHashIterator<int, mPostMeshNodeData1*> mOneFrameData1::getMeshNodeIterator()
	{
		QHashIterator<int, mPostMeshNodeData1*> iter(_nodeData1);
		return iter;
	}
}
