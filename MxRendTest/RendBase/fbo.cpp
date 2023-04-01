
#include "log.h"

#include "vao.h"
#include "fbo.h"
#include "shader.h"
#include "texture.h"
#include "buffer.h"

namespace mxr
{
	// optimize context switching by avoiding unnecessary binds and unbinds
	static GLuint curr_bound_renderbuffer = 0;
	static GLuint curr_bound_framebuffer = 0;

	static asset_tmp<VAO> internal_vao = nullptr;
	static std::vector<asset_ref<VBO>> internal_vbos;
	static asset_tmp<Shader> internal_shader = nullptr;


	static GLuint DefaultFrameBuffer = 0;
	static void setDefaultFrame(GLuint frame)
	{
		DefaultFrameBuffer = frame;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	RBO::RBO(GLuint width, GLuint height, bool multisample) : IAsset()
	{
		glCreateRenderbuffers(1, &id);

		if (multisample)
		{
			glNamedRenderbufferStorageMultisample(id, 4, GL_DEPTH24_STENCIL8, width, height);
		}
		else
		{
			glNamedRenderbufferStorage(id, GL_DEPTH24_STENCIL8, width, height);
		}
	}

	RBO::~RBO()
	{
		glDeleteRenderbuffers(1, &id);
	}

	void RBO::Bind()
	{
		if (id != curr_bound_renderbuffer) {
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			curr_bound_renderbuffer = id;
		}
	}

	void RBO::Unbind()
	{
		if (curr_bound_renderbuffer == id)
		{
			curr_bound_renderbuffer = 0;
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	FBO::FBO(GLuint width, GLuint height) : IAsset(), width(width), height(height), status(0)
	{
		glDisable(GL_FRAMEBUFFER_SRGB);  // important! turn off colorspace correction globally
		glCreateFramebuffers(1, &id);
		if (!internal_vao)
		{
			internal_vao = WrapAsset<VAO>();
			float vertices[] =
			{
				 0.5f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 -0.5f,  0.5f, 0.0f,

				 0.5f, -0.5f, 0.0f,
				 -0.5f, -0.5f, 0.0f,
				 -0.5f,  0.5f, 0.0f
			};

			float coords[] =
			{
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 1.0f,

				1.0f, 0.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,
			};

			asset_ref<VBO> ver = MakeAsset<VBO>(sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);
			asset_ref<VBO> coo = MakeAsset<VBO>(sizeof(coords), coords, GL_DYNAMIC_STORAGE_BIT);
			internal_vbos.push_back(ver);
			internal_vbos.push_back(coo);
			internal_vao->SetFormat(0, 0, 0, 3, GL_FLOAT);
			internal_vao->SetFormat(1, 1, 0, 2, GL_FLOAT);
			internal_vao->SetVBO(ver->ID(), 0, 0, 12);
			internal_vao->SetVBO(coo->ID(), 1, 0, 8);
		}


		if (!internal_shader)
		{
			//internal_shader = WrapAsset<Shader>(utils::paths::shader + "core\\framebuffer.glsl");
		}
	}

	FBO::~FBO()
	{
		Unbind();
		glDeleteFramebuffers(1, &id);
	}


	void FBO::SetColorTexture(GLenum index, GLuint texture_2d)
	{
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + index, texture_2d, 0);
		SetDrawBuffers();
		status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
	}

	void FBO::SetColorTexture(GLenum index, GLuint texture_cubemap, GLuint face)
	{

		if constexpr (true)
		{
			// some Intel drivers do not support this DSA function
			glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + index, texture_cubemap, 0, face);
		}
		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, texture_cubemap, 0);
		}

		SetDrawBuffers();
		status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
	}

	void FBO::AddDepStTexture(bool multisample)
	{
		// a framebuffer can only have one depth stencil buffer, either as a texture or a renderbuffer
		//CORE_ASERT(!depst_renderbuffer, "The framebuffer already has a depth stencil renderbuffer...");
		//CORE_ASERT(!depst_texture, "Only one depth stencil texture can be attached to the framebuffer...");

		// depth stencil textures are meant to be filtered anyway, it doesn't make sense to use a depth
		// stencil texture for MSAA because filtering on multisampled textures is not allowed by OpenGL.
		if (multisample) {
			CORE_ERROR("Multisampled depth stencil texture is not supported, it is a waste of memory!");
			CORE_ERROR("If you need MSAA, please add a multisampled renderbuffer (RBO) instead...");
			return;
		}

		// depth and stencil values are combined in a single immutable-format texture
		// each 32-bit pixel contains 24 bits of depth value and 8 bits of stencil value
		depst_texture = WrapAsset<Texture>(GL_TEXTURE_2D, width, height, 1, GL_DEPTH24_STENCIL8, 1);
		glTextureParameteri(depst_texture->ID(), GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);

		GLint immutable_format;
		glGetTextureParameteriv(depst_texture->ID(), GL_TEXTURE_IMMUTABLE_FORMAT, &immutable_format);
		//CORE_ASERT(immutable_format == GL_TRUE, "Unable to attach an immutable depth stencil texture...");

		// to access the stencil values in GLSL, we need a separate texture view
		stencil_view = WrapAsset<TexView>(*depst_texture);
		stencil_view->SetView(GL_TEXTURE_2D, 0, 1, 0, 1);
		glTextureParameteri(stencil_view->ID(), GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);

		glNamedFramebufferTexture(id, GL_DEPTH_STENCIL_ATTACHMENT, depst_texture->ID(), 0);
		status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
	}

	void FBO::AddDepStRenderBuffer(bool multisample) {
		// a framebuffer can only have one depth stencil buffer, either as a texture or a renderbuffer
		//CORE_ASERT(!depst_texture, "The framebuffer already has a depth stencil texture...");
		//CORE_ASERT(!depst_renderbuffer, "Only one depth stencil renderbuffer can be attached to the framebuffer...");

		depst_renderbuffer = WrapAsset<RBO>(width, height, multisample);
		depst_renderbuffer->Bind();
		glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depst_renderbuffer->ID());

		status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
	}

	void FBO::AddDepthCubemap()
	{
		// a framebuffer can only have one depth stencil buffer, either as a texture or a renderbuffer
		//CORE_ASERT(!depst_renderbuffer, "The framebuffer already has a depth stencil renderbuffer...");
		//CORE_ASERT(!depst_texture, "Only one depth stencil texture can be attached to the framebuffer...");


		depst_texture = WrapAsset<Texture>(GL_TEXTURE_CUBE_MAP, width, height, 6, GL_DEPTH_COMPONENT24, 1);
		GLuint tid = depst_texture->ID();

		glTextureParameteri(tid, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
		glTextureParameteri(tid, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(tid, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(tid, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(tid, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(tid, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glNamedFramebufferTexture(id, GL_DEPTH_ATTACHMENT, tid, 0);
		const GLenum null[] = { GL_NONE };
		glNamedFramebufferReadBuffer(id, GL_NONE);
		glNamedFramebufferDrawBuffers(id, 1, null);

		status = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
	}

	const Texture& FBO::GetColorTexture(GLenum index) const
	{
		//CORE_ASERT(index < color_attachments.size(), "Invalid color attachment index: {0}", index);
		return color_attachments[index];
	}

	const Texture& FBO::GetDepthTexture() const
	{
		//CORE_ASERT(depst_texture, "The framebuffer does not have a depth texture...");
		return *depst_texture;
	}

	const TexView& FBO::GetStencilTexView() const
	{
		//CORE_ASERT(stencil_view, "The framebuffer does not have a stencil texture view...");
		return *stencil_view;
	}

	void FBO::Bind()
	{
		if (id != curr_bound_framebuffer)
		{
			//CORE_ASERT(status == GL_FRAMEBUFFER_COMPLETE, "Incomplete framebuffer status: {0}", status);
			if (depst_renderbuffer)
			{
				depst_renderbuffer->Bind();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, id);
			curr_bound_framebuffer = id;
		}
	}

	void FBO::Unbind()
	{
		if (curr_bound_framebuffer == id)
		{
			curr_bound_framebuffer = DefaultFrameBuffer;
			glBindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBuffer);
		}
	}

	void FBO::SetDrawBuffer(GLuint index)
	{
		// this function only enables a single color attachment for writing
		//CORE_ASERT(index < color_attachments.size(), "Color buffer index out of bound!");
		const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 + index };
		glNamedFramebufferDrawBuffers(id, 1, buffers);
	}

	void FBO::SetDrawBuffers(std::vector<GLuint> indices)
	{
		// this function enables the input list of color attachments for writing
		size_t n_buffs = color_attachments.size();
		size_t n_index = indices.size();
		GLenum* buffers = new GLenum[n_index];

		for (GLenum i = 0; i < n_index; i++)
		{
			// the `layout(location = i) out` variable will write to this attachment
			GLuint index = indices[i];
			//CORE_ASERT(index < n_buffs, "Color buffer index {0} out of bound!", index);
			*(buffers + i) = GL_COLOR_ATTACHMENT0 + index;
		}

		glNamedFramebufferDrawBuffers(id, n_index, buffers);
		delete[] buffers;
	}

	void FBO::SetDrawBuffers()
	{
		// enable all color attachments for writing
		if (size_t n = color_attachments.size(); n > 0) {
			GLenum* attachments = new GLenum[n];

			for (GLenum i = 0; i < n; i++) {
				*(attachments + i) = GL_COLOR_ATTACHMENT0 + i;
			}

			glNamedFramebufferDrawBuffers(id, n, attachments);
			delete[] attachments;
		}
	}

	void FBO::Draw(GLint index)
	{
		internal_vao->Bind();
		internal_shader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void FBO::Clear(GLint index)
	{
		const GLfloat clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		const GLfloat clear_depth = 1.0f;
		const GLint clear_stencil = 0;

		// clear one of the color attachments
		if (index >= 0)
		{
			glClearNamedFramebufferfv(id, GL_COLOR, index, clear_color);
		}
		// clear the depth buffer
		else if (index == -1)
		{
			glClearNamedFramebufferfv(id, GL_DEPTH, 0, &clear_depth);
		}
		// clear the stencil buffer
		else if (index == -2)
		{
			glClearNamedFramebufferiv(id, GL_STENCIL, 0, &clear_stencil);
		}
		else
		{
			CORE_ERROR("Buffer index {0} is not valid in the framebuffer!", index);
		}
	}

	void FBO::Clear()
	{
		for (int i = 0; i < color_attachments.size(); i++)
		{
			Clear(i);
		}

		Clear(-1);
		Clear(-2);
	}

	void FBO::CopyColor(const FBO& fr, GLuint fr_idx, const FBO& to, GLuint to_idx)
	{
		//CORE_ASERT(fr_idx < fr.color_attachments.size(), "Color buffer index {0} out of bound...", fr_idx);
		//CORE_ASERT(to_idx < to.color_attachments.size(), "Color buffer index {0} out of bound...", to_idx);

		// if the source and target rectangle areas differ in size, interpolation will be applied
		GLuint fw = fr.width, fh = fr.height;
		GLuint tw = to.width, th = to.height;

		glNamedFramebufferReadBuffer(fr.id, GL_COLOR_ATTACHMENT0 + fr_idx);
		glNamedFramebufferDrawBuffer(to.id, GL_COLOR_ATTACHMENT0 + to_idx);
		glBlitNamedFramebuffer(fr.id, to.id, 0, 0, fw, fh, 0, 0, tw, th, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void FBO::CopyDepth(const FBO& fr, const FBO& to)
	{
		// make sure that GL_FRAMEBUFFER_SRGB is globally disabled when calling this function!
		// if colorspace correction is enabled, depth values will be gamma encoded during blits...
		GLuint fw = fr.width, fh = fr.height;
		GLuint tw = to.width, th = to.height;
		glBlitNamedFramebuffer(fr.id, to.id, 0, 0, fw, fh, 0, 0, tw, th, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void FBO::CopyStencil(const FBO& fr, const FBO& to)
	{
		// make sure that GL_FRAMEBUFFER_SRGB is globally disabled when calling this function!
		// if colorspace correction is enabled, stencil values will be gamma encoded during blits...
		GLuint fw = fr.width, fh = fr.height;
		GLuint tw = to.width, th = to.height;
		glBlitNamedFramebuffer(fr.id, to.id, 0, 0, fw, fh, 0, 0, tw, th, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}
}

