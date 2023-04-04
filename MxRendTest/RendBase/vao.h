#pragma once

#include "asset.h"

#include "Node.h"

#include <vector>

//#include "Drawable.h"

//struct DrawArraysIndirectCommand;
namespace mxr
{
	class MXR_EXPORT VAO : public IAsset
	{
	public:
		VAO();
		~VAO();

		VAO(const VAO&) = delete;
		VAO& operator=(const VAO&) = delete;
		VAO(VAO&& other) noexcept = default;
		VAO& operator=(VAO&& other) noexcept = default;

		void Bind()  override;
		void Unbind()  override;

		void SetFormat(GLuint bind_id, GLuint attr_id, GLint offset, GLint size, GLenum type);
		void SetVBO(GLuint vbo, GLuint bind_id, GLint offset, GLint stride);
		void SetIBO(GLuint ibo);
		void SetDrawBuffer(GLuint did);
		void DrawArray(GLenum mode, GLsizei count);
		void DrawElement(GLenum mode, GLsizei count);

		void DrawIndirectArray(GLenum mode, int _offset, int count);
		void DrawArrays(std::vector<DrawArraysIndirectCommand> draws, GLenum mode);
		void DrawIndirectElement(GLenum mode, int _offset, int count);
	protected:
		GLuint _drawbuffer;
	};

}
