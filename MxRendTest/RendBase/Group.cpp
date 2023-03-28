#include "Group.h"
#include "NodeVisitor.h"

namespace mxr
{
	Group::Group()
	{

	}

	void Group::accept(asset_ref<NodeVisitor> nv)
	{
		if (isNeedAccept())
		{
			for (int i = 0; i < _children.size(); i++)
			{
				_children[i]->accept(nv);
			}
			_needAccept = false;
		}

	}

	bool Group::addChild(asset_ref<Node> child)
	{
		if (containsNode(child.get()))
		{
			return false;
		}
		_needAccept = true;
		child->addParent(this);
		_children.push_back(child);
		return true;
	}

	bool Group::insertChild(unsigned int index, asset_ref<Node> child)
	{
		child->addParent(this);
		_needAccept = true;
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
		_needAccept = true;
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
		_needAccept = true;
	}


	unsigned int Group::getNumChildren() const
	{
		return _children.size();
	}


	Group::~Group()
	{

	}

}