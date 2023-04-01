#include "mPostSetBase.h"

namespace MDataPost
{
	mPostSetBase::mPostSetBase()
	{

	}

	mPostSetBase::mPostSetBase(const QString& name, PostSetType type) :_setName(name), _setType(type)
	{

	}

	mPostSetBase::~mPostSetBase()
	{

	}

	void mPostSetBase::setSetName(const QString& name)
	{
		_setName = name;
	}

	QString mPostSetBase::getSetName()
	{
		return _setName;
	}
	//////////
	void mPostSetBase::setSetType(PostSetType type)
	{
		_setType = type;
	}

	PostSetType mPostSetBase::getSetType()
	{
		return _setType;
	}

	QString mPostSetBase::getSetTypeStr()
	{
		switch (_setType)
		{
		case PostSetType::Element:
			return QString("单元集");
		case PostSetType::Node:
			return QString("节点集");
		case PostSetType::Part:
			return QString("零件集");
		case PostSetType::ElementSurface:
			return QString("单元表面集");
		case PostSetType::CuttingPlane:
			return QString("切面集");
		default:
			return QString();
		}
	}
	mPostNodeSet::mPostNodeSet()
	{
		this->setSetType(PostSetType::Node);
	}
	mPostNodeSet::mPostNodeSet(const QString & name) :mPostSetBase(name, PostSetType::Node)
	{
	}
	mPostNodeSet::~mPostNodeSet()
	{
	}

	mPostElementSet::mPostElementSet()
	{
		this->setSetType(PostSetType::Element);
	}
	mPostElementSet::mPostElementSet(const QString & name) :mPostSetBase(name, PostSetType::Element)
	{
	}
	mPostElementSet::~mPostElementSet()
	{
	}

	mPostElementSurfaceSet::mPostElementSurfaceSet()
	{
		this->setSetType(PostSetType::ElementSurface);
	}

	mPostElementSurfaceSet::mPostElementSurfaceSet(const QString & name) :mPostSetBase(name, PostSetType::ElementSurface)
	{
	}

	mPostElementSurfaceSet::~mPostElementSurfaceSet()
	{
	}


	mPostPartSet::mPostPartSet()
	{
		this->setSetType(PostSetType::Part);
	}

	mPostPartSet::mPostPartSet(const QString & name) :mPostSetBase(name, PostSetType::Part)
	{
	}

	mPostPartSet::~mPostPartSet()
	{
	}

	mPostCuttingPlaneSet::mPostCuttingPlaneSet()
	{
		this->setSetType(PostSetType::CuttingPlane);
	}

	mPostCuttingPlaneSet::mPostCuttingPlaneSet(const QString & name) :mPostSetBase(name, PostSetType::CuttingPlane)
	{
	}

	mPostCuttingPlaneSet::~mPostCuttingPlaneSet()
	{
	}

}
