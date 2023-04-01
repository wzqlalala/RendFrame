#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include "mPostEnum.h"
//QT
#include <QObject>
#include <set>

namespace MDataPost
{
	class RENDDATA_EXPORT mPostSetBase
	{
	
	public:
		/*
		* 默认构造函数
		*/
		mPostSetBase();

		/*
		* 重载构造函数
		*/
		mPostSetBase(const QString& name, PostSetType type);

		/*
		* 析构函数
		*/
		~mPostSetBase();

		/*
		* 设置名
		*/
		void setSetName(const QString& name);

		/*
		* 获取名
		*/
		QString getSetName();

		/*
		* 设置类型
		*/
		void setSetType(PostSetType type);

		/*
		* 获取类型
		*/
		PostSetType getSetType();

		/*
		* 获取集合类型字符串
		*/
		QString getSetTypeStr();
	
	private:

		//名
		QString _setName;

		//类型
		PostSetType _setType;
	};

	class RENDDATA_EXPORT mPostNodeSet : public mPostSetBase
	{
	public:
		/*
		* 默认构造函数
		*/
		mPostNodeSet();

		/*
		* 重载构造函数
		*/
		mPostNodeSet(const QString& name);

		/*
		* 析构函数
		*/
		~mPostNodeSet();

	public:
		//节点ID列表（无顺序）
		std::set<int> _ids;

	};

	class RENDDATA_EXPORT mPostElementSet : public mPostSetBase
	{
	public:
		/*
		* 默认构造函数
		*/
		mPostElementSet();

		/*
		* 重载构造函数
		*/
		mPostElementSet(const QString& name);

		/*
		* 析构函数
		*/
		~mPostElementSet();

	public:
		//单元ID列表（无顺序）
		std::set<int> _ids;

	};

	class RENDDATA_EXPORT mPostElementSurfaceSet: public mPostSetBase
	{
	public:
		/*
		* 默认构造函数
		*/
		mPostElementSurfaceSet();

		/*
		* 重载构造函数
		*/
		mPostElementSurfaceSet(const QString& name);

		/*
		* 析构函数
		*/
		~mPostElementSurfaceSet();

	public:
		//单元面ID列表（无顺序）
		std::set<int> _ids;

	};

	class RENDDATA_EXPORT mPostPartSet : public mPostSetBase
	{
	public:
		/*
		* 默认构造函数
		*/
		mPostPartSet();

		/*
		* 重载构造函数
		*/
		mPostPartSet(const QString& name);

		/*
		* 析构函数
		*/
		~mPostPartSet();

	public:
		//部件列表（无顺序）
		std::set<QString> _names;

	};

	class RENDDATA_EXPORT mPostCuttingPlaneSet : public mPostSetBase
	{
	public:
		/*
		* 默认构造函数
		*/
		mPostCuttingPlaneSet();

		/*
		* 重载构造函数
		*/
		mPostCuttingPlaneSet(const QString& name);

		/*
		* 析构函数
		*/
		~mPostCuttingPlaneSet();

	public:
		//切面列表（无顺序）
		std::set<QString> _names;

	};
}
