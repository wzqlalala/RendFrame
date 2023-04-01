#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QObject>
#include <QVector>
#include <QPair>
#include <QThread>
#include <set>



namespace MDataPost
{
	class mOneFrameData1;
	class mPostMeshVisualThread;
	class RENDDATA_EXPORT mPostMeshVisualControl :public QObject
	{
		Q_OBJECT

	public:
		mPostMeshVisualControl();

		~mPostMeshVisualControl();

		//隐藏网格
		void maskMesh(mOneFrameData1 *oneFrameData,std::set<int> meshIDs);

		//显示网格
		void showMesh(mOneFrameData1 *oneFrameData,std::set<int> meshIDs);

		//反向隐藏网格
		void reverseMaskMesh(mOneFrameData1 *oneFrameData);

		//显示所有网格
		void showAllMesh(mOneFrameData1 *oneFrameData);

	private:

		void doSomeWork(mOneFrameData1 * oneFrameData);

	private slots:

		void slot_finishedWork();

	signals:

		void startWorkSig();

		void finishedWorkSig();

	private:

		QVector<QPair<mPostMeshVisualThread*, QThread*>> _threads;
	};
}

