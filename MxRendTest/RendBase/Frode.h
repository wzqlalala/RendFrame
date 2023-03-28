#pragma once
#include "Group.h"

namespace mxr
{
	class MXR_EXPORT Frode : public Group
	{
	public:
		Frode();
		virtual ~Frode();
		virtual Frode* asFrode() { return this; }
		virtual const Frode* asFrode() const { return this; }
		virtual void setNodeMask(NodeMask nm)
		{
			_nodeMask = nm;
		}
		virtual void accept(asset_ref<NodeVisitor> nv);
		
	};

}
