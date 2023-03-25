#include "Group.h"
#include "NodeVisitor.h"

namespace mxr
{
	Group::Group()
	{

	}

	void Group::accept(asset_ref<NodeVisitor> nv)
	{
		for (int i = 0; i < _children.size(); i++)
		{
			_children[i]->accept(nv);
		}

	}

	bool Group::addChild(asset_ref<Node> child)
	{
		if (containsNode(child.get()))
		{
			return false;
		}
		child->addParent(this);
		_children.push_back(child);
		return true;
	}

	bool Group::insertChild(unsigned int index, asset_ref<Node> child)
	{
		child->addParent(this);
		_children.insert(_children.begin() + index, child);
		return true;
	}

	bool Group::removeChild(asset_ref<Node> child)
	{
		NodeList::const_iterator result = std::find(_children.begin(), _children.end(), child);
		if (result == _children.end()) 
		{
			return false;
		}
		child->removeParent(this);
		_children.erase(result);
		return true;
	}

	void Group::removeAllChild()
	{
		for (auto child : _children)
		{
			child->removeParent(this);
		}
	}


	unsigned int Group::getNumChildren() const
	{
		return _children.size();
	}


	Group::~Group()
	{

	}

}