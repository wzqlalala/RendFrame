#include "RenderPass.h"
#include "StateSet.h"

namespace mxr
{
	void SubPass::setPass(asset_ref<StateSet> state, asset_ref<FBO> fbo, asset_ref<Clear> clear)
	{
		_state = state;
		_fbo = fbo;
		_clear = clear;
		if (!clear)
		{
			clear = MakeAsset<Clear>(QVector4D(0, 0, 0, 1), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

	}

	void SubPass::Bind()
	{
		_fbo->Bind();
		_clear->apply();
		_state->apply();
	}

	void SubPass::UnBind()
	{
		_fbo->Unbind();
	}


	void SubPass::CopyToScreen()
	{

	}

	void SubPass::DrawQuad()
	{

	}

	RenderPass::RenderPass()
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void RenderPass::Bind(int index)
	{
		_subpasslist[index]->Bind();
	}

	void RenderPass::UnBind(int index)
	{
		_subpasslist[index]->UnBind();
	}

	void RenderPass::DrawQuad()
	{

	}

}