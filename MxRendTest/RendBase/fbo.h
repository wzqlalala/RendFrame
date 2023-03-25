#pragma once
#include <vector>
#include "texture.h"

namespace mxr
{
	class MXR_EXPORT RBO : public IAsset
	{
	public:
		RBO(GLuint width, GLuint height, bool multisample = false);
		~RBO();
		void Bind()  override;
		void Unbind()  override;
	};


	class MXR_EXPORT FBO : public IAsset
	{
	private:
		GLenum status;
		GLuint width, height;
		std::vector<Texture> color_attachments;   // vector of color attachments
		asset_tmp<RBO>       depst_renderbuffer;  // depth and stencil as a single renderbuffer
		asset_tmp<Texture>   depst_texture;       // depth and stencil as a single texture
		asset_tmp<TexView>   stencil_view;        // stencil as a temporary texture view
	public:
		FBO() = default;
		FBO(GLuint width, GLuint height);
		~FBO();
		FBO(const FBO&) = delete;
		FBO& operator=(const FBO&) = delete;
		FBO(FBO&& other) noexcept = default;
		FBO& operator=(FBO&& other) noexcept = default;
	public:
		//void AddColorTexture(GLuint count, bool multisample = false);
		void SetColorTexture(GLenum index, GLuint texture_2d);
		void SetColorTexture(GLenum index, GLuint texture_cubemap, GLuint face);
		void AddDepStTexture(bool multisample = false);
		void AddDepStRenderBuffer(bool multisample = false);
		void AddDepthCubemap();
		const Texture& GetColorTexture(GLenum index) const;
		const Texture& GetDepthTexture() const;
		const TexView& GetStencilTexView() const;
		void Bind()  override;
		void Unbind() override;
		void SetDrawBuffer(GLuint index);
		void SetDrawBuffers(std::vector<GLuint> indices);
		void SetDrawBuffers();
		void Draw(GLint index);
		void Clear(GLint index);
		void Clear();
	public:
		void CopyColor(const FBO& fr, GLuint fr_idx, const FBO& to, GLuint to_idx);
		void CopyDepth(const FBO& fr, const FBO& to);
		void CopyStencil(const FBO& fr, const FBO& to);
	};
}

