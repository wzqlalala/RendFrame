#pragma once
#include "Drawable.h"
#include "NodeVisitor.h"
#include "StateAttribute.h"

#include <QTime>

namespace mxr
{
	extern MXR_EXPORT QTime *time;
	class MXR_EXPORT Viewer
	{
	public:
		Viewer();
		void setSceneData(asset_ref<Node> _node) { _sceneData = _node; }
		void compile();
		void noClearRun();
		void run();

	protected:
		asset_ref<Node> _sceneData;
		asset_ref<NodeVisitor> _visitor;
		asset_ref<Clear> _clear;
	};

}