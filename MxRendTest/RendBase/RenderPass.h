#pragma once
#include "fbo.h"
#include "StateAttribute.h"
namespace mxr
{
	class StateSet;
	class MXR_EXPORT SubPass
	{
	private:
		asset_ref<StateSet> _state;
		asset_ref<FBO> _fbo;
		asset_ref<Clear> _clear;
	public:
		SubPass() {}
		~SubPass() {}
		void setPass(asset_ref<StateSet> state, asset_ref<FBO> fbo, asset_ref<Clear> clear = nullptr);
		void Bind();
		void UnBind();
		void CopyToScreen();
		void DrawQuad();

	};




	class MXR_EXPORT RenderPass
	{
	private:
		std::vector<asset_ref<SubPass>> _subpasslist;
	public:
		RenderPass();
		~RenderPass();
		void addSubPass(asset_ref<SubPass> subpass) { _subpasslist.push_back(subpass); }
		int getpassSize() const { return _subpasslist.size(); }
		void Bind(int index);
		void UnBind(int index);
		void DrawQuad();

	};

}