#include "mPostMeshVisualControl.h"
#include "mPostMeshVisualThread.h"
#include "mOneFrameData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshPartData1.h"

namespace MDataPost
{
	mPostMeshVisualControl::mPostMeshVisualControl()
	{
	
	}

	mPostMeshVisualControl::~mPostMeshVisualControl()
	{

	}

	void mPostMeshVisualControl::maskMesh(mOneFrameData1 *oneFrameData,std::set<int> meshIDs)
	{
		oneFrameData->maskMesh(meshIDs);

		doSomeWork(oneFrameData);
	}

	void mPostMeshVisualControl::showMesh(mOneFrameData1 * oneFrameData, std::set<int> meshIDs)
	{
		for (auto iter = meshIDs.begin(); iter != meshIDs.end(); ++iter)
		{
			mPostMeshData1* meshData = oneFrameData->getMeshDataByID(*iter);
			if (meshData == nullptr)
			{
				continue;
			}
			meshData->setMeshVisual(true);
		}

		doSomeWork(oneFrameData);
	}

	void mPostMeshVisualControl::reverseMaskMesh(mOneFrameData1 * oneFrameData)
	{
		oneFrameData->reverseMaskMesh();

		doSomeWork(oneFrameData);
	}

	void mPostMeshVisualControl::showAllMesh(mOneFrameData1 * oneFrameData)
	{
		oneFrameData->showAllMesh();
		doSomeWork(oneFrameData);
	}

	void mPostMeshVisualControl::slot_finishedWork()
	{
		for (QPair<mPostMeshVisualThread*, QThread*> threads : _threads)
		{
			mPostMeshVisualThread *thread = threads.first;
			if (!thread->isFinished())
			{
				return;
			}
		}
		for (QPair<mPostMeshVisualThread*, QThread*> threads : _threads)
		{
			mPostMeshVisualThread *thread = threads.first;
			QThread *thread2 = threads.second;
			thread2->exit();
			thread2->wait(1);//∑¿÷πœﬂ≥ÃŒ¥ Õ∑≈
			//delete partThread;
			//partThread = nullptr;
			delete thread;
			delete thread2;
		}
		QVector<QPair<mPostMeshVisualThread*, QThread*>>().swap(_threads);

		emit finishedWorkSig();
	}

	void mPostMeshVisualControl::doSomeWork(mOneFrameData1 * oneFrameData)
	{
		auto iter = oneFrameData->getMeshPartIterator();
		while (iter.hasNext())
		{
			iter.next();
			mPostMeshPartData1 *partData = iter.value();
			if (partData == nullptr || !partData->getPartVisual())
			{
				continue;
			}
			partData->clearMeshFaceData();
			QVector<mPostMeshData1*> meshDatas = partData->getMeshDatas3();
			for (int i = 0; i < meshDatas.size(); i += 500000)
			{
				mPostMeshVisualThread *meshVisualThread = new mPostMeshVisualThread();
				QThread *thread = new QThread(this);
				meshVisualThread->moveToThread(thread);
				thread->start();
				_threads.append({ meshVisualThread, thread });
				meshVisualThread->setParameters(oneFrameData, meshDatas.mid(i, min(500000, meshDatas.size() - i)), partData);

				connect(this, &mPostMeshVisualControl::startWorkSig, meshVisualThread, &mPostMeshVisualThread::slot_startWork, Qt::QueuedConnection);
				connect(meshVisualThread, &mPostMeshVisualThread::finishedWorkSig, this, &mPostMeshVisualControl::slot_finishedWork);
			}
		}

		emit startWorkSig();

		if (_threads.size()==0)
		{
			emit finishedWorkSig();
		}
	}
}