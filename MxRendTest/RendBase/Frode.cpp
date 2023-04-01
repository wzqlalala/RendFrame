#include "Frode.h"
#include "NodeVisitor.h"
mxr::Frode::Frode()
{
}

mxr::Frode::~Frode()
{

}

void mxr::Frode::accept(asset_ref<NodeVisitor> nv)
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
