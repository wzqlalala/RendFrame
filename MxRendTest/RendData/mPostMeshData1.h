#pragma once
#include "renddata_global.h"

#include <QVector>
#include <QObject>
#include <set>

#include "mBasicEnum.h"

namespace MDataPost
{
	class mPostMeshFaceData1;
	class mPostMeshNodeData1;
	class RENDDATA_EXPORT mPostMeshData1
	{
	public:
		

		mPostMeshData1(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType eleType, QString partName);

		~mPostMeshData1();

		//设置网格的节点
		void setNodeIndex(QVector<int> index);

		//获取网格的节点
		QVector<int> getNodeIndex();
	

		//获取网格的网格类型
		MBasicFunction::MeshType getMeshType();

		//设置网格的单元类型
		void setElementType(MBasicFunction::ElementType eleType);

		//获取网格的单元类型
		MBasicFunction::ElementType getElementType();

		//添加网格对应的网格面
		void appendMeshFace(mPostMeshFaceData1* meshFace);

		//设置网格对应的网格面
		void setMeshFace(uint8_t location, mPostMeshFaceData1* meshFace);

		//设置网格对应的网格面
		//void setMeshFace(const QVector<int>& ids);

		//获取网格对应的网格面
		QVector<int> getMeshFaces();
		QVector<mPostMeshFaceData1*> getFace();
		//添加网格对应的网格线
		void appendMeshLine(int meshLineID);

		//获取网格对应的网格面
		QVector<int> getMeshLines();

		//设置网格对应的网格线
		void setMeshLine(const QVector<int>& ids);

		//设置网格显隐
		void setMeshVisual(bool isshow);

		//反向网格显隐
		void reverseMeshVisual();

		//获取网格是否显隐
		bool getMeshVisual();

		//获取网格的维度
		int getMeshDimension();

		//获取网格的ID
		int getMeshID();

		//获取所在的部件名称
		QString getPartName();

	private:
		QVector<int> _index;
		mPostMeshFaceData1* faces[6] = { nullptr };	
		//网格下的所有单元线ID
		//QVector<int> _meshLineIDs;
		//网格所在的部件名称
		QString _partName;
		//网格类型
		MBasicFunction::MeshType _meshType;
		//单元类型
		MBasicFunction::ElementType _eleType;
		//网格ID
		int _meshID;
		//网格是否显示
		bool _isShow;
		//面的大小
		uint8_t _location = 0;


	};
}
