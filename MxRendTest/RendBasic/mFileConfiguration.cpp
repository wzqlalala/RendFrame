#include "mFileConfiguration.h"

//QT
#include <QCoreApplication>
#include <QDir>

namespace MAppConfiguration
{
	mFileConfiguration::mFileConfiguration()
	{
		_recentInDir = QDir(qApp->applicationDirPath() + "/../WorkingDir/").absolutePath();
		_recentOutDir = QDir(qApp->applicationDirPath() + "/../WorkingDir/").absolutePath();
		_workingDir = QDir(qApp->applicationDirPath() + "/../WorkingDir/").absolutePath();
	}

	mFileConfiguration::~mFileConfiguration()
	{
	}

	void mFileConfiguration::setRecentOpenMxCaeFileDir(const QList<QString> &recentOpenMxCaeFileDir)
	{
		_recentOpenMxCaeFileDir = recentOpenMxCaeFileDir;
	}

	void mFileConfiguration::appendMxCaeFileDir(const QString &fileDir)
	{
		if (_recentOpenMxCaeFileDir.removeOne(fileDir))
		{
			_recentOpenMxCaeFileDir.append(fileDir);
			return;
		}
		if (_recentOpenMxCaeFileDir.size() == 6)
		{
			_recentOpenMxCaeFileDir.removeFirst();
		}
		_recentOpenMxCaeFileDir.append(fileDir);
	}

	QString mFileConfiguration::getMxCaeFileDir()
	{
		QString fileDir{ "" };
		if (_recentOpenMxCaeFileDir.isEmpty())
		{
			return fileDir;
		}
		fileDir = _recentOpenMxCaeFileDir.last();
		return fileDir;
	}

	QList<QString> mFileConfiguration::getMxCaeFileDirData()
	{
		return _recentOpenMxCaeFileDir;
	}

	void mFileConfiguration::setRecentInDir(const QString& dir)
	{
		_recentInDir = dir;
	}

	QString mFileConfiguration::getRecentInDir()
	{
		return _recentInDir;
	}

	void mFileConfiguration::setRecentOutDir(const QString& dir)
	{
		_recentOutDir = dir;
	}

	QString mFileConfiguration::getRecentOutDir()
	{
		return _recentOutDir;
	}

	void mFileConfiguration::setWorkingDir(const QString &dir)
	{
		_workingDir = dir;
	}

	QString mFileConfiguration::getWorkingDir()
	{
		return _workingDir;
	}
}
