#include "mPostMeshVisualThread.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mOneFrameData1.h"

#include <QMutex>

namespace MDataPost
{
	QMutex mutex;
	mPostMeshVisualThread::mPostMeshVisualThread()
	{
		_isfinished = false;
	}
	mPostMeshVisualThread::~mPostMeshVisualThread()
	{
	}
	void mPostMeshVisualThread::setParameters(mOneFrameData1 *oneFrameData, QVector<mPostMeshData1*> meshDatas, mPostMeshPartData1 * partData)
	{
		_oneFrameData = oneFrameData;
		_meshDatas = meshDatas;
		_partData = partData;
	}

	void mPostMeshVisualThread::slot_startWork()
	{
		QVector<mPostMeshFaceData1*> faces;
		for (mPostMeshData1* meshData : _meshDatas)
		{
			QVector<mPostMeshFaceData1 *> meshFaceIDs = meshData->getFace();
			for (mPostMeshFaceData1 *meshFaceData : meshFaceIDs)
			{
				if (meshFaceData == nullptr)
				{
					continue;
				}
				if (_oneFrameData->getMeshFaceShow(meshFaceData, meshData))
				{
					faces.append(meshFaceData);
				}
			}
		}
		mutex.lock();
		_partData->appendMeshFaceData(faces);
		mutex.unlock();
		_isfinished = true;

		emit finishedWorkSig();

		



	}

	bool mPostMeshVisualThread::isFinished()
	{
		return _isfinished;
	}
}