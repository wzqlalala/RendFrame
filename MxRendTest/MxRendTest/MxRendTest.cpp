#include "MxRendTest.h"
#include "mPreRend.h"
#include "mPostRend.h"
#include "mTestRender.h"
#include "mPostRender.h"
#include "mModelTestRender.h"
#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mModelView.h"

#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostMeshPartData1.h"
#include "mPostMeshData1.h"
#include "mPostMeshFaceData1.h"
#include "mPostMeshLineData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostOneFrameRendData.h"


#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QCollator>

#include "mIMxdbFile1.h"

using namespace MDataPost;
MxRendTest::MxRendTest(int id)
{
    ui.setupUi(this);
	_preRend = nullptr;
	_postRend = nullptr;
	_testRender = nullptr;
	if (id == 0)
	{
		_preRend = new MPreRend::mPreRend(QString::number(id)); ui.gridLayout->addWidget(_preRend);
	}
	else
	{
		_postRend = new MPostRend::mPostRend(QString::number(id)); ui.gridLayout->addWidget(_postRend);
	}

	
}

void MxRendTest::keyPressEvent(QKeyEvent * event)
{

	if (event->key() == Qt::Key_Escape)
	{
		if (_preRend != nullptr)
		{
			_preRend->clearRender();
		}
		if (_postRender != nullptr)
		{
			_postRender->clearRender();
		}
	}
	else if (event->key() == Qt::Key_F1)
	{
		if (_preRend == nullptr)
		{
			return;
		}
		QString filename = QFileDialog::getOpenFileName(this, "ѡ��obj�ļ�", qApp->applicationDirPath(), "*.obj");
		QFileInfo info(filename);

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string err;
		QString baseDir = info.absolutePath();
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.toLocal8Bit(), baseDir.toLocal8Bit(), true);
		if (!warn.empty()) {
			qDebug() << "WARN: " << QString::fromStdString(warn) << endl;
		}

		if (!err.empty()) {
			qDebug() << "ERR: " << QString::fromStdString(err) << endl;
		}

		if (!ret) {
			printf("Failed to load/parse .obj.\n");
			return ;
		}
		if (!_modelRender)
		{
			//_preRend->removeRender(_modelRender);
		}

		_modelRender = make_shared<MBaseRend::mModelTestRender>(_preRend->getApplication(), _preRend->getRoot());
		auto model = _modelRender->setData(attrib, shapes, materials);
		_preRend->addRender(_modelRender);
		_preRend->getCamera()->ResetOrthoAndCamera(model.first, model.second);
	}

	else if (event->key() == Qt::Key_F2)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender = make_shared<MPostRend::mPostRender>(_postRend->getApplication(), _postRend->getRoot());
		
		shared_ptr<mDataPost1> dp = make_shared<mDataPost1>();
		mPostOneFrameRendData *oneFrameRendData = new mPostOneFrameRendData();
		if (!getData(dp, oneFrameRendData))
		{
			delete oneFrameRendData;
			return;
		}
		_postRender->setPostData(dp);
		_postRender->setRendCurrentFrameData(oneFrameRendData);	
		_postRend->addRender(_postRender);
		Space::AABB aabb = _postRender->getOneFrameRender()->getModelRender()->getModelAABB();
		QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
		float radius = (aabb.maxEdge - aabb.minEdge).length()/2.0;
		_postRend->getCamera()->ResetOrthoAndCamera(center, radius);
		_postRender->setShowFuntion(ElementFace);
	}
	else if (event->key() == Qt::Key_1)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->setIsShowInitialShape(_isShowInitialShape);
		_isShowInitialShape = !_isShowInitialShape;
		//_postRender->getOneFrameRender()->updateAllModelOperate(ShowNodeformation);
		//_postRender->getOneFrameRender()->updateOneModelOperate({ HideOnePartOperate, set<QString>{"Bolt"} });
	}
	else if (event->key() == Qt::Key_2)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->getOneFrameRender()->updateAllModelOperate(_modelOperate);
		int id = int(_modelOperate);
		if (_modelOperate == TextureAllPartOperate)
		{
			_modelOperate = ColorAllPartOperate;
		}
		else
		{
			id++;
			_modelOperate = PostModelOperateEnum(id);
		}
	}
	else if (event->key() == Qt::Key_3)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->getOneFrameRender()->updateAllModelOperate(_modelOperate1);
		if (_modelOperate1 == HideAllPartOperate)
		{
			_modelOperate1 = ShowAllPartOperate;
		}
		else
		{
			_modelOperate1 = HideAllPartOperate;
		}
	}
	else if (event->key() == Qt::Key_4)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->setDeformationScale(QVector3D(2.0,2.0,2.0));
	}
	else if (event->key() == Qt::Key_5)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->setDispersed(_dispersed);
		_dispersed = !_dispersed;
	}
	else if (event->key() == Qt::Key_6)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->setDispersIsEquivariance(_isEquivariance);
		_isEquivariance = !_isEquivariance;
	}
	else if (event->key() == Qt::Key_Q)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		Space::AABB aabb = _postRender->getOneFrameRender()->getModelRender()->getModelAABB();
		float radius = (aabb.maxEdge - aabb.minEdge).length() / 2.0;
		QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
		_postRender->createCuttingPlane(0, QVector3D(0,0,1), center);
		_postRender->setPlaneData(0, QVector3D(0, 0, 1), center, radius);
	}
	else if (event->key() == Qt::Key_W)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		Space::AABB aabb = _postRender->getOneFrameRender()->getModelRender()->getModelAABB();
		float radius = (aabb.maxEdge - aabb.minEdge).length() / 2.0;
		QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
		_postRender->createCuttingPlane(1, QVector3D(0, 1, 0), center);
		_postRender->setPlaneData(1, QVector3D(0, 1, 0), center, radius);
	}
	else if (event->key() == Qt::Key_E)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		_postRender->deleteCuttingPlane(0);		
	}
	else if (event->key() == Qt::Key_Asterisk)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		int show = int(_showfuntion);
		if (_showfuntion == SmoothShaded)
		{
			show = 0;
		}
		else
		{
			show++;
		}
		_showfuntion = ShowFuntion(show);
		_postRender->setShowFuntion(_showfuntion);
	}
	else if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus)
	{
		if (_postRend == nullptr)
		{
			return;
		}
		if (event->key() == Qt::Key_Plus)
		{
			_lineWidth += 0.1f;
			if (_lineWidth > 20.0f)
			{
				_lineWidth = 1.0f;
			}
		}
		else
		{
			_lineWidth -= 0.1f;
			if (_lineWidth < 0.0f)
			{
				_lineWidth = 1.0f;
			}
		}

		_postRender->setEdgeLineWidth(_lineWidth);
	}
	else if (event->key() == Qt::Key_0)
	{
		if (_preRend == nullptr)
		{
			return;
		}
		if (!_testRender)
		{
			_testRender = make_shared<MBaseRend::mTestRender>(_preRend->getApplication(), _preRend->getRoot());
			_preRend->addRender(_testRender);
		}
		_testRender->appendOnePart();
		_preRend->getCamera()->ResetOrthoAndCamera(QVector3D(0, 0, 0), 5.0);
	}

	if (_preRend)
	{
		_preRend->update();
	}
	if (_postRend)
	{
		_postRend->update();
	}

}

bool MxRendTest::getData(shared_ptr<mDataPost1> dp, mPostOneFrameRendData *oneFrameRendData)
{
	QStringList names = QFileDialog::getOpenFileNames(this, "ѡ��obj�ļ�", qApp->applicationDirPath(), "*.mxdb;*.mxdb0");
	
	if (names.empty())
	{
		return false;
	}

	//��������ɸѡ����
	QMultiHash<QString, QString> postframefiles;
	QHash<QString, QString> postinitfile;

	//��ȡ��ǰ�ļ��������е�mxdb0�ļ�
	QFileInfo initinfo(names.at(0));
	QDir filedir = initinfo.absoluteDir();
	QStringList db0filter = QStringList{ QString("*.mxdb0") };
	QFileInfoList fileInfos = filedir.entryInfoList(db0filter, QDir::Files, QDir::Time | QDir::Reversed);
	for (QFileInfo finfo : fileInfos)
	{
		postinitfile.insert(finfo.completeBaseName(), finfo.absoluteFilePath());
	}

	//��ȡ����ѡ�е�mxdb�ļ�
	QStringList initnames = postinitfile.keys();
	for (QString filename : names)
	{
		QFileInfo finfo(filename);
		QString fsuffix = finfo.suffix();
		QString fname = finfo.completeBaseName();
		if (fsuffix == "mxdb")
		{
			for (QString initname : initnames)
			{
				int nbyte = initname.size();
				if (fname.left(nbyte) == initname && fname.at(nbyte) == "_")
				{
					postframefiles.insert(initname, filename);
					break;
				}
			}
		}
	}

	QStringList postnames = postframefiles.uniqueKeys();
	for (QString pname : postnames)
	{
		QStringList framefiles = postframefiles.values(pname);
		if (framefiles.size() < 1)
		{

			continue;
		}
		//��û��ѡ���ʼ�����ļ������Զ���ȡ
		QString initfile = postinitfile.value(pname);
		if (initfile.isEmpty())
		{

			continue;
		}
		//��֡�ļ���������
		QCollator collator;
		collator.setNumericMode(true);
		std::sort(framefiles.begin(), framefiles.end(), [&collator](const QString & str1, const QString & str2)
		{
			return collator.compare(str1, str2) < 0;
		});

		//��ȡ�ļ�
		MIOFile::mIMxdbFile1* mxdbThread1 = new MIOFile::mIMxdbFile1(initfile, framefiles);
		mxdbThread1->setDataPost(dp);
		mxdbThread1->setOneFrameRenderData(oneFrameRendData);
		mxdbThread1->slot_importAllMxDbFile();
		mxdbThread1->readCloudData();
		mxdbThread1->readDisplacementData();
	}

	return true;
}