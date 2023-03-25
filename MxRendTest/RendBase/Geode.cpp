#include "Geode.h"
#include "NodeVisitor.h"
mxr::Geode::Geode()
{
}

mxr::Geode::~Geode()
{

}

void mxr::Geode::accept(asset_ref<NodeVisitor> nv)
{
	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->accept(nv);
	}
}
