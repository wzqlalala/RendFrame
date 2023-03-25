#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "rendbasic_global.h"

//QT
#include <QObject>


namespace MAppConfiguration
{
	class RENDBASIC_EXPORT mFileConfiguration :public QObject
	{
	public:
		/*
		* 默认构造函数
		*/
		mFileConfiguration();

		/*
		* 析构函数
		*/
		~mFileConfiguration();

		/**********************************RecentOpenMxCaeFileDir**************************************/

		/*
		 * 软件初始化的时候从文件读取最近打开的工程文件的路径
		 */
		void setRecentOpenMxCaeFileDir(const QList<QString> &dir);

		/*
		 * 添加打开的工程文件的路径
		 */
		void appendMxCaeFileDir(const QString &fileDir);

		/*
		 * 获取最近打开的工程文件的路径(上次打开的)
		 */
		QString getMxCaeFileDir();

		/*
		* 获取最近打开的工程文件的路径(最近几次的)
		*/
		QList<QString> getMxCaeFileDirData();

		/**********************************RecentInDir**************************************/

		/*
		 * 软件初始化的时候从文件读取最近打开的路径
		 */
		void setRecentInDir(const QString &dir);

		/*
		 * 获取最近打开的的路径
		 */
		QString getRecentInDir();


		/**********************************RecentOutDir**************************************/

		/*
		 * 软件初始化的时候从文件读取最近导出的路径
		 */
		void setRecentOutDir(const QString &dir);

		/*
		 * 获取最近导出的的路径
		 */
		QString getRecentOutDir();

		/**********************************WorkingDir**************************************/

		/*
		 * 设置工作目录
		 */
		void setWorkingDir(const QString &dir);

		/*
		 * 获取工作目录
		 */
		QString getWorkingDir();

	private:

		//最近打开的工程文件的路径的数据
		QList<QString> _recentOpenMxCaeFileDir;

		//最近导入或者打开（读文件）的路径
		QString _recentInDir;

		//最近导出或者写出（写文件）的路径（即工作目录）
		QString _recentOutDir;

		//工作目录
		QString _workingDir;

	public:

		//前后处理的网格节点关系文件名
		QByteArray _prePointCellFileName;//除去dat后缀
		QByteArray _prePointLineFileName;//除去dat后缀
		QByteArray _postPointCellFileName;//除去dat后缀
		QByteArray _postPointLineFileName;//除去dat后缀
	};
}