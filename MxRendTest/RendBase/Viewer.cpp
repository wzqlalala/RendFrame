#include "Viewer.h"
#include "Group.h"

namespace mxr
{
	Viewer::Viewer()
	{
		_visitor = MakeAsset<NodeVisitor>();
		_clear = MakeAsset<Clear>();
	}

	void Viewer::run()
	{
		_sceneData->accept(_visitor);
		_visitor->compile();
		_visitor->clear();

		//»æÖÆ
		_clear->apply();
		_visitor->run();

	}
}