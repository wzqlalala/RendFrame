#pragma once
#include <string>
#include "asset.h"

namespace mxr
{
	class Texture;
	class MXR_EXPORT TexView : public IAsset
	{
	public:
		const Texture& host;
		TexView(const Texture& texture);
		TexView(const Texture&& texture) = delete;  // prevent rvalue references to temporary objects
		~TexView();

		TexView(const TexView&) = delete;
		TexView& operator=(const TexView&) = delete;
		TexView(TexView&& other) = default;
		TexView& operator=(TexView&& other) = default;

		void SetView(GLenum target, GLuint fr_level, GLuint levels, GLuint fr_layer, GLuint layers);
		void Bind(GLuint index);
		void Unbind(GLuint index);
	};

	class MXR_EXPORT Texture : public IAsset
	{
	private:
		friend class TexView;
		GLenum target;
		GLenum format, i_format;  // internal format
		void SetSampleState();

	public:
		GLuint width, height, depth;
		GLuint n_levels;

		Texture(const std::string& img_path, GLuint levels = 0);
		Texture(const std::string& img_path, GLuint resolution, GLuint levels);
		Texture(const std::string& directory, const std::string& extension, GLuint resolution, GLuint levels);
		Texture(GLenum target, GLuint width, GLuint height, GLuint depth, GLenum i_format, GLuint levels);
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&& other) noexcept = default;
		Texture& operator=(Texture&& other) noexcept = default;

		void Bind(GLuint index)  override;
		void Unbind(GLuint index)  override;
		void BindILS(GLuint level, GLuint index, GLenum access);
		void UnbindILS(GLuint index);

		void GenerateMipmap();
		void Clear(GLuint level);
		void Invalidate(GLuint level);

		void PixelStorei(GLenum panel, GLint param);
		void Copy(const Texture& fr, GLuint fr_level, const Texture& to, GLuint to_level);
		void SetData(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void*pixels);
		void SetData(GLint level, GLint xoffset, GLsizei width, const void*pixels);

		void setTexParameteri(GLint min_filter, GLint mag_filter);
	};

}

