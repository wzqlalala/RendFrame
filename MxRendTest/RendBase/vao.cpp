#include "vao.h"
#include <qdebug.h>

#include "Drawable.h"

namespace mxr
{
	static GLuint curr_bound_vertex_array = 0;  // smart binding

	VAO::VAO() : IAsset()
	{
		glCreateVertexArrays(1, &id);

	}

	VAO::~VAO()
	{
		Unbind();
		glDeleteVertexArrays(1, &id);
	}

	void VAO::Bind()
	{
		if (id != curr_bound_vertex_array) {
			glBindVertexArray(id);
			curr_bound_vertex_array = id;
		}
	}

	void VAO::Unbind()
	{
		if (curr_bound_vertex_array == id)
		{
			curr_bound_vertex_array = 0;
			glBindVertexArray(0);
		}
	}



	void VAO::SetFormat(GLuint bind_id, GLuint attr_id, GLint offset, GLint size, GLenum type)
	{
		glEnableVertexArrayAttrib(id, attr_id);
		glVertexArrayAttribBinding(id, attr_id, bind_id);

		switch (type)
		{
		case GL_HALF_FLOAT:
		case GL_FLOAT:
		{
			glVertexArrayAttribFormat(id, attr_id, size, type, GL_FALSE, offset); break;
		}
		case GL_UNSIGNED_INT:
		case GL_INT:
		{
			glVertexArrayAttribIFormat(id, attr_id, size, type, offset); break;  // notice the "I" here
		}
		case GL_DOUBLE:
		{
			glVertexArrayAttribLFormat(id, attr_id, size, type, offset); break;  // notice the "L" here
		}
		default:
		{
			//throw NotImplementedError("Unsupported vertex attribute type!"); break;
		}
		}


	}

	void VAO::SetVBO(GLuint vbo, GLuint bind_id, GLint offset, GLint stride)
	{
		glVertexArrayVertexBuffer(id, bind_id, vbo, offset, stride);
	}

	void VAO::SetIBO(GLuint ibo)
	{
		
		glVertexArrayElementBuffer(id, ibo);
		
	}

	void VAO::SetDrawBuffer(GLuint did)
	{
		_drawbuffer = did;
	}

	void VAO::DrawArray(GLenum mode, GLsizei count)
	{
		Bind();
		glDrawArrays(mode, 0, count);
		Unbind();
	}

	void VAO::DrawElement(GLenum mode, GLsizei count)
	{
		Bind();
		glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
		Unbind();  // with smart bindings, we never need to unbind after the draw call	
	}

	
	void VAO::DrawIndirectArray(GLenum mode, int _offset, int count)
	{
		Bind();
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _drawbuffer);
		glMultiDrawArraysIndirect(mode, (void*)_offset, count, 0);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		Unbind();

	}

	void VAO::DrawArrays(std::vector<DrawArraysIndirectCommand> draws, GLenum mode)
	{
		Bind();
		for (size_t i = 0; i < draws.size(); i++)
		{
			glDrawArrays(mode, draws.at(i).firstVertex, draws.at(i).vertexCount);
		}
		Unbind();

	}

	void VAO::DrawIndirectElement(GLenum mode, int _offset, int count)
	{
		
		Bind();
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _drawbuffer);
		glMultiDrawElementsIndirect(mode, GL_UNSIGNED_INT, (void*)_offset, count, 0);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
		Unbind();		

	}

}

