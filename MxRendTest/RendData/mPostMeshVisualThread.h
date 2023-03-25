#pragma once
#include <QObject>
#include <set>

namespace MDataPost
{
	class mPostMeshData1;
	class mPostMeshPartData1;
	class mOneFrameData1;
	class mPostMeshVisualThread:public QObject
	{
		Q_OBJECT
	public:
		mPostMeshVisualThread();

		~mPostMeshVisualThread();

		void setParameters(mOneFrameData1 *oneFrameData,QVector<mPostMeshData1*> meshDatas, mPostMeshPartData1 *partData);

		/*
		 *	是否完成拾取
		 */
		bool isFinished();

	signals:

		void finishedWorkSig();

	public slots:

		void slot_startWork();

	private:
		bool _isfinished;
		mOneFrameData1 *_oneFrameData;
		QVector<mPostMeshData1*> _meshDatas;
		mPostMeshPartData1 *_partData;
	};
}


