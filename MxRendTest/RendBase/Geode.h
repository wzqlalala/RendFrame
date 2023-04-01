#pragma once
#include "Group.h"

namespace mxr
{
	class MXR_EXPORT Geode : public Group
	{
	public:
		Geode();
		virtual ~Geode();
		virtual Geode* asGeode() { return this; }
		virtual const Geode* asGeode() const { return this; }
		virtual void setNodeMask(NodeMask nm)
		{
			if (this->getNumChildren() == 0)
			{
				return;
			}
			for (auto Node : _children)
			{
				Node->setNodeMask(nm);
			}
			_nodeMask = nm;
		}		
	};

}
