#include "Node.h"
#include "StateSet.h"
#include "Group.h"

namespace mxr
{
	Node::Node() { _nodeMask = 0; }
	Node::~Node() 
	{
		
	}


	void Node::setStateSet(asset_ref<StateSet> stateset)
	{
		_stateset = stateset;
	}

	asset_ref<StateSet> Node::getOrCreateStateSet()
	{
		_stateset = MakeAsset<StateSet>();
		return _stateset;
	}


	void Node::addParent(Group * parent)
	{
		_parents.push_back(parent);
	}

	void Node::removeParent(Group * parent)
	{
		_parents.erase(std::find(_parents.begin(), _parents.end(), parent));
	}

}
